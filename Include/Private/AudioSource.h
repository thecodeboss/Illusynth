#ifndef AudioSource_h__
#define AudioSource_h__

#define DLL_SPECS_ONLY
#include <Illusynth.h>
#undef DLL_SPECS_ONLY

#include <Private\AudioEffects.h>
#include <string>

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
	static int AudioSourceCounter;
	int m_SourceID;
protected:
	int m_EffectFlags;
	ReverbSettings m_ReverbSettings;
	AudioSourceType m_Type;
	std::string m_Name;
	AudioSource(AudioSourceType type = S_WAVE);
	AudioSource(std::string name, AudioSourceType type = S_WAVE);
public:
	virtual bool Init() = 0;
	virtual bool Cleanup() = 0;
	virtual bool Start() = 0;
	virtual bool Pause() = 0;
	virtual bool Stop() = 0;
	virtual bool IsPlaying() = 0;
	std::string GetName() const;

	virtual bool AddProcedural(WaveformType type, Waveform waveform);
	virtual size_t GetNumProcedural() = 0;
	virtual bool SetReverbSettings(float Diffusion = 0.9f, float RoomSize = 0.6f);
	virtual bool SetFilterCutoff(int FilterHandle, float Cutoff);
};

#endif // AudioSource_h__
