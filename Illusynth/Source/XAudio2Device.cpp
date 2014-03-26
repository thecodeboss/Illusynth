#include <Private\XAudio2Device.h>

#ifdef _WINDOWS

/************************************************************************/
/* Static Members                                                       */
/************************************************************************/

XAudio2Device* XAudio2Device::m_Instance = nullptr;
IXAudio2* XAudio2Device::XAudio2;
IXAudio2MasteringVoice* XAudio2Device::XAudio2MasteringVoice;


/************************************************************************/
/* Non-member helper functions                                          */
/************************************************************************/

DWORD WINAPI StreamProc( LPVOID pContext )
{
	if (!pContext) return -1;
	return ((XAudioStreamContext*)pContext)->AudioDevice->StreamThreadMain(((XAudioStreamContext*)pContext)->Source);
}


/************************************************************************/
/* XAudio2Device Implementation                                         */
/************************************************************************/

XAudio2Device::XAudio2Device()
{
}

XAudio2Device* XAudio2Device::Get()
{
	if (m_Instance == nullptr) m_Instance = new XAudio2Device();
	return m_Instance;
}

bool XAudio2Device::Init()
{
	// Set thread concurrency model
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// Initialize the XAudio2 instance
	if (XAudio2Create(&XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR) < 0)
	{
		return false;
	}

	// Initialize the mastering voice to handle all audio
	HRESULT hr = XAudio2->CreateMasteringVoice(&XAudio2MasteringVoice);
	if (hr < 0)
	{
		return false;
	}

	return true;
}

bool XAudio2Device::Cleanup()
{
	XAudio2->Release();
	CoUninitialize();
	return true;
}


AudioSource* XAudio2Device::CreateSoundSource(AudioSourceType type, INT EffectFlags)
{
	return CreateSourceVoice(type, EffectFlags);
}


XAudio2SourceVoice* XAudio2Device::CreateSourceVoice(AudioSourceType type, INT EffectFlags)
{
	// @ILLUSYNTH_TODO: Need to create custom allocators

	XAudio2SourceVoice* ReturnValue = nullptr;

	switch (type)
	{
	case S_PROCEDURAL:
		ReturnValue = new XAudio2ProceduralSourceVoice();
	case S_WAVE:
		break;
	}

	ReturnValue->m_EffectFlags = EffectFlags;
	ReturnValue->Init();

	return ReturnValue;
}


bool XAudio2Device::PlaySource(AudioSource* Source)
{
	return PlaySourceVoice((XAudio2SourceVoice*)Source);
}

bool XAudio2Device::PlaySourceVoice( XAudio2SourceVoice* source )
{
	source->m_bPlaying = true;

	// @ILLUSYNTH_TODO
	XAudioStreamContext* StreamContext = new XAudioStreamContext(this, source);

	DWORD dwThreadId = 0;
	HANDLE StreamingVoiceThread = CreateThread( NULL, 0, StreamProc, StreamContext, 0, &dwThreadId );
	if( StreamingVoiceThread == NULL )
	{
		return false;
	}

	return true;
}

DWORD WINAPI XAudio2Device::StreamThreadMain( XAudio2SourceVoice* source )
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (!source->m_bInitialized)
	{
		source->Init();
	}

	source->Start();
	source->Stop();
	source->Cleanup();

	CoUninitialize();

	return 0;
}

#endif // _WINDOWS