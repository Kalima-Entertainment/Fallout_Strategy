#ifndef _VIDEO_H_
#define _VIDEO_H_

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

#include "j1Module.h"
#include "theoraplay.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_version.h"

typedef struct AudioQueue
{
	const THEORAPLAY_AudioPacket* audio;
	int offset;
	struct AudioQueue* next;
} AudioQueue;

static volatile AudioQueue* audio_queue = NULL;
static volatile AudioQueue* audio_queue_tail = NULL;


class j1Video : public j1Module
{
public:
	j1Video();
	~j1Video();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	THEORAPLAY_Decoder* decoder;
	const THEORAPLAY_VideoFrame* video;
	SDL_Texture* texture;
	SDL_AudioSpec audio_spec;
	Uint32 baseticks;
	Uint32 framems;

	int SDLCALL Load(const char* file, SDL_Renderer* renderer);
	SDL_Texture* SDLCALL UpdateVideo(int video);
	int SDLCALL IsPlaying(int video);
	void SDLCALL DestroyVideo(int video);
	static void SDLCALL AudioCallback(void* userdata, Uint8* stream, int len);
	static void QueueAudio(const THEORAPLAY_AudioPacket* audio);

private:


};

#endif
