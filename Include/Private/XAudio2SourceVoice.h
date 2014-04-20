#ifndef XAudio2SourceVoice_h__
#define XAudio2SourceVoice_h__
#ifdef _WINDOWS

#define _USE_MATH_DEFINES 1
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <Private\AudioSource.h>
#include <Private\XAudio2VoiceCallback.h>
#include <vector>

#define PROCEDURAL_BUFFER_SIZE 2048
#define MAX_BUFFER_COUNT 3

#define DEFAULT_PROCEDURAL_NUM_CHANNELS 1
#define DEFAULT_PROCEDURAL_SAMPLE_RATE 48000
#define DEFAULT_PROCEDURAL_BITS_PER_SAMPLE 16

enum XAudio2EffectIndices
{
	XAUDIO2_REVERB_EFFECT_INDEX = 0,
	XAUDIO2_EQ_EFFECT_INDEX = 1
};

class XAudio2SourceVoice : public AudioSource
{
	friend class XAudio2Device;
protected:
	IXAudio2SourceVoice* m_Source;
	WAVEFORMATEXTENSIBLE* m_wfx;
	BYTE* m_Buffers;
	bool m_bPlaying;
	bool m_bInitialized;

	XAudio2VoiceCallback m_Callback;

public:

	XAudio2SourceVoice(AudioSourceType type = S_WAVE);

	virtual bool Init();
	virtual bool Cleanup();
	virtual bool Start();
	virtual bool Pause();
	virtual bool Stop();
	virtual bool IsPlaying();

	virtual bool AddProcedural(WaveformType type, Waveform waveform) = 0;
	virtual size_t GetNumProcedural() = 0;
	virtual bool SetReverbSettings(float Diffusion = FXREVERB_DEFAULT_DIFFUSION, float RoomSize = FXREVERB_DEFAULT_ROOMSIZE);
};

#endif // _WINDOWS
#endif // XAudio2SourceVoice_h__
