#include <Private\XAudio2SourceVoice.h>
#include <Private\XAudio2Device.h>

#if _WINDOWS

XAudio2SourceVoice::XAudio2SourceVoice(AudioSourceType type)
	: m_Source(nullptr), m_wfx(nullptr), m_Type(type), m_bPlaying(false)
{

}

bool XAudio2SourceVoice::Init()
{
	HRESULT hr = XAudio2Device::XAudio2->CreateSourceVoice(&m_Source, (WAVEFORMATEX*)m_wfx, 0, 2.0f, &m_Callback);
	if (hr < 0)
	{
		return false;
	}

	return true;
}

bool XAudio2SourceVoice::Start()
{
	// @ILLUSYNTH_TODO
	m_bPlaying = true;
	return true;
}

bool XAudio2SourceVoice::Pause()
{
	// @ILLUSYNTH_TODO
	m_bPlaying = false;
	return true;
}

bool XAudio2SourceVoice::Stop()
{
	// @ILLUSYNTH_TODO
	m_bPlaying = false;
	return (m_Source->Stop() == S_OK);
}

bool XAudio2SourceVoice::IsPlaying()
{
	return m_bPlaying;
}

bool XAudio2SourceVoice::Cleanup()
{
	// @ILLUSYNTH_TODO
	m_Source->FlushSourceBuffers();
	m_Source->DestroyVoice();

	return S_OK;
}

/************************************************************************/
/* XAudio2ProceduralSourceVoice Implementation                          */
/************************************************************************/

XAudio2ProceduralSourceVoice::XAudio2ProceduralSourceVoice()
	: XAudio2SourceVoice(S_PROCEDURAL)
{

}


bool XAudio2ProceduralSourceVoice::Init()
{
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

	HRESULT hr = m_Source->Start(0, 0);
	if (hr < 0) return false;

	unsigned CurrentDiskReadBuffer = 0;
	unsigned CurrentPosition = 0;
	unsigned BufferSize = PROCEDURAL_BUFFER_SIZE;
	while ( CurrentPosition < PROCEDURAL_BUFFER_SIZE )
	{
		ComputeActiveSounds(CurrentPosition, BufferSize);
		ComputeProceduralBuffer((BYTE *)&m_Buffers[CurrentDiskReadBuffer*BufferSize], BufferSize);

		// Check with XAudio to see if there are any buffers available, and wait if needed
		XAUDIO2_VOICE_STATE state;
		while(m_Source->GetState( &state ), state.BuffersQueued >= MAX_BUFFER_COUNT - 1)
		{
			WaitForSingleObject( m_Callback.hBufferEndEvent, INFINITE );
		}

		// Pointer the XAudio buffer at our buffer array
		XAUDIO2_BUFFER buf = {0};
		buf.AudioBytes = BufferSize;
		buf.pAudioData = &m_Buffers[CurrentDiskReadBuffer*BufferSize];

		// Submit buffers
		hr = m_Source->SubmitSourceBuffer( &buf );
		if (hr < 0)
		{
			return false;
		}

		CurrentDiskReadBuffer++;
		CurrentDiskReadBuffer %= MAX_BUFFER_COUNT;
		CurrentPosition++;

		// @ILLUSYNTH_TODO: Implement a 'IsFinished' function
		if (ActiveSquareWaves.size() == 0 && SquareWaves.size() == 0) break;
	}

	// Wait for everything to finish
	XAUDIO2_VOICE_STATE state;
	while( m_Source->GetState( &state ), state.BuffersQueued > 0 )
	{
		WaitForSingleObjectEx( m_Callback.hBufferEndEvent, INFINITE, TRUE );
	}

	return S_OK;
}

float fsel( float a, float b, float c ) {
	return a >= 0 ? b : c;
}

inline float clamp ( float a, float min, float max ) 
{
	a = fsel( a - min , a, min );
	return fsel( a - max, max, a );
}

