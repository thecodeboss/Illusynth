#ifndef XAudio2ProceduralSourceVoice_h__
#define XAudio2ProceduralSourceVoice_h__

#include <Private\XAudio2SourceVoice.h>

class XAudio2ProceduralSourceVoice : public XAudio2SourceVoice
{
	HANDLE m_Mutex;
public:

	XAudio2ProceduralSourceVoice();

	virtual bool Init();
	virtual bool Start();
};

#endif // XAudio2ProceduralSourceVoice_h__
