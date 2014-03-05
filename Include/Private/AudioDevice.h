#ifndef AudioDevice_h__
#define AudioDevice_h__

class AudioDevice
{
public:
	virtual bool Init() = 0;
	virtual bool Cleanup() = 0;
};

#endif // AudioDevice_h__
