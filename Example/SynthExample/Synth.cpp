#include "pch.h"
#include <Synth.h>

Synth::Synth() : m_Volume(1.0f), m_FilterCutoff(0.5f), m_WaveType(W_SAW)
{

}

int Synth::Run()
{
	// Get the audio device for making sound
	m_Audio = AudioDevice::GetInstance();
	if (m_Audio == nullptr) return -1;

	// Initialize the audio device, quitting execution on failure
	if (!m_Audio->Init()) return -1;

	// A simple 16 note melody
#define NOTE_REST 0.0f
	float Melody[] = { NOTE_A2S, NOTE_F4, NOTE_D3, NOTE_C5,
		NOTE_A3S, NOTE_G3, NOTE_F2S, NOTE_G3S,
		NOTE_A4S, NOTE_A2, NOTE_C4, NOTE_D4,
		NOTE_F3, NOTE_D3, NOTE_F2, NOTE_D3S };

#define SLIDE	0x1
#define ACCENT	0x2
	int Properties[] = { 0, 0, SLIDE, 0,
		SLIDE | ACCENT, 0, SLIDE | ACCENT, 0,
		SLIDE, 0, SLIDE, SLIDE | ACCENT,
		0, SLIDE, SLIDE, SLIDE };

	// Set some playback parameters
	float BPM = 111.0f;
	float DurationRatio = 0.75f;

	// Calculate based on our parameters
	float TotalEighthDuration = 60.0f / (BPM * 4.0f);
	float EighthNoteDuration = TotalEighthDuration * DurationRatio;
	float EighthPauseDuration = TotalEighthDuration * (1.0f - DurationRatio);

	AudioSource* MySoundSource = m_Audio->CreateSoundSource(S_PROCEDURAL, ILLUSYNTH_FX_REVERB);
	MySoundSource->SetReverbSettings(0.1f, 0.1f);
	int count = 0;
	while (1)
	{
		int i = count % (sizeof(Melody) / sizeof(float));
		float Duration = (Properties[i] & SLIDE) ? TotalEighthDuration : EighthNoteDuration;
		float Amplitude = (Properties[i] & ACCENT) ? m_Volume : m_Volume*0.5f;
		if (Melody[i]) MySoundSource->AddProcedural(m_WaveType, Waveform(Melody[i], Amplitude, Duration, count*TotalEighthDuration));

		MySoundSource->SetFilterCutoff(0, m_FilterCutoff);

		if (!MySoundSource->IsPlaying()) m_Audio->PlaySource(MySoundSource);

		// Infinite loop until playback needs more samples
		while (MySoundSource->GetNumProcedural() > 2)
		{
			if (!MySoundSource->IsPlaying())
			{
				break;
			}
		}

		count++;
	}

	return 0;
}

void Synth::SetVolume(int arg)
{
	m_Volume = (arg + 50) / 100.0f;
}

void Synth::SetFilterCutoff(int arg)
{
	m_FilterCutoff = (arg + 50) / 100.0f;
}

void Synth::ToggleWaveType()
{
	m_WaveType = m_WaveType == W_SAW ? W_SQUARE : W_SAW;
}
