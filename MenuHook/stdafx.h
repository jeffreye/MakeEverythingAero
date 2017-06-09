// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <process.h>
#include <time.h>
#include <vector>
#include <algorithm>

#define Log
#define AERO_ID (0xAE130)

const DWORD WCA_ACCENT_POLICY = 19;

typedef enum {
	ACCENT_DISABLED = 0,
	ACCENT_ENABLE_GRADIENT = 1,
	ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
	ACCENT_ENABLE_BLURBEHIND = 3,
	ACCENT_INVALID_STATE = 4,
	_ACCENT_STATE_SIZE = 0xFFFFFFFF
} ACCENT_STATE;

typedef struct {
	ACCENT_STATE accentState;
	int accentFlags;
	int gradientColor;
	int invalidState;
} DWMACCENTPOLICY;

typedef struct _WINCOMPATTR_DATA {
	DWMACCENTPOLICY AccentPolicy;
} WINCOMPATTR_DATA;

typedef struct tagWINCOMPATTR
{
	DWORD attribute; // the attribute to query
	WINCOMPATTR_DATA *pData; // buffer to store the result
	ULONG dataSize; // size of the pData buffer
} WINCOMPATTR;

typedef enum _DWMNCRENDERINGPOLICY {
	DWMNCRP_USEWINDOWSTYLE,
	DWMNCRP_DISABLED,
	DWMNCRP_ENABLED,
	DWMNCRP_LAST
} DWMNCRENDERINGPOLICY;

typedef enum _DWMWINDOWATTRIBUTE {
	DWMWA_NCRENDERING_ENABLED = 1,
	DWMWA_NCRENDERING_POLICY,          // 2
	DWMWA_TRANSITIONS_FORCEDISABLED,   // 3
	DWMWA_ALLOW_NCPAINT,               // 4
	DWMWA_CAPTION_BUTTON_BOUNDS,       // 5
	DWMWA_NONCLIENT_RTL_LAYOUT,        // 6
	DWMWA_FORCE_ICONIC_REPRESENTATION, // 7
	DWMWA_FLIP3D_POLICY,               // 8
	DWMWA_EXTENDED_FRAME_BOUNDS,       // 9
	DWMWA_HAS_ICONIC_BITMAP,           // 10
	DWMWA_DISALLOW_PEEK,               // 11
	DWMWA_EXCLUDED_FROM_PEEK,          // 12
	DWMWA_CLOAK,                       // 13
	DWMWA_CLOAKED,                     // 14
	DWMWA_FREEZE_REPRESENTATION,       // 15
	DWMWA_LAST                         // 16
} DWMWINDOWATTRIBUTE;



void ReadConfig();
void FreeResources();

// TODO: reference additional headers your program requires here
