#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Video.h"


j1Video::j1Video()
{
	baseticks = 0;
	framems = 0;
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
	//TODO 1: Decode the format. To do that we use THEORAPLAY_Decoder
	THEORAPLAY_Decoder* decoder = THEORAPLAY_startDecodeFile(file, 30, THEORAPLAY_VIDFMT_IYUV);
	if (!decoder)
	{
		LOG("Failed to start decoding file");
		return 0;
	}

	//TODO 2: After that we need to create a vido and audio buffer and fill them with THEORAPLAY_getAudio and THEORAPLAY_getVideo. 
	const THEORAPLAY_VideoFrame* video = NULL;

	while (!video)
	{
		
		if (!video) video = THEORAPLAY_getVideo(decoder);
		SDL_Delay(10);
	}

	SDL_Texture* overlay = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, video->width, video->height);
	if (overlay == 0)
	{
		// Error set by SDL_CreateTexture()
		THEORAPLAY_freeVideo(video);
		THEORAPLAY_stopDecode(decoder);
		return 0;
	}

	// Create video item
	j1Video* player = (j1Video*)malloc(sizeof(j1Video));
	player->video = video;
	player->decoder = decoder;
	player->texture = overlay;
	player->baseticks = SDL_GetTicks();
	player->framems = (video->fps == 0.0) ? 0 : ((Uint32)(1000.0 / video->fps));

	


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


	return vid->texture;
}

void j1Video::DestroyVideo(int video)
{
	j1Video* vid = (j1Video*)video;

	//TODO 4: Just free everything the texture, video, audio and decoder:
	if (vid != nullptr) {
		SDL_DestroyTexture(vid->texture);
		THEORAPLAY_freeVideo(vid->video);
		THEORAPLAY_stopDecode(vid->decoder);
	}
}

int j1Video::IsPlaying(int video)
{
	return THEORAPLAY_isDecoding(((j1Video*)video)->decoder);
}

