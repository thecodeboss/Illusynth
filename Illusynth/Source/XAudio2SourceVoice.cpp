#include <Private\XAudio2SourceVoice.h>
#include <Private\XAudio2Device.h>

#if _WINDOWS

XAudio2SourceVoice::XAudio2SourceVoice(SourceVoiceType type)
	: m_Source(nullptr), m_wfx(nullptr), m_Type(type)
{

}

HRESULT XAudio2SourceVoice::Init()
{
	HRESULT hr = XAudio2Device::XAudio2->CreateSourceVoice(&m_Source, (WAVEFORMATEX*)m_wfx, 0, 2.0f, m_Callback);
	if (hr < 0)
	{
		return false;
	}
	return true;
}

HRESULT XAudio2SourceVoice::Start()
{
	// @ILLUSYNTH_TODO
	return S_OK;
}

HRESULT XAudio2SourceVoice::Pause()
{
	// @ILLUSYNTH_TODO
	return S_OK;
}

HRESULT XAudio2SourceVoice::Stop()
{
	// @ILLUSYNTH_TODO
	return m_Source->Stop();
}

HRESULT XAudio2SourceVoice::Cleanup()
{
	// @ILLUSYNTH_TODO
	m_Source->FlushSourceBuffers();
	m_Source->DestroyVoice();
	return S_OK;
}

#endif // _WINDOWS