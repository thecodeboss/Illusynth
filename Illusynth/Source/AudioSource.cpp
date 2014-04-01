#include <Private\AudioSource.h>

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
