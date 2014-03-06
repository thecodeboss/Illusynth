#ifndef AudioSource_h__
#define AudioSource_h__

#define DLL_SPECS_ONLY
#include <Illusynth.h>
#undef DLL_SPECS_ONLY

enum AudioSourceType
{
	S_WAVE,
	S_PROCEDURAL
};

class AudioSource
{
public:
	virtual bool Init() = 0;
	virtual bool Cleanup() = 0;
	virtual bool Start() = 0;
	virtual bool Pause() = 0;
	virtual bool Stop() = 0;
};

#endif // AudioSource_h__
