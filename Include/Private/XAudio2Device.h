#ifndef XAudio2Device_h__
#define XAudio2Device_h__
#ifdef _WINDOWS

#include <xaudio2.h>
#include <Private\AudioDevice.h>
#include <Private\XAudio2SourceVoice.h>
#include <Private\XAudio2VoiceCallback.h>

DWORD WINAPI StreamProc(LPVOID pContext);

class XAudio2Device : public AudioDevice
{
	static XAudio2Device* m_Instance;
	static IXAudio2* XAudio2;
	static IXAudio2MasteringVoice* XAudio2MasteringVoice;

protected:
	XAudio2Device();
	XAudio2SourceVoice* CreateSourceVoice(AudioSourceType type, INT EffectFlags);
	bool PlaySourceVoice(XAudio2SourceVoice* source);

public:
	static XAudio2Device* Get();
	virtual bool Init();
	virtual bool Cleanup();
	virtual bool PlaySource(AudioSource* Source);

	virtual AudioSource* CreateSoundSource(AudioSourceType type, INT EffectFlags = ILLUSYNTH_FX_NONE);
	DWORD WINAPI StreamThreadMain(XAudio2SourceVoice* source);

	friend class AudioDevice;
	friend class XAudio2SourceVoice;
	friend class XAudio2VoiceCallback;
};

struct XAudioStreamContext
{
	XAudio2Device* AudioDevice;
	XAudio2SourceVoice* Source;
	XAudioStreamContext(XAudio2Device* x, XAudio2SourceVoice* s) : AudioDevice(x), Source(s) {}
};

#endif // _WINDOWS
#endif // XAudio2Device_h__