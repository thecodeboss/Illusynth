//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "DialControl.xaml.h"
#include "MainPage.xaml.h"

using namespace SynthExample;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
	m_VolumeControl->OnAmountChanged +=ref new SynthExample::DialControlEventHandler(this, &SynthExample::MainPage::OnVolumeChanged);
	m_FilterControl->OnAmountChanged +=ref new SynthExample::DialControlEventHandler(this, &SynthExample::MainPage::OnFilterCutoffChanged);

	Concurrency::create_async([this]
	{
		return m_Synth.Run();
	});
}


void SynthExample::MainPage::OnVolumeChanged(int e)
{
	m_Synth.SetVolume(e);
}


void SynthExample::MainPage::OnFilterCutoffChanged(int e)
{
	m_Synth.SetFilterCutoff(e);
}

void SynthExample::MainPage::m_bWaveType_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_Synth.ToggleWaveType();
}
