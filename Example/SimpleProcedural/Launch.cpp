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
	float Melody[] = {	NOTE_G2, NOTE_G2, NOTE_G2, NOTE_G2,
						NOTE_REST, NOTE_G2, NOTE_G2, NOTE_REST,
						NOTE_A2S, NOTE_REST, NOTE_A2S, NOTE_A2S,
						NOTE_A2S, NOTE_A2S, NOTE_REST, NOTE_A2S };

	// Set some playback parameters
	float Duration = 0.07f;
	float Delay = 0.045f;

	// Create a sound source with an oscillator (and track)
	AudioSource* MySoundSource = Audio->CreateSoundSource(S_PROCEDURAL);
	Oscillator* MainOscillator = MySoundSource->AddOscillator(O_SAW);
	OscillatorTrack* MainTrack = new OscillatorTrack();
	MainOscillator->SetOscillatorTrack(MainTrack);

	while (1)
	{
		for (auto i = 0; i < sizeof(Melody) / sizeof(float); i++)
		{
			MainTrack->AddFrame(Melody[i], 1.0f, Duration);
			MainTrack->AddFrame(0.0f, 0.0f, Delay);
		}

		if (!MySoundSource->IsPlaying()) Audio->PlaySource(MySoundSource);

		// Loop until the track needs more data
		while (MainTrack->GetSize() > 1);
	}

	return 0;
}
