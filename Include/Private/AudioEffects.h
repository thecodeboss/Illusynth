#ifndef AudioEffects_h__
#define AudioEffects_h__

#define DLL_SPECS_ONLY
#include <Illusynth.h>
#undef DLL_SPECS_ONLY

#define ILLUSYNTH_FX_NONE		0x0
#define ILLUSYNTH_FX_REVERB		0x1
#define ILLUSYNTH_FX_EQ			0x2

#define ILLUSYNTH_DEFAULT_REVERB_DIFFUSION 0.9f
#define ILLUSYNTH_DEFAULT_REVERB_ROOMSIZE 0.6f

struct ReverbSettings
{
	float Diffusion;
	float RoomSize;
	ReverbSettings() : Diffusion(ILLUSYNTH_DEFAULT_REVERB_DIFFUSION), RoomSize(ILLUSYNTH_DEFAULT_REVERB_ROOMSIZE) {}
};

#endif // AudioEffects_h__
