#pragma once

#include <Illusynth.h>
#include <Pitches.h>

class Synth
{
	AudioDevice* m_Audio;
	float m_Volume;
	float m_FilterCutoff;
	WaveformType m_WaveType;
public:
	Synth();

	int Run();
	void SetVolume(int arg);
	void SetFilterCutoff(int arg);
	void ToggleWaveType();
};
