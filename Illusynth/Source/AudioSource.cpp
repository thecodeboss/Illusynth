#include <Private\AudioSource.h>
#include <Private\Debug.h>

AudioSource::AudioSource(AudioSourceType type) : m_Type(type)
{
}

bool AudioSource::SetReverbSettings(float Diffusion, float RoomSize)
{
	m_ReverbSettings.Diffusion = Diffusion;
	m_ReverbSettings.RoomSize = RoomSize;
	return true;
}

bool AudioSource::SetFilterCutoff(int FilterHandle, float Cutoff)
{
	return true;
}

bool AudioSource::AddProcedural(WaveformType type, Waveform waveform)
{
	CheckWarning(m_Type == S_PROCEDURAL, TEXT("Attempted to add a prodedural waveform to a non-procedural source."));
	return (m_Type == S_PROCEDURAL);
}
