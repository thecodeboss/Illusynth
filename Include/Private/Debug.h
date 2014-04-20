#ifndef Debug_h__
#define Debug_h__

#if _WINDOWS
#include <windows.h>
#include <string>

// Supress C4793: function compiled as native, varargs not supported under / clr
#pragma warning( disable : 4793 )

const int IllusynthCheckErrorException = -10;

// Outputs to the Visual Studio console
static void DebugConsolePrintf(LPTSTR fmt, ...)
{
	va_list vl;
	char szBuf[256];

	va_start(vl, fmt);
	vsprintf_s(szBuf, fmt, vl);
	va_end(vl);

	OutputDebugString(szBuf);
	OutputDebugString(TEXT("\r\n"));
}

// Prints a warning to the console if a condition fails
static bool _CheckWarning(bool Condition, LPTSTR fmt, ...)
{
	if (!Condition)
	{
		va_list vl;
		va_start(vl, fmt);
		DebugConsolePrintf(fmt, vl);
		va_end(vl);
		return false;
	}
	return true;
}

// Prints a warning to the console if a condition fails,
// but only if running a debug configuration
static bool _CheckDebug(bool Condition, LPTSTR fmt, ...)
{
#if _DEBUG
	if (!Condition)
	{
		va_list vl;
		va_start(vl, fmt);
		DebugConsolePrintf(fmt, vl);
		va_end(vl);
		return false;
	}
#endif
	return true;
}

// Prints a warning to the console if a condition fails,
// and halts program execution
static void _CheckError(bool Condition, LPTSTR fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	_CheckWarning(Condition, fmt, vl);
	va_end(vl);
	if (!Condition)
	{
		throw IllusynthCheckErrorException;
	}
}

#define CheckWarning(cond, fmt, ...) _CheckWarning(cond, TEXT("%s(%i): " fmt), __FILE__, __LINE__, __VA_ARGS__)
#define CheckDebug(cond, fmt, ...) _CheckDebug(cond, TEXT("%s(%i): " fmt), __FILE__, __LINE__, __VA_ARGS__)
#define CheckError(cond, fmt, ...) _CheckError(cond, TEXT("%s(%i): " fmt), __FILE__, __LINE__, __VA_ARGS__)

#endif // _WINDOWS
#endif // Debug_h__
