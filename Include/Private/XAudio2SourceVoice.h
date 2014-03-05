#ifndef XAudio2SourceVoice_h__
#define XAudio2SourceVoice_h__
#ifdef _WINDOWS

#include <xaudio2.h>
#include <Private\XAudio2VoiceCallback.h>

enum SourceVoiceType
{
	S_WAVE,
	S_PROCEDURAL
};

class XAudio2SourceVoice
{
protected:
	IXAudio2SourceVoice* m_Source;
	WAVEFORMATEXTENSIBLE* m_wfx;
	SourceVoiceType m_Type;
	BYTE* m_Buffers;

	XAudio2VoiceCallback* m_Callback;
public:

	XAudio2SourceVoice(SourceVoiceType type = S_WAVE);

	virtual HRESULT Init();
	virtual HRESULT Cleanup();
	virtual HRESULT Start();
	virtual HRESULT Pause();
	virtual HRESULT Stop();
};

#endif // _WINDOWS
#endif // XAudio2SourceVoice_h__
