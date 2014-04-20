#include <cmath>
#define MATH_PI 3.1415927f

#include <Private\Oscillator.h>
#include <Private\Debug.h>

Oscillator::Oscillator(OscillatorType type, float frequency, size_t table_size)
	: m_TableSize(table_size), m_PhaseIndex(0.0f), m_Amplitude(1.0f), m_SamplingRate(OSCILLATOR_DEFAULT_SAMPLING_RATE), m_SamplesPassed(0), m_OscillatorTrack(nullptr)
{
	m_Table.resize(table_size);
	SetFrequency(frequency);
	SetType(type);
}

void Oscillator::SetOscillatorTrack(OscillatorTrack* osc)
{
	m_OscillatorTrack = osc;
}

float Oscillator::Sample()
{
	if (m_OscillatorTrack != nullptr)
	{
		OscillatorFrame frame = m_OscillatorTrack->Sample();
		SetFrequency(frame.Frequency);
		SetAmplitude(frame.Amplitude);
	}
	m_PhaseIndex = fmod(m_PhaseIndex + m_SamplingIncrement, static_cast<float>(m_TableSize));
	int iPhaseIndex = static_cast<int>(floor(m_PhaseIndex));
	m_SamplesPassed++;
	return m_Amplitude * m_Table[iPhaseIndex];
}

void Oscillator::SetFrequency(float f)
{
	m_Frequency = f;
	m_SamplingIncrement = (m_TableSize * f) / m_SamplingRate;
}

void Oscillator::SetAmplitude(float a)
{
	m_Amplitude = a;
}

void Oscillator::SetType(OscillatorType type)
{
	m_Type = type;
	switch (type)
	{
	case O_SINE:
		for (size_t i = 0; i < m_TableSize; i++)
		{
			float fIndex = static_cast<float>(i);
			float fTableSize = static_cast<float>(m_TableSize);
			m_Table[i] = sin(2.0f * MATH_PI * fIndex / fTableSize);
		}
		break;
	case O_SQUARE:
		for (size_t i = 0; i < m_TableSize; i++)
		{
			m_Table[i] = (i <= m_TableSize / 2) ? 1.0f : -1.0f;
		}
		break;
	case O_SAW:
		for (size_t i = 0; i < m_TableSize; i++)
		{
			float fIndex = static_cast<float>(i);
			float fTableSize = static_cast<float>(m_TableSize);
			m_Table[i] = (2.0f / fTableSize) * fIndex - 1.0f;
		}
		break;
	case O_NOISE:
		for (size_t i = 0; i < m_TableSize; i++)
		{
			m_Table[i] = 2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX)-1.0f;
		}
		break;
	default:
		ConsolePrintf(TEXT("Oscillator error: unknown type %d specified"), type);
		break;
	}
}

OscillatorType Oscillator::GetType() const
{
	return m_Type;
}
