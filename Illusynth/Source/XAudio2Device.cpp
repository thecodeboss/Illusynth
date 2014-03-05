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

XAudio2SourceVoice* XAudio2Device::CreateSourceVoice(SourceVoiceType type)
{
	// @ILLUSYNTH_TODO: Need to create custom allocators
	return new XAudio2SourceVoice(type);
}

bool XAudio2Device::PlaySourceVoice( XAudio2SourceVoice* source )
{
	// @ILLUSYNTH_TODO
	XAudioStreamContext* s = new XAudioStreamContext(this, source);

	DWORD dwThreadId = 0;
	HANDLE StreamingVoiceThread = CreateThread( NULL, 0, StreamProc, s, 0, &dwThreadId );
	if( StreamingVoiceThread == NULL )
	{
		return false;
	}

	return true;
}

DWORD WINAPI XAudio2Device::StreamThreadMain( XAudio2SourceVoice* source )
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	source->Init();

	source->Start();
	source->Stop();
	source->Cleanup();

	CoUninitialize();

	return 0;
}

#endif // _WINDOWS