/************************************************************************/
/* Illusynth Example: Complex Procedural                                */
/*                                                                      */
/* This example demonstrates a more advanced procedural sound generator */
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
	float Melody[] = {	NOTE_A2S, NOTE_F4, NOTE_D3, NOTE_C5,
						NOTE_A3S, NOTE_G3, NOTE_F2S, NOTE_G3S,
						NOTE_A4S, NOTE_A2, NOTE_C4, NOTE_D4,
						NOTE_F3, NOTE_D3, NOTE_F2, NOTE_D3S };

#define SLIDE	0x1
#define ACCENT	0x2
	int Properties[] = {	0, 0, SLIDE, 0,
							SLIDE | ACCENT, 0, SLIDE | ACCENT, 0,
							SLIDE, 0, SLIDE, SLIDE | ACCENT,
							0, SLIDE, SLIDE, SLIDE };

	// Set some playback parameters
	float BPM = 111.0f;
	float DurationRatio = 0.75f;

	// Calculate based on our parameters
	float TotalEighthDuration = 60.0f / (BPM * 4.0f);
	float EighthNoteDuration = TotalEighthDuration * DurationRatio;
	float EighthPauseDuration = TotalEighthDuration * (1.0f - DurationRatio);

	// Create a sound source with an oscillator (and track)
	AudioSource* MySoundSource = Audio->CreateSoundSource(S_PROCEDURAL);
	Oscillator* MainOscillator = MySoundSource->AddOscillator(O_SAW);
	OscillatorTrack* MainTrack = new OscillatorTrack();
	MainOscillator->SetOscillatorTrack(MainTrack);

	int count = 0;
	while (1)
	{
		int i = count % (sizeof(Melody) / sizeof(float));
		float Amplitude = (Properties[i] & ACCENT) ? 1.0f : 0.5f;
		if (Properties[i] & SLIDE)
		{
			MainTrack->AddFrame(Melody[i], Amplitude, TotalEighthDuration);
		}
		else
		{
			MainTrack->AddFrame(Melody[i], Amplitude, EighthNoteDuration);
			MainTrack->AddFrame(Melody[i], 0.0f, TotalEighthDuration - EighthNoteDuration);
		}

		if (!MySoundSource->IsPlaying()) Audio->PlaySource(MySoundSource);

		// Infinite loop until playback needs more info
		while (MainTrack->GetSize() > 1)
		{
			if (!MySoundSource->IsPlaying())
			{
				break;
			}
		}

		if (!MySoundSource->IsPlaying())
		{
			break;
		}

		count++;
	}

	return 0;
}
