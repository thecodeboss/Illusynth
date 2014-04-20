#ifndef Illusynth_h__
#define Illusynth_h__

/************************************************************************/
/* Global defines                                                       */
/************************************************************************/

#define DEFAULT_PROCEDURAL_NUM_CHANNELS 1
#define DEFAULT_PROCEDURAL_SAMPLE_RATE 48000
#define DEFAULT_PROCEDURAL_BITS_PER_SAMPLE 16

/************************************************************************/
/* DLL Exports Setup                                                    */
/************************************************************************/

#ifdef _WINDOWS
#ifdef _WINDLL
	#define ILLUSYNTH_API __declspec(dllexport)
#else
	#define ILLUSYNTH_API __declspec(dllimport)
#endif // _WINDLL
#else
	#define ILLUSYNTH_API extern
#endif // _WINDOWS

#ifndef DLL_SPECS_ONLY
/************************************************************************/
/* Primary Abstract Audio Device                                        */
/************************************************************************/

#include <Private\Oscillator.h>
#include <Private\AudioEffects.h>
#include <Private\AudioSource.h>
#include <Private\AudioDevice.h>

#endif // DLL_SPECS_ONLY
#endif // Illusynth_h__
