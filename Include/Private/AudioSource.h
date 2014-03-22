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

enum WaveformType
{
	W_SQUARE,
	W_SAW,
	W_SINE
};

struct Waveform
{
	float Frequency;
	float Amplitude;
	float Duration;
	float Delay;
	float Offset;
	Waveform(float f, float a, float dur, float del) : Frequency(f), Amplitude(a), Duration(dur), Delay(del), Offset(0.0f) {}
};

struct Noise
{
	float Amplitude;
	float Duration;
	float Delay;
	Noise(float a, float dur, float del) : Amplitude(a), Duration(dur), Delay(del) {}
};

class AudioSource
{
public:
	virtual bool Init() = 0;
	virtual bool Cleanup() = 0;
	virtual bool Start() = 0;
	virtual bool Pause() = 0;
	virtual bool Stop() = 0;
	virtual bool IsPlaying() = 0;

	virtual bool AddProcedural(WaveformType type, Waveform waveform) = 0;
};

#endif // AudioSource_h__
