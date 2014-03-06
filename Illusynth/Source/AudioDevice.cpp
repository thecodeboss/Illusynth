#include <Private\AudioDevice.h>
#include <Private\XAudio2Device.h>

AudioDevice* AudioDevice::m_Instance = nullptr;

AudioDevice* AudioDevice::GetInstance()
{
	if (m_Instance == nullptr)
	{
#if _WINDOWS
		m_Instance = (AudioDevice*)new XAudio2Device();
#else
		return nullptr;
#endif
	}
	return m_Instance;
}
