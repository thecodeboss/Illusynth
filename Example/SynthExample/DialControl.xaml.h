//
// DialControl.xaml.h
// Declaration of the DialControl class
//

#pragma once

#include "DialControl.g.h"

namespace SynthExample
{
	public delegate void DialControlEventHandler(int e);

	[Windows::UI::Xaml::Data::Bindable]
	public ref class DialControl sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		DialControl();

		event DialControlEventHandler^ OnAmountChanged;
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

		property float Angle
		{
			float get();
			void set(float value);
		};

		property int Amount
		{
			int get();
			void set(int value);
		};

	private:

		void OnPropertyChanged(Platform::String^ propertyName);
		void Grid_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e);
		float GetAngle(Windows::Foundation::Point TouchPoint, Windows::Foundation::Size CircleSize);

		float m_Angle;
		int m_Amount;
	};

	public ref class MultiplyByValueConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		MultiplyByValueConverter();
		virtual ~MultiplyByValueConverter();
		virtual Object^ Convert(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, Platform::String^ language);
		virtual Object^ ConvertBack(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, Platform::String^ language);
	};
}
