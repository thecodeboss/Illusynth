#ifndef XAudio2SourceVoice_h__
#define XAudio2SourceVoice_h__
#ifdef _WINDOWS

#include <xaudio2.h>
#include <Private\AudioSource.h>
#include <Private\XAudio2VoiceCallback.h>

class XAudio2SourceVoice : public AudioSource
{
protected:
	IXAudio2SourceVoice* m_Source;
	WAVEFORMATEXTENSIBLE* m_wfx;
	AudioSourceType m_Type;
	BYTE* m_Buffers;

	XAudio2VoiceCallback* m_Callback;
public:

	XAudio2SourceVoice(AudioSourceType type = S_WAVE);

	virtual bool Init();
	virtual bool Cleanup();
	virtual bool Start();
	virtual bool Pause();
	virtual bool Stop();
};

#endif // _WINDOWS
#endif // XAudio2SourceVoice_h__
