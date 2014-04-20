#include <Private\XAudio2ProceduralSourceVoice.h>
#include <Private\Debug.h>

#if _WINDOWS

/************************************************************************/
/* XAudio2ProceduralSourceVoice Implementation                          */
/************************************************************************/

XAudio2ProceduralSourceVoice::XAudio2ProceduralSourceVoice()
: XAudio2SourceVoice(S_PROCEDURAL)
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
		ComputeProceduralBuffer((char *)&m_Buffers[CurrentDiskReadBuffer*BufferSize], BufferSize, m_wfx->Format.wBitsPerSample);
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

#endif // _WINDOWS