#pragma once

#include "Resource.h"
#include "targetver.h"

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

// Header Files
#include <windows.h>
#include <cmath>
#include <shellapi.h>
#include <strsafe.h>
#include <tchar.h>

// Macros
// Hotkey IDs
#define ID_HK_DEC_WND_SZ    1
#define ID_HK_INC_WND_SZ    2
#define ID_HK_CEN_WND       3
#define ID_HK_ALT_CEN_WND_1 4
#define ID_HK_ALT_CEN_WND_2 5
#define ID_HK_ALT_CEN_WND_3 6

// Modifiers
#define MOD_WND_SIZE 100

// Tray Icon IDs
#define ID_TRAY_ICON             200
#define ID_TRAY_ICON_SMENU_TITLE 201
#define ID_TRAY_ICON_SMENU_EXIT  202

// Messages
#define WM_TRAYICON 500

// Constants
#define WND_CHANGE_SIZE     50

#define APP_TITLE           _T("ReSize")
#define MAIN_WND_CLASS_NAME _T("MainWnd")

// Program status return values
#define SUCCESS       0

#define BAD_HWND      400
#define BAD_KEY       401
#define BAD_RECT_READ 402

// Global variables
GUID GUID_NULL = {
    0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};

HMENU trayIconMenu = {};

NOTIFYICONDATA trayIconData = {};

HANDLE icon = LoadImage(GetModuleHandle(NULL),
                        MAKEINTRESOURCE(IDI_SMALL),
                        IMAGE_ICON,
                        0,
                        0,
                        LR_DEFAULTSIZE | LR_SHARED);

// User input variables
LONG defWidthPer  = 80;
LONG defHeightPer = 90;
