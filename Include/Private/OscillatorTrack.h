#ifndef OscillatorTrack_h__
#define OscillatorTrack_h__

#define DLL_SPECS_ONLY
#include <Illusynth.h>
#undef DLL_SPECS_ONLY

#include <vector>

struct OscillatorFrame
{
	float Frequency;
	float Amplitude;
};

class OscillatorTrack
{
	size_t m_Size;
	unsigned m_SampleRate;
	std::vector<float> m_Frequencies;
	std::vector<float> m_Amplitudes;
	std::vector<unsigned long long> m_SampleDurations;
	inline unsigned long long DurationToSamples(float d)
	{
		return static_cast<unsigned long long>(d * m_SampleRate);
	}

public:
	ILLUSYNTH_API OscillatorTrack();
	ILLUSYNTH_API void AddFrame(float frequency, float amplitude, float duration);
	OscillatorFrame Sample();
	ILLUSYNTH_API size_t GetSize() const;
};

#endif // OscillatorTrack_h__
