#include <Private\XAudio2SourceVoice.h>
#include <Private\XAudio2Device.h>

#if _WINDOWS

XAudio2SourceVoice::XAudio2SourceVoice(AudioSourceType type)
	: m_Source(nullptr), m_wfx(nullptr), m_bPlaying(false), m_bInitialized(false), AudioSource(type)
{

}

bool XAudio2SourceVoice::Init()
{
	HRESULT hr = XAudio2Device::XAudio2->CreateSourceVoice(&m_Source, (WAVEFORMATEX*)m_wfx, 0, 2.0f, &m_Callback);
	if (hr < 0)
	{
		return false;
	}

	UINT32 NumEffects = 0;

	// Create the reverb effect
	IUnknown* ReverbXAPO = nullptr;
	bool bReverbEnabled = (m_EffectFlags & ILLUSYNTH_FX_REVERB) ? true : false;
	if (CreateFX(__uuidof(FXReverb), &ReverbXAPO) < 0)
	{
		return false;
	}

	// Create the EQ effect
	IUnknown* EQ_XAPO = nullptr;
	bool bEQEnabled = (m_EffectFlags & ILLUSYNTH_FX_EQ) ? true : false;
	if (CreateFX(__uuidof(FXEQ), &EQ_XAPO) < 0)
	{
		return false;
	}

	// NOTE: The order of these effects must match the order in the
	// header file
	XAUDIO2_EFFECT_DESCRIPTOR EffectDescriptors[] = {
		{ ReverbXAPO, bReverbEnabled, 1 },
		{ EQ_XAPO, bEQEnabled, 1 }
	};

	XAUDIO2_EFFECT_CHAIN EffectChain;
	EffectChain.EffectCount = sizeof(EffectDescriptors) / sizeof(XAUDIO2_EFFECT_DESCRIPTOR);
	EffectChain.pEffectDescriptors = EffectDescriptors;

	m_Source->SetEffectChain(&EffectChain);

	m_bInitialized = true;

	ReverbXAPO->Release();
	EQ_XAPO->Release();

	return true;
}

bool XAudio2SourceVoice::Start()
{
	// @ILLUSYNTH_TODO
	m_bPlaying = true;
	return true;
}

bool XAudio2SourceVoice::Pause()
{
	// @ILLUSYNTH_TODO
	m_bPlaying = false;
	return true;
}

bool XAudio2SourceVoice::Stop()
{
	// @ILLUSYNTH_TODO
	m_bPlaying = false;
	return (m_Source->Stop() == S_OK);
}

bool XAudio2SourceVoice::IsPlaying()
{
	return m_bPlaying;
}

bool XAudio2SourceVoice::Cleanup()
{
	// @ILLUSYNTH_TODO
	m_Source->FlushSourceBuffers();
	m_Source->DestroyVoice();

	return S_OK;
}

bool XAudio2SourceVoice::SetReverbSettings(float Diffusion, float RoomSize)
{
	AudioSource::SetReverbSettings(Diffusion, RoomSize);

	FXREVERB_PARAMETERS ReverbParameters;
	ReverbParameters.Diffusion = m_ReverbSettings.Diffusion;
	ReverbParameters.RoomSize = m_ReverbSettings.RoomSize;

	if (m_Source->SetEffectParameters(0, &ReverbParameters, sizeof(ReverbParameters)) < 0)
	{
		return false;
	}
	return true;
}

#endif // _WINDOWS