#include <Illusynth.h>

int main()
{
	AudioDevice* Audio = AudioDevice::GetInstance();//GetIllusynthInstance();
	if (Audio != nullptr)
	{
		return 0;
	}
	else return -1;
}
