#include "..\..\include\Debug\Debug.h"

using namespace WyvernsAssault;

void Debug::Out(Ogre::String text)
{
	Debug::Out(text.c_str());
}

void Debug::Out(const char *text)
{
	::OutputDebugStringA(text);
}

void Debug::Out(const wchar_t *text)
{
	::OutputDebugStringW(text);
}