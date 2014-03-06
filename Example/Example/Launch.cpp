#include <Illusynth.h>

int main()
{
	AudioDevice* Audio = AudioDevice::GetInstance();//GetIllusynthInstance();
	if (Audio == nullptr) return -1;

	bool result = Audio->Init();

	AudioSource* MySoundSource = Audio->CreateSoundSource(S_WAVE);

	return 0;
}
