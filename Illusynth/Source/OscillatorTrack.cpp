#include <Private\OscillatorTrack.h>
#include <Private\Oscillator.h>

OscillatorTrack::OscillatorTrack()
: m_SampleRate(OSCILLATOR_DEFAULT_SAMPLING_RATE), m_Size(0)
{
}

void OscillatorTrack::AddFrame(float frequency, float amplitude, float duration)
{
	m_Frequencies.push_back(frequency);
	m_Amplitudes.push_back(amplitude);
	m_SampleDurations.push_back(DurationToSamples(duration));
	m_Size++;
}

OscillatorFrame OscillatorTrack::Sample()
{
	OscillatorFrame frame;
	if (m_Size > 0)
	{
		frame.Frequency = m_Frequencies[0];
		frame.Amplitude = m_Amplitudes[0];
		if (--m_SampleDurations[0] == 0)
		{
			m_Frequencies.erase(m_Frequencies.begin());
			m_Amplitudes.erase(m_Amplitudes.begin());
			m_SampleDurations.erase(m_SampleDurations.begin());
			m_Size--;
		}
	}
	else
	{
		frame.Frequency = OSCILLATOR_DEFAULT_FREQUENCY;
		frame.Amplitude = 0.0f;
	}
	return frame;
}

size_t OscillatorTrack::GetSize() const
{
	return m_Size;
}
