#include <Private\XAudio2ProceduralSourceVoice.h>
#include <Private\Debug.h>

#if _WINDOWS

/************************************************************************/
/* XAudio2ProceduralSourceVoice Implementation                          */
/************************************************************************/

XAudio2ProceduralSourceVoice::XAudio2ProceduralSourceVoice()
: XAudio2SourceVoice(S_PROCEDURAL), m_FilterCutoff(0.5f)
{

}


bool XAudio2ProceduralSourceVoice::Init()
{
	m_Mutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	m_wfx = new WAVEFORMATEXTENSIBLE();
	m_wfx->Format.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx->Format.nChannels = DEFAULT_PROCEDURAL_NUM_CHANNELS;
	m_wfx->Format.nSamplesPerSec = DEFAULT_PROCEDURAL_SAMPLE_RATE;
	m_wfx->Format.wBitsPerSample = DEFAULT_PROCEDURAL_BITS_PER_SAMPLE;
	m_wfx->Format.nBlockAlign = m_wfx->Format.nChannels * m_wfx->Format.wBitsPerSample / 8;
	m_wfx->Format.nAvgBytesPerSec = m_wfx->Format.nSamplesPerSec * m_wfx->Format.nBlockAlign;
	m_wfx->Format.cbSize = 0;
	m_Buffers = new BYTE[MAX_BUFFER_COUNT*PROCEDURAL_BUFFER_SIZE];
	return XAudio2SourceVoice::Init();
}

bool XAudio2ProceduralSourceVoice::Start()
{
	if (!XAudio2SourceVoice::Start()) return false;

	if (m_Source == nullptr) return false;

	if (!XAudio2CheckedCall(m_Source->Start(0, 0)))
	{
		return false;
	}

	unsigned CurrentDiskReadBuffer = 0;
	unsigned CurrentPosition = 0;
	unsigned BufferSize = PROCEDURAL_BUFFER_SIZE;
	while (m_bPlaying)
	{
		WaitForSingleObject(m_Mutex, INFINITE);
		ComputeActiveSounds(CurrentPosition, BufferSize);
		ComputeProceduralBuffer((BYTE *)&m_Buffers[CurrentDiskReadBuffer*BufferSize], BufferSize);
		ReleaseMutex(m_Mutex);

		// Check with XAudio to see if there are any buffers available, and wait if needed
		XAUDIO2_VOICE_STATE state;
		while (m_Source->GetState(&state), state.BuffersQueued >= MAX_BUFFER_COUNT - 1)
		{
			WaitForSingleObject(m_Callback.hBufferEndEvent, INFINITE);
		}

		// Pointer the XAudio buffer at our buffer array
		XAUDIO2_BUFFER buf = { 0 };
		buf.AudioBytes = BufferSize;
		buf.pAudioData = &m_Buffers[CurrentDiskReadBuffer*BufferSize];

		// Submit buffers
		if (!XAudio2CheckedCall(m_Source->SubmitSourceBuffer(&buf)))
		{
			ConsolePrintf(TEXT("XAudio2: Failed to submit source buffers."));
			break;
		}

		CurrentDiskReadBuffer++;
		CurrentDiskReadBuffer %= MAX_BUFFER_COUNT;
		CurrentPosition++;

		//// @ILLUSYNTH_TODO: Implement a 'IsFinished' function
		//if (ActiveSquareWaves.size() == 0 && SquareWaves.size() == 0 &&
		//	ActiveSawWaves.size() == 0 && SawWaves.size() == 0) break;
	}

	// Wait for everything to finish
	XAUDIO2_VOICE_STATE state;
	while (m_Source->GetState(&state), state.BuffersQueued > 0)
	{
		WaitForSingleObjectEx(m_Callback.hBufferEndEvent, INFINITE, TRUE);
	}

	DebugConsolePrintf(TEXT("Finished playing procedural source voice (%s)"), m_Name);

	return S_OK;
}

float fsel(float a, float b, float c) {
	return a >= 0 ? b : c;
}

inline float clamp(float a, float min, float max)
{
	a = fsel(a - min, a, min);
	return fsel(a - max, max, a);
}

bool XAudio2ProceduralSourceVoice::ComputeProceduralBuffer(BYTE * OutputLocation, unsigned BufferSize)
{
	unsigned Increment = m_wfx->Format.wBitsPerSample / 8;
	float SampleRate = static_cast<float>(m_wfx->Format.nSamplesPerSec);
	float SamplePeriod = 1.0f / SampleRate;
	for (unsigned i = 0; i < BufferSize; i += Increment)
	{
		float sample = 0.0f;
		float Position = static_cast<float>((i / Increment)) / SampleRate;

		ComputeSquareWaves(sample, Position, i, Increment, SampleRate, SamplePeriod, BufferSize);
		ComputeSineWaves(sample, Position, i, Increment, SampleRate, SamplePeriod, BufferSize);
		ComputeSawWaves(sample, Position, i, Increment, SampleRate, SamplePeriod, BufferSize);
		ComputeNoise(sample, SamplePeriod);

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
		switch (m_wfx->Format.wBitsPerSample)
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

void XAudio2ProceduralSourceVoice::ComputeSquareWaves(float& sample, float Position, unsigned Index, unsigned Increment, float SampleRate, float SamplePeriod, unsigned BufferSize)
{
	for (auto it = ActiveSquareWaves.begin(); it != ActiveSquareWaves.end(); it++)
	{
		Waveform* s = *it;
		if (s->Delay > 0.0f)
		{
			s->Delay -= SamplePeriod;
			continue;
		}
		if (s->Duration < 0.0f) continue;

		float SquareWidth = 0.5f / s->Frequency;
		if (fabs(fmod(floor(Position / SquareWidth + s->Offset), 2.0f)) <= 0.0001f) sample += s->Amplitude;
		else sample -= s->Amplitude;

		s->Duration -= SamplePeriod;
		if (Index + Increment == BufferSize) s->Offset = fmod(Position / SquareWidth + s->Offset, 2.0f);
	}
	sample = clamp(sample, -1.0f, 1.0f);
}

void XAudio2ProceduralSourceVoice::ComputeSineWaves(float& sample, float Position, unsigned Index, unsigned Increment, float SampleRate, float SamplePeriod, unsigned BufferSize)
{
	for (auto it = ActiveSineWaves.begin(); it != ActiveSineWaves.end(); it++)
	{
		Waveform* s = *it;
		if (s->Delay > 0.0f)
		{
			s->Delay -= SamplePeriod;
			continue;
		}
		if (s->Duration < 0.0f)
		{
			continue;
		}

		sample += s->Amplitude * sin(4 * static_cast<float>(M_PI_2)* s->Frequency * (Position + s->Offset));

		s->Duration -= SamplePeriod;
		if (Index + Increment == BufferSize) s->Offset += Position;
	}
	sample = clamp(sample, -1.0f, 1.0f);
}

void XAudio2ProceduralSourceVoice::ComputeSawWaves(float& sample, float Position, unsigned Index, unsigned Increment, float SampleRate, float SamplePeriod, unsigned BufferSize)
{
	for (auto it = ActiveSawWaves.begin(); it != ActiveSawWaves.end(); it++)
	{
		Waveform* s = *it;
		if (s->Delay > 0.0f)
		{
			s->Delay -= SamplePeriod;
			continue;
		}
		if (s->Duration < 0.0f) continue;

		float Period = 1.0f / s->Frequency;
		sample += s->Amplitude * fmod(Position + s->Offset, Period) / Period;

		s->Duration -= SamplePeriod;
		if (Index + Increment == BufferSize) s->Offset += Position;
	}
	sample = clamp(sample, -1.0f, 1.0f);
}

void XAudio2ProceduralSourceVoice::ComputeNoise(float& sample, float SamplePeriod)
{
	for (auto it = ActiveNoiseGenerators.begin(); it != ActiveNoiseGenerators.end(); it++)
	{
		Noise* n = *it;
		if (n->Delay > 0.0f)
		{
			n->Delay -= SamplePeriod;
			continue;
		}
		if (n->Duration < 0.0f) continue;

		sample += n->Amplitude * (2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX)-1.0f);

		n->Duration -= SamplePeriod;
	}
	sample = clamp(sample, -1.0f, 1.0f);
}

template<typename T>
void ProcessActives(std::vector<T*>& NotActives, std::vector<T*>& Actives, float StartTime, float EndTime)
{
	// Figure out which sounds will play during the current buffer
	for (int i = static_cast<int>(NotActives.size()) - 1; i >= 0; i--)
	{
		T* s = NotActives[i];
		if (EndTime >= s->Delay)
		{
			s->Delay -= StartTime;
			Actives.push_back(s);
			NotActives.erase(NotActives.begin() + i);
		}
	}

	// Remove sounds that have finished playing
	for (int i = static_cast<int>(Actives.size()) - 1; i >= 0; i--)
	{
		T* s = Actives[i];
		if (s->Duration < 0.0f)
		{
			Actives.erase(Actives.begin() + i);
			delete s;
		}
	}
}

void XAudio2ProceduralSourceVoice::ComputeActiveSounds(unsigned CurrentBuffer, unsigned SamplesPerBuffer)
{
	float StartofBufferTime = static_cast<float>(CurrentBuffer)*static_cast<float>(SamplesPerBuffer) / static_cast<float>(m_wfx->Format.nSamplesPerSec) / (m_wfx->Format.wBitsPerSample / 8.0f);
	float EndofBufferTime = static_cast<float>(CurrentBuffer + 1)*static_cast<float>(SamplesPerBuffer) / static_cast<float>(m_wfx->Format.nSamplesPerSec) / (m_wfx->Format.wBitsPerSample / 8.0f);

	ProcessActives(SquareWaves, ActiveSquareWaves, StartofBufferTime, EndofBufferTime);
	ProcessActives(SineWaves, ActiveSineWaves, StartofBufferTime, EndofBufferTime);
	ProcessActives(SawWaves, ActiveSawWaves, StartofBufferTime, EndofBufferTime);
	ProcessActives(NoiseGenerators, ActiveNoiseGenerators, StartofBufferTime, EndofBufferTime);
}

bool XAudio2ProceduralSourceVoice::AddProcedural(WaveformType type, Waveform waveform)
{
	WaitForSingleObject(m_Mutex, INFINITE);
	Waveform* NewWaveform = new Waveform(waveform);
	switch (type)
	{
	case W_SQUARE:
		SquareWaves.push_back(NewWaveform);
		break;
	case W_SAW:
		SawWaves.push_back(NewWaveform);
		break;
	default:
		delete NewWaveform;
		ReleaseMutex(m_Mutex);
		return false;
		break;
	}
	ReleaseMutex(m_Mutex);
	return true;
}

size_t XAudio2ProceduralSourceVoice::GetNumProcedural()
{
	WaitForSingleObject(m_Mutex, INFINITE);
	size_t NumSines = ActiveSineWaves.size() + SineWaves.size();
	size_t NumSaws = ActiveSawWaves.size() + SawWaves.size();
	size_t NumSquares = ActiveSquareWaves.size() + SquareWaves.size();
	ReleaseMutex(m_Mutex);
	return NumSines + NumSaws + NumSquares;
}

bool XAudio2ProceduralSourceVoice::SetFilterCutoff(int FilterHandle, float Cutoff)
{
	if (!AudioSource::SetFilterCutoff(FilterHandle, Cutoff))
	{
		return false;
	}

	m_FilterCutoff = (Cutoff > 0.999f) ? 0.999f : Cutoff;
	return true;
}

#endif // _WINDOWS