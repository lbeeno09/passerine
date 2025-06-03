#pragma once

#ifdef PS_PLATFORM_WINDOWS
#ifdef PS_BUILD_DLL
#define PASSER_API __declspec(dllexport)
#else
#define PASSER_API __declspec(dllimport)
#endif
#else
#error Passer Support Windows Only
#endif