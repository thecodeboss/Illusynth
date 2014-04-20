//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "Synth.h"

namespace SynthExample
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
		Synth m_Synth;
	public:
		MainPage();

		void OnVolumeChanged(int e);
		void OnFilterCutoffChanged(int e);
	private:
		void m_bWaveType_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
