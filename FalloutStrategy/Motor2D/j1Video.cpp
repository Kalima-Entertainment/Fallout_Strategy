#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Video.h"


j1Video::j1Video()
{
}

j1Video::~j1Video()
{
}

bool j1Video::Awake(pugi::xml_node&)
{

	return true;
}

bool j1Video::Start()
{

	return true;
}

bool j1Video::PreUpdate()
{

	return true;
}

bool j1Video::Update(float dt)
{

	return true;
}

bool j1Video::PostUpdate()
{

	return true;
}

bool j1Video::CleanUp()
{

	return true;
}


int j1Video::Load(const char* file, SDL_Renderer* renderer)
{
	//TODO 2: After that we need to create a vido and audio buffer and fill them with THEORAPLAY_getAudio and THEORAPLAY_getVideo. 
	const THEORAPLAY_AudioPacket* audio = NULL;
	const THEORAPLAY_VideoFrame* video = NULL;

	//TODO 1: Decode the format. To do that we use THEORAPLAY_Decoder
	THEORAPLAY_Decoder* decoder = THEORAPLAY_startDecodeFile(file, 30, THEORAPLAY_VIDFMT_IYUV);
	if (!decoder)
	{
		LOG("Failed to start decoding file");
		return 0;
	}


	while (!audio || !video)
	{
		if (!audio) audio = THEORAPLAY_getAudio(decoder);
		if (!video) video = THEORAPLAY_getVideo(decoder);
		SDL_Delay(10);
	}

	SDL_Texture* overlay = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, video->width, video->height);
	if (overlay == 0)
	{
		// Error set by SDL_CreateTexture()
		THEORAPLAY_freeAudio(audio);
		THEORAPLAY_freeVideo(video);
		THEORAPLAY_stopDecode(decoder);
		return 0;
	}

	// Create video item
	j1Video* player = (j1Video*)malloc(sizeof(j1Video));
	player->audio = audio;
	player->video = video;
	player->decoder = decoder;
	player->texture = overlay;
	player->baseticks = SDL_GetTicks();
	player->framems = (video->fps == 0.0) ? 0 : ((Uint32)(1000.0 / video->fps));

	//Load audio specs
	memset(&player->audio_spec, '\0', sizeof(SDL_AudioSpec));
	player->audio_spec.freq = audio->freq;
	player->audio_spec.format = AUDIO_S16SYS;
	player->audio_spec.channels = audio->channels;
	player->audio_spec.samples = 2048;
	player->audio_spec.callback = AudioCallback;

	if (SDL_OpenAudio(&player->audio_spec, NULL) != 0)
	{
		//Error set by SDL_OpenAudio()
		THEORAPLAY_freeAudio(audio);
		THEORAPLAY_freeVideo(video);
		THEORAPLAY_stopDecode(decoder);
		return 0;
	}

	//After the audio packet is processed we process the next audio packet
	while (audio)
	{
		QueueAudio(audio);
		audio = THEORAPLAY_getAudio(decoder);
	}

	SDL_PauseAudio(0);

	return (int)player;
}

SDL_Texture* j1Video::UpdateVideo(int video_id)
{
	j1Video* vid = (j1Video*)video_id;

	if (THEORAPLAY_isDecoding(vid->decoder))
	{
		//Get and process the video data. For that in the main loop we get the video from the decoder.
		if (!vid->video)
			vid->video = THEORAPLAY_getVideo(vid->decoder);

		const Uint32 now = SDL_GetTicks() - vid->baseticks;


		if (vid->video && (vid->video->playms <= now))
		{
			// Play video frames when it's time. Each frame need to be rendered during a certain amount of time, the time is specified in video->playms. 
			// If the time elapsed is greater thant this we go to the next frame.
			if (vid->framems && ((now - vid->video->playms) >= vid->framems))
			{
				const THEORAPLAY_VideoFrame* last = vid->video;
				// TODO 3: While ((vid->video = THEORAPLAY_getVideo(vid->decoder)) != NULL) free the last frame and go to the next one 
				while ((vid->video = THEORAPLAY_getVideo(vid->decoder)) != NULL)
				{
					THEORAPLAY_freeVideo(last);
					last = vid->video;
					if ((now - vid->video->playms) < vid->framems)
						break;
				}

				if (!vid->video)
					vid->video = last;
			}

			if (!vid->video)
			{
				LOG("WARNING: Playback can't keep up!");
				return NULL;
			}
			else
			{

				int half_w = vid->video->width / 2;

				const Uint8* y = (const Uint8*)vid->video->pixels;
				const Uint8* u = y + (vid->video->width * vid->video->height);
				const Uint8* v = u + (half_w * (vid->video->height / 2));

				SDL_UpdateYUVTexture(vid->texture, NULL, y, vid->video->width, u, half_w, v, half_w);
			}
			THEORAPLAY_freeVideo(vid->video);
			vid->video = NULL;
		}
	}

	//For echa frame the audio need to be processed again
	while ((vid->audio = THEORAPLAY_getAudio(vid->decoder)) != NULL)
		QueueAudio(vid->audio);

	return vid->texture;
}

void j1Video::DestroyVideo(int video)
{
	j1Video* vid = (j1Video*)video;

	while (audio_queue != NULL)
	{
		SDL_LockAudio();
		SDL_UnlockAudio();
		SDL_Delay(10);
	}

	//TODO 4: Just free everything the texture, video, audio and decoder:
	SDL_DestroyTexture(vid->texture);
	THEORAPLAY_freeVideo(vid->video);
	THEORAPLAY_freeAudio(vid->audio);
	THEORAPLAY_stopDecode(vid->decoder);
}

int j1Video::IsPlaying(int video)
{
	return THEORAPLAY_isDecoding(((j1Video*)video)->decoder);
}

void j1Video::QueueAudio(const THEORAPLAY_AudioPacket* audio)
{
	AudioQueue* item = (AudioQueue*)malloc(sizeof(AudioQueue));
	if (!item)
	{
		THEORAPLAY_freeAudio(audio);
		return;
	}

	item->audio = audio;
	item->offset = 0;
	item->next = NULL;

	SDL_LockAudio();
	if (audio_queue_tail)
		audio_queue_tail->next = item;
	else
		audio_queue = item;
	audio_queue_tail = item;
	SDL_UnlockAudio();
}

void SDLCALL j1Video::AudioCallback(void* userdata, Uint8* stream, int len)
{

	Sint16* dst = (Sint16*)stream;

	while (audio_queue && (len > 0))
	{
		volatile AudioQueue* item = audio_queue;
		AudioQueue* next = item->next;
		const int channels = item->audio->channels;

		const float* src = item->audio->samples + (item->offset * channels);
		int cpy = (item->audio->frames - item->offset) * channels;
		int i;

		if (cpy > (len / sizeof(Sint16)))
			cpy = len / sizeof(Sint16);

		for (i = 0; i < cpy; i++)
		{
			const float val = *(src++);
			if (val < -1.0f)
				*(dst++) = -32768;
			else if (val > 1.0f)
				*(dst++) = 32767;
			else
				*(dst++) = (Sint16)(val * 32767.0f);
		}

		item->offset += (cpy / channels);
		len -= cpy * sizeof(Sint16);

		if (item->offset >= item->audio->frames)
		{
			THEORAPLAY_freeAudio(item->audio);
			free((void*)item);
			audio_queue = next;
		}
	}

	if (!audio_queue)
		audio_queue_tail = NULL;

	if (len > 0)
		memset(dst, '\0', len);

}


