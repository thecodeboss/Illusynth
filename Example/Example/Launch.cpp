#include <Illusynth.h>
#include <Pitches.h>

int main()
{
	AudioDevice* Audio = AudioDevice::GetInstance();//GetIllusynthInstance();
	if (Audio == nullptr) return -1;

	float Rhythm[] = {	NOTE_C3, NOTE_C3, NOTE_G3, NOTE_C3, 
						NOTE_C3, NOTE_E3, NOTE_C3, NOTE_C3,
						NOTE_D3, NOTE_C3, NOTE_C3, NOTE_A2,
						NOTE_C3, NOTE_B2, NOTE_B2, NOTE_C3};
	float Duration = 0.1f;
	float Delay = 0.05f;

	bool result = Audio->Init();

	AudioSource* OldSource = nullptr;
	while (1)
	{
		AudioSource* MySoundSource = Audio->CreateSoundSource(S_PROCEDURAL);
		for (auto i = 0; i < sizeof(Rhythm) / sizeof(float); i++)
		{
			MySoundSource->AddProcedural(W_SQUARE, Waveform(Rhythm[i], 0.3f, Duration, i*(Duration + Delay)) );
		}

		Audio->PlaySource(MySoundSource);

		while (MySoundSource->IsPlaying());

		if (OldSource != nullptr) delete OldSource;

		OldSource = MySoundSource;
	}

	return 0;
}
