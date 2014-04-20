#ifndef Oscillator_h__
#define Oscillator_h__

#define DLL_SPECS_ONLY
#include <Illusynth.h>
#undef DLL_SPECS_ONLY

#include <Private\OscillatorTrack.h>
#include <vector>

#define OSCILLATOR_DEFAULT_TABLE_SIZE 2048
#define OSCILLATOR_DEFAULT_FREQUENCY 220.0f
#define OSCILLATOR_DEFAULT_SAMPLING_RATE DEFAULT_PROCEDURAL_SAMPLE_RATE

enum OscillatorType
{
	O_SINE,
	O_SQUARE,
	O_SAW,
	O_NOISE
};

class Oscillator
{
	unsigned long long m_SamplesPassed;
	std::vector<float> m_Table;
	size_t m_TableSize;
	float m_Amplitude;
	float m_Frequency;
	float m_SamplingIncrement;
	float m_SamplingRate;
	float m_PhaseIndex;
	OscillatorTrack* m_OscillatorTrack;
	OscillatorType m_Type;
public:
	ILLUSYNTH_API Oscillator(OscillatorType type, float frequency = OSCILLATOR_DEFAULT_FREQUENCY, size_t table_size = OSCILLATOR_DEFAULT_TABLE_SIZE);
	ILLUSYNTH_API void SetOscillatorTrack(OscillatorTrack* osc);
	float Sample();
	ILLUSYNTH_API void SetFrequency(float f);
	ILLUSYNTH_API void SetAmplitude(float a);
	ILLUSYNTH_API void SetType(OscillatorType type);
	ILLUSYNTH_API OscillatorType GetType() const;
};

#endif // Oscillator_h__
