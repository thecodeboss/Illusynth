#ifndef AudioDevice_h__
#define AudioDevice_h__

#define DLL_SPECS_ONLY
#include <Illusynth.h>
#undef DLL_SPECS_ONLY

#include <Private\AudioEffects.h>
#include <Private\AudioSource.h>

class AudioDevice
{
	static AudioDevice* m_Instance;
public:
	static ILLUSYNTH_API AudioDevice* GetInstance();
	virtual ILLUSYNTH_API bool Init() = 0;
	virtual ILLUSYNTH_API bool Cleanup() = 0;
	virtual ILLUSYNTH_API AudioSource* CreateSoundSource(AudioSourceType type, int EffectFlags = ILLUSYNTH_FX_NONE) = 0;
	virtual ILLUSYNTH_API bool PlaySource(AudioSource* Source) = 0;
};

#endif // AudioDevice_h__
