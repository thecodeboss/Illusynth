#ifndef XAudio2ProceduralSourceVoice_h__
#define XAudio2ProceduralSourceVoice_h__

#include <Private\XAudio2SourceVoice.h>

class XAudio2ProceduralSourceVoice : public XAudio2SourceVoice
{
	HANDLE m_Mutex;
	float m_FilterCutoff;

	std::vector<Waveform*> ActiveSquareWaves;
	std::vector<Waveform*> ActiveSineWaves;
	std::vector<Waveform*> ActiveSawWaves;
	std::vector<Noise*> ActiveNoiseGenerators;
public:

	std::vector<Waveform*> SquareWaves;
	std::vector<Waveform*> SineWaves;
	std::vector<Waveform*> SawWaves;
	std::vector<Noise*> NoiseGenerators;

	XAudio2ProceduralSourceVoice();
	virtual bool AddProcedural(WaveformType type, Waveform waveform);
	virtual size_t GetNumProcedural();
	virtual bool SetFilterCutoff(int FilterHandle, float Cutoff);

	virtual bool Init();
	virtual bool Start();
	bool ComputeProceduralBuffer(BYTE * OutputLocation, unsigned BufferSize);
	void ComputeSquareWaves(float& sample, float Position, unsigned Index, unsigned Increment, float SampleRate, float SamplePeriod, unsigned BufferSize);
	void ComputeSineWaves(float& sample, float Position, unsigned Index, unsigned Increment, float SampleRate, float SamplePeriod, unsigned BufferSize);
	void ComputeSawWaves(float& sample, float Position, unsigned Index, unsigned Increment, float SampleRate, float SamplePeriod, unsigned BufferSize);
	void ComputeNoise(float& sample, float SamplePeriod);
	void ComputeActiveSounds(unsigned CurrentBuffer, unsigned SamplesPerBuffer);
};

#endif // XAudio2ProceduralSourceVoice_h__