bool XAudio2ProceduralSourceVoice::ComputeProceduralBuffer(BYTE * OutputLocation, unsigned BufferSize)
{
	unsigned Increment = m_wfx->Format.wBitsPerSample / 8;
	float SampleRate = static_cast<float>(m_wfx->Format.nSamplesPerSec);
	float SamplePeriod = 1.0f / SampleRate;
	for (unsigned i=0; i < BufferSize; i+=Increment)
	{
		float sample = 0.0f;
		float Position = static_cast<float>((i / Increment)) / SampleRate;

		ComputeSquareWaves(sample, Position, i, Increment, SampleRate, SamplePeriod, BufferSize);
		ComputeSineWaves(sample, Position, i, Increment, SampleRate, SamplePeriod, BufferSize);
		ComputeSawWaves(sample, Position, i, Increment, SampleRate, SamplePeriod, BufferSize);
		ComputeNoise(sample, SamplePeriod);

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
	for (auto it=ActiveSquareWaves.begin(); it != ActiveSquareWaves.end(); it++)
	{
		Waveform* s = *it;
		if (s->Delay > 0.0f)
		{
			s->Delay -= SamplePeriod;
			continue;
		}
		if (s->Duration < 0.0f) continue;

		float SquareWidth = 0.5f / s->Frequency;
		if (fabs(fmod(floor(Position / SquareWidth + s->Offset), 2.0f) ) <= 0.0001f) sample += s->Amplitude;
		else sample -= s->Amplitude;

		s->Duration -= SamplePeriod;
		if (Index + Increment == BufferSize) s->Offset = fmod(Position / SquareWidth + s->Offset, 2.0f);
	}
	sample = clamp(sample, -1.0f, 1.0f);
}

void XAudio2ProceduralSourceVoice::ComputeSineWaves( float& sample, float Position, unsigned Index, unsigned Increment, float SampleRate, float SamplePeriod, unsigned BufferSize )
{
	for (auto it=ActiveSineWaves.begin(); it != ActiveSineWaves.end(); it++)
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

		sample += s->Amplitude * sin(4 * static_cast<float>(M_PI_2) * s->Frequency * (Position + s->Offset));

		s->Duration -= SamplePeriod;
		if (Index + Increment == BufferSize) s->Offset += Position;
	}
	sample = clamp(sample, -1.0f, 1.0f);
}

void XAudio2ProceduralSourceVoice::ComputeSawWaves( float& sample, float Position, unsigned Index, unsigned Increment, float SampleRate, float SamplePeriod, unsigned BufferSize )
{
	for (auto it=ActiveSawWaves.begin(); it != ActiveSawWaves.end(); it++)
	{
		Waveform* s = *it;
		if (s->Delay > 0.0f)
		{
			s->Delay -= SamplePeriod;
			continue;
		}
		if (s->Duration < 0.0f) continue;

		float Period = 1.0f / s->Frequency;
		sample += fmod(Position + s->Offset, Period) / Period;

		s->Duration -= SamplePeriod;
		if (Index + Increment == BufferSize) s->Offset += Position;
	}
	sample = clamp(sample, -1.0f, 1.0f);
}

void XAudio2ProceduralSourceVoice::ComputeNoise( float& sample, float SamplePeriod )
{
	for (auto it=ActiveNoiseGenerators.begin(); it != ActiveNoiseGenerators.end(); it++)
	{
		Noise* n = *it;
		if (n->Delay > 0.0f)
		{
			n->Delay -= SamplePeriod;
			continue;
		}
		if (n->Duration < 0.0f) continue;

		sample += n->Amplitude * (2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0f);

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

void XAudio2ProceduralSourceVoice::ComputeActiveSounds( unsigned CurrentBuffer, unsigned SamplesPerBuffer )
{
	float StartofBufferTime = static_cast<float>(CurrentBuffer)*static_cast<float>(SamplesPerBuffer) / static_cast<float>(m_wfx->Format.nSamplesPerSec) / (m_wfx->Format.wBitsPerSample / 8.0f);
	float EndofBufferTime = static_cast<float>(CurrentBuffer+1)*static_cast<float>(SamplesPerBuffer) / static_cast<float>(m_wfx->Format.nSamplesPerSec) / (m_wfx->Format.wBitsPerSample / 8.0f);
	ProcessActives(SquareWaves, ActiveSquareWaves, StartofBufferTime, EndofBufferTime);
	ProcessActives(SineWaves, ActiveSineWaves, StartofBufferTime, EndofBufferTime);
	ProcessActives(SawWaves, ActiveSawWaves, StartofBufferTime, EndofBufferTime);
	ProcessActives(NoiseGenerators, ActiveNoiseGenerators, StartofBufferTime, EndofBufferTime);
	//for (size_t i = SquareWaves.size() - 1; i >= 0; i--)
	//{
	//	Waveform* s = SquareWaves[i];
	//	if (EndofBufferTime >= s->Delay)
	//	{
	//		ActiveSquareWaves.push_back(s);
	//		SquareWaves.erase(SquareWaves.begin() + i);
	//	}
	//}
}

bool XAudio2ProceduralSourceVoice::AddProcedural(WaveformType type, Waveform waveform)
{
	Waveform* NewWaveform = new Waveform(waveform);
	switch (type)
	{
	case W_SQUARE:
		SquareWaves.push_back(NewWaveform);
		break;
	default:
		delete NewWaveform;
		return false;
		break;
	}
	return true;
}

#endif // _WINDOWS