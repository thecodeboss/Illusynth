#include <Private\AudioSource.h>
#include <Private\Debug.h>

int AudioSource::AudioSourceCounter = 0;

AudioSource::AudioSource(AudioSourceType type) : m_Type(type), m_FilterCutoff(0.0f)
{
	m_SourceID = AudioSourceCounter++;
	m_Name = "Source_" + std::to_string(m_SourceID);
}

AudioSource::AudioSource(std::string name, AudioSourceType type) : m_Name(name), m_Type(type)
{
	m_SourceID = AudioSourceCounter++;
}

bool AudioSource::SetReverbSettings(float Diffusion, float RoomSize)
{
	m_ReverbSettings.Diffusion = Diffusion;
	m_ReverbSettings.RoomSize = RoomSize;
	return true;
}

bool AudioSource::SetFilterCutoff(int FilterHandle, float Cutoff)
{
	m_FilterCutoff = (Cutoff > 0.999f) ? 0.999f : Cutoff;
	return true;
}

Oscillator* AudioSource::AddOscillator(OscillatorType type, float frequency)
{
	CheckWarning(m_Type == S_PROCEDURAL, TEXT("Attempted to add an oscillator to a non-procedural source."));
	if (m_Type != S_PROCEDURAL) return nullptr;

	Oscillator* oscillator = new Oscillator(type, frequency);
	m_Oscillators.push_back(oscillator);

	return oscillator;
}

std::string AudioSource::GetName() const
{
	return m_Name;
}

float fsel(float a, float b, float c) {
	return a >= 0 ? b : c;
}

inline float clamp(float a, float min, float max)
{
	a = fsel(a - min, a, min);
	return fsel(a - max, max, a);
}

bool AudioSource::ComputeProceduralBuffer(char * OutputLocation, unsigned BufferSize, unsigned BitsPerSample)
{
	unsigned Increment = BitsPerSample / 8;
	for (unsigned i = 0; i < BufferSize; i += Increment)
	{
		float sample = 0.0f;

		for (size_t j = 0; j < m_Oscillators.size(); j++)
		{
			sample += m_Oscillators[j]->Sample();
		}

		// This is a hard clamp: try to make sure it doesn't get used often!
		sample = clamp(sample, -1.0f, 1.0f);

		// HACKY LOW PASS FILTER
		static float n[4] = { 0 };

		float cut_lp = m_FilterCutoff; // cutoff frequency of the lowpass(0..1)
		float cut_hp = m_FilterCutoff; // cutoff frequency of the hipass(0..1)
		static float res_lp = 0.8f; // resonance of the lowpass(0..1)
		static float res_hp = 0.5f; // resonance of the hipass(0..1)

		static float fb_lp = 0.0f;
		static float fb_hp = 0.0f;

		fb_lp = res_lp + res_lp / (1 - cut_lp);
		fb_hp = res_hp + res_hp / (1 - cut_lp);

		n[1] = n[1] + cut_lp*(sample - n[1] + fb_lp*(n[1] - n[2]));// +p4;
		n[2] = n[2] + cut_lp*(n[1] - n[2]);
		n[3] = n[3] + cut_hp*(n[2] - n[3] + fb_hp*(n[3] - n[4]));// +p4;
		n[4] = n[4] + cut_hp*(n[3] - n[4]);

		sample -= n[4];

		short temp = 0;
		switch (BitsPerSample)
		{
		case 8:
			OutputLocation[i] = static_cast<char>(sample * SCHAR_MAX);
			break;
		case 16:
			temp = static_cast<short>(sample * SHRT_MAX);
			*(short*)&OutputLocation[i] = static_cast<short>(sample * SHRT_MAX);
			break;
		default:
			OutputLocation[i] = 0;
			break;
		}
	}
	return true;
}
