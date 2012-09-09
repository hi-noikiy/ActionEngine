#pragma once

#include <msclr\marshal_cppstd.h>
#include "EngineMain.h"
#include "ACInputDeviceW.h"
#include "ACMouseW.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace EngineWrapper
{
	public ref class ACInputW
	{
		private:
			ACInput* pInput;
			ACInputDeviceW^ _inputDevice;
			ACMouseW^ _mouse;
		public:
			ACInputW(IntPtr hInst);
			HRESULT CreateDevice(String^ inputLibraryName);
			ACInputDeviceW^ GetDevice();

			void CreateMouse();
			ACMouseW^ GetMouse();
	};
}