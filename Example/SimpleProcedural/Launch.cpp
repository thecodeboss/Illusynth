/************************************************************************/
/* Illusynth Example: Simple Procedural                                 */
/*                                                                      */
/* This example demonstrates a simple looping melody or square waves.   */
/************************************************************************/
#include <Illusynth.h>
#include <Pitches.h>

int main()
{
	// Get the audio device for making sound
	AudioDevice* Audio = AudioDevice::GetInstance();
	if (Audio == nullptr) return -1;

	// Initialize the audio device, quitting execution on failure
	if (!Audio->Init()) return -1;

	// A simple 16 note melody
#define NOTE_REST 0.0f
	float Melody[] = {	NOTE_G2, NOTE_G2, NOTE_G2, NOTE_G2,
						NOTE_REST, NOTE_G2, NOTE_G2, NOTE_REST,
						NOTE_A2S, NOTE_REST, NOTE_A2S, NOTE_A2S,
						NOTE_A2S, NOTE_A2S, NOTE_REST, NOTE_A2S };

	// Set some playback parameters
	float Duration = 0.07f;
	float Delay = 0.045f;

	// We loop continuously, creating new sound sources and deleting the old ones
	AudioSource* OldSource = nullptr;
	while (1)
	{
		AudioSource* MySoundSource = Audio->CreateSoundSource(S_PROCEDURAL);
		for (auto i = 0; i < sizeof(Melody) / sizeof(float); i++)
		{
			if (Melody[i]) MySoundSource->AddProcedural(W_SQUARE, Waveform(Melody[i], 0.3f, Duration, i*(Duration + Delay)) );
		}

		Audio->PlaySource(MySoundSource);

		// Infinite loop until sound is complete
		while (MySoundSource->IsPlaying());

		if (OldSource != nullptr) delete OldSource;
		OldSource = MySoundSource;
	}

	return 0;
}
