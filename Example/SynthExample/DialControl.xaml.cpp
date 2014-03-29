//
// DialControl.xaml.cpp
// Implementation of the DialControl class
//

#include "pch.h"
#include "DialControl.xaml.h"
#include <cmath>
#include <string>

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

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

DialControl::DialControl()
{
	InitializeComponent();
	DataContext = this;
	Angle = 0.0f;
	Amount = 0;
}


void SynthExample::DialControl::Grid_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e)
{
	Angle = GetAngle(e->Position, RenderSize);
	if (Angle > 180.0f && Angle < 240.0f) Angle = -120.0f;
	else if (Angle > 120.0f && Angle < 240.0f) Angle = 120.0f;
	else if (Angle > 240.0f) Angle -= 360.0f;

	Amount = static_cast<int>((Angle / 240.0f) * 100.0f);
}

void SynthExample::DialControl::OnPropertyChanged(Platform::String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}

float SynthExample::DialControl::GetAngle(Point TouchPoint, Size CircleSize)
{
	float x = TouchPoint.X - (CircleSize.Width / 2.0f);
	float y = CircleSize.Height - TouchPoint.Y - (CircleSize.Height / 2.0f);
	
	float AngleInRadians = atan(x/y);

	return 180.0f * AngleInRadians / 3.1415927f + (y < 0.0f ? 180.0f : 0.0f);
}

float SynthExample::DialControl::Angle::get()
{
	return m_Angle;
}

void SynthExample::DialControl::Angle::set(float value)
{
	if (m_Angle != value)
	{
		m_Angle = value;
		OnPropertyChanged("Angle");
	}
}

int SynthExample::DialControl::Amount::get()
{
	return m_Amount;
}

void SynthExample::DialControl::Amount::set(int value)
{
	if (m_Amount != value)
	{
		m_Amount = value;
		OnPropertyChanged("Amount");
		OnAmountChanged(value);
	}
}

SynthExample::MultiplyByValueConverter::MultiplyByValueConverter()
{

}

SynthExample::MultiplyByValueConverter::~MultiplyByValueConverter()
{

}

Object^ SynthExample::MultiplyByValueConverter::Convert(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, String^ language)
{
	float _value_float;
	float _param_float;

	float result = 1.0f;

	Platform::String^ _param_str = parameter->ToString();
	unsigned param_len = _param_str->Length();
	_param_float = static_cast<float>(_wtof(_param_str->Data()));
	result *= _param_float;

	Platform::String^ _value_str = value->ToString();
	unsigned value_len = _value_str->Length();
	_value_float = static_cast<float>(_wtof(_value_str->Data()));
	result *= _value_float;

	if (_param_str->Data()[param_len - 1] == 'f')
	{
		return result;
	}
	else
	{
		return static_cast<int>(result).ToString();
	}
}

Object^ SynthExample::MultiplyByValueConverter::ConvertBack(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, String^ language)
{
	return value;
}
