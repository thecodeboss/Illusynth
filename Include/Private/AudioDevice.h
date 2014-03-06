#ifndef AudioDevice_h__
#define AudioDevice_h__

#define DLL_SPECS_ONLY
#include <Illusynth.h>
#undef DLL_SPECS_ONLY

class AudioDevice
{
	static AudioDevice* m_Instance;
public:
	static ILLUSYNTH_API AudioDevice* GetInstance();
	virtual ILLUSYNTH_API bool Init() = 0;
	virtual ILLUSYNTH_API bool Cleanup() = 0;
};

#endif // AudioDevice_h__
