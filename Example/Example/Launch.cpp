#include <Illusynth.h>
#include <Pitches.h>

int main()
{
	AudioDevice* Audio = AudioDevice::GetInstance();
	if (Audio == nullptr) return -1;

#define NOTE_REST 0.0f
	float Melody[] = { NOTE_G2, NOTE_G2, NOTE_G2, NOTE_G2,
		NOTE_REST, NOTE_G2, NOTE_G2, NOTE_REST,
		NOTE_A2S, NOTE_REST, NOTE_A2S, NOTE_A2S,
		NOTE_A2S, NOTE_A2S, NOTE_REST, NOTE_A2S };
	float Duration = 0.07f;
	float Delay = 0.045f;

	bool result = Audio->Init();

	AudioSource* OldSource = nullptr;
	while (1)
	{
		AudioSource* MySoundSource = Audio->CreateSoundSource(S_PROCEDURAL);
		for (auto i = 0; i < sizeof(Melody) / sizeof(float); i++)
		{
			if (Melody[i]) MySoundSource->AddProcedural(W_SQUARE, Waveform(Melody[i], 0.3f, Duration, i*(Duration + Delay)) );
		}

		Audio->PlaySource(MySoundSource);

		while (MySoundSource->IsPlaying());

		if (OldSource != nullptr) delete OldSource;

		OldSource = MySoundSource;
	}

	return 0;
}
