#ifndef AudioEffects_h__
#define AudioEffects_h__

#define DLL_SPECS_ONLY
#include <Illusynth.h>
#undef DLL_SPECS_ONLY

#define ILLUSYNTH_FX_NONE		0x0
#define ILLUSYNTH_FX_REVERB		0x1
#define ILLUSYNTH_FX_EQ			0x2

struct ReverbSettings
{
	float Diffusion;
	float RoomSize;
	ReverbSettings() : Diffusion(0.9f), RoomSize(0.6f) {}
};

#endif // AudioEffects_h__
