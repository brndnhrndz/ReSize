#pragma once

#include "framework.h"

/* ********** FORWARD DECLARATIONS ********** */

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

// MESSAGE HANDLERS

void HandleCloseMessage(HWND);
void HandleCommandMessage(HWND, WPARAM, LPARAM);
void HandleCreateMessage(HWND, LPARAM);
void HandleDestroyMessage();
void HandleHotKeyMessage(HWND, WPARAM, LPARAM);
void HandlePaintMessage(HWND);
void HandleTrayIconMessage(HWND, WPARAM, LPARAM);

//  HELPER FUNCTIONS

LONG CenterWindow(HWND,
                  LONG flag      = NULL,
                  LONG widthPer  = defWidthPer,
                  LONG heightPer = defHeightPer);
void CreateTrayIcon(HWND);
void CreateTrayIconMenu();
BOOL IsDesktopWindow(HWND);
BOOL IsMainWindow(HWND);
void ShowTrayIconMenu(HWND);
LONG ResizeWindow(HWND, LONG, LONG);
BOOL TrayIconExists(HWND);

/* ********** FUNCTION DEFINITIONS ********** */

LRESULT CALLBACK WindowProc(HWND   hWnd,
                            UINT   uMsg,
                            WPARAM wParam,
                            LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            HandleCreateMessage(hWnd, lParam);
            break;
        }
        case WM_PAINT: {
            HandlePaintMessage(hWnd);
            break;
        }
        case WM_CLOSE: {
            HandleCloseMessage(hWnd);
            break;
        }
        case WM_DESTROY: {
            HandleDestroyMessage();
            break;
        }
        case WM_TRAYICON: {
            HandleTrayIconMessage(hWnd, wParam, lParam);
            break;
        }
        case WM_COMMAND: {
            HandleCommandMessage(hWnd, wParam, lParam);
            break;
        }
        case WM_HOTKEY: {
            HandleHotKeyMessage(hWnd, wParam, lParam);
            break;
        }
        default: {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
    return SUCCESS;
}

// MESSAGE HANDLERS

void HandleCloseMessage(HWND hWnd) {
    ShowWindow(hWnd, SW_HIDE);
}

void HandleCommandMessage(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    switch (LOWORD(wParam)) {
        case ID_TRAY_ICON_SMENU_TITLE: {
            ShowWindow(hWnd, SW_SHOW);
            break;
        }
        case ID_TRAY_ICON_SMENU_EXIT: {
            Shell_NotifyIcon(NIM_DELETE, &trayIconData);
            DestroyWindow(hWnd);
            break;
        }
        default: {
            break;
        }
    }
}

void HandleCreateMessage(HWND hWnd, LPARAM lParam) {
    if (!TrayIconExists(hWnd)) {
        CreateTrayIcon(hWnd);
        CreateTrayIconMenu();
    }
}

void HandleDestroyMessage() {
    PostQuitMessage(0);
}

void HandleHotKeyMessage(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    HWND fgWnd = GetForegroundWindow();

    if ((IsMainWindow(fgWnd) && wParam != ID_HK_CEN_WND) ||
        (IsDesktopWindow(fgWnd))) {
        return;
    }

    switch (wParam) {
        case ID_HK_DEC_WND_SZ: {
            ResizeWindow(fgWnd, -WND_CHANGE_SIZE, -WND_CHANGE_SIZE);
            break;
        }
        case ID_HK_INC_WND_SZ: {
            ResizeWindow(fgWnd, WND_CHANGE_SIZE, WND_CHANGE_SIZE);
            break;
        }
        case ID_HK_CEN_WND: {
            CenterWindow(fgWnd);
            break;
        }
        case ID_HK_ALT_CEN_WND_1: {
            CenterWindow(fgWnd, MOD_WND_SIZE, 20, 30);
            break;
        }
        case ID_HK_ALT_CEN_WND_2: {
            CenterWindow(fgWnd, MOD_WND_SIZE, 34, 50);
            break;
        }
        case ID_HK_ALT_CEN_WND_3: {
            CenterWindow(fgWnd, MOD_WND_SIZE);
            break;
        }
        default: {
            break;
        }
    }
}

// TODO [L]: Paint GUI
void HandlePaintMessage(HWND hWnd) {
    PAINTSTRUCT ps = {};

    HDC hdc = BeginPaint(hWnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
    EndPaint(hWnd, &ps);
}

void HandleTrayIconMessage(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    switch (lParam) {
        case WM_RBUTTONUP: {
            ShowTrayIconMenu(hWnd);
            break;
        }
        case WM_LBUTTONUP: {
            ShowWindow(hWnd, SW_SHOW);
            break;
        }
        default: {
            break;
        }
    }
}

//  HELPER FUNCTIONS

LONG CenterWindow(HWND hWnd,
                  LONG flag,
                  LONG widthPer,
                  LONG heightPer) {
    if (hWnd == NULL) {
        return BAD_HWND;
    }

    // Get window dimensions
    RECT rect = {};

    if (GetWindowRect(hWnd, &rect)) {
        LONG wndWidth, wndHeight;
        LONG workAreaWidth, workAreaHeight;
        LONG leftEdgeXCoord, topEdgeYCoord;

        // Get work area dimensions
        RECT workArea = {};

        SystemParametersInfo(SPI_GETWORKAREA, NULL, &workArea, NULL);

        workAreaWidth  = workArea.right - workArea.left;
        workAreaHeight = workArea.bottom - workArea.top;

        // Compute centered window dimensions
        if (flag == MOD_WND_SIZE) {
            wndWidth  = (LONG) std::ceil(workAreaWidth * (widthPer / 100.0));
            wndHeight = (LONG) std::ceil(workAreaHeight * (heightPer / 100.0));
        } else {
            wndWidth  = rect.right - rect.left;
            wndHeight = rect.bottom - rect.top;
        }

        // Compute edge coordinates used to center window
        leftEdgeXCoord = (workAreaWidth - wndWidth) / 2;
        topEdgeYCoord  = (workAreaHeight - wndHeight) / 2;

        // Undo window maximization if necessary
        if (IsZoomed(hWnd) && flag == MOD_WND_SIZE) {
            ShowWindow(hWnd, SW_RESTORE);
        }

        // Reposition window
        MoveWindow(hWnd,
                   leftEdgeXCoord,
                   topEdgeYCoord,
                   wndWidth,
                   wndHeight,
                   TRUE);

        return SUCCESS;
    } else {
        return BAD_RECT_READ;
    }
}

void CreateTrayIcon(HWND hWnd) {
    // Set NOTIFYICONDATA properties
    SecureZeroMemory(&trayIconData, sizeof(NOTIFYICONDATA));

    trayIconData.cbSize           = sizeof(NOTIFYICONDATA);
    trayIconData.hWnd             = hWnd;
    trayIconData.uID              = ID_TRAY_ICON;
    trayIconData.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    trayIconData.uCallbackMessage = WM_TRAYICON;
    trayIconData.hIcon            = (HICON) icon;

    StringCchCopy(trayIconData.szTip,
                  ARRAYSIZE(trayIconData.szTip),
                  APP_TITLE);

    // Place icon in system tray
    Shell_NotifyIcon(NIM_ADD, &trayIconData);
}

// Is it better to have a menu created before WM_CREATE is processed?
void CreateTrayIconMenu() {
    trayIconMenu = CreatePopupMenu();
    AppendMenu(trayIconMenu,
               MF_STRING,
               ID_TRAY_ICON_SMENU_TITLE,
               APP_TITLE);
    AppendMenu(trayIconMenu,
               MF_STRING,
               ID_TRAY_ICON_SMENU_EXIT,
               _T("Exit"));
}

BOOL IsDesktopWindow(HWND hWnd) {
    TCHAR className[256];

    GetClassName(hWnd,
                 className,
                 (sizeof(className) / sizeof(TCHAR)));

    // FALSE if window refered to by hWnd has class name
    // that is neither Progman and WorkerW
    return !_tcscmp(className, _T("Progman")) ||
           !_tcscmp(className, _T("WorkerW"));
}

BOOL IsMainWindow(HWND hWnd) {
    // TRUE if hWnd is a handle to mainWnd created in wWinMain
    return hWnd == FindWindow(MAIN_WND_CLASS_NAME, APP_TITLE);
}

void ShowTrayIconMenu(HWND hWnd) {
    // Get cursor coordinates
    POINT p = {};

    GetCursorPos(&p);

    // Show context menu
    SetForegroundWindow(hWnd);
    TrackPopupMenuEx(trayIconMenu,
                     TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON,
                     p.x,
                     p.y,
                     hWnd,
                     NULL);
}

LONG ResizeWindow(HWND hWnd, LONG widthChange, LONG heightChange) {
    if (hWnd == NULL) {
        return BAD_HWND;
    }

    // Get window dimensions
    RECT rect = {};

    if (GetWindowRect(hWnd, &rect)) {
        LONG newWndWidth, newWndHeight;
        LONG leftEdgeXCoord, topEdgeYCoord;
        LONG minWidth, minHeight;

        // Compute new window dimensions with change factor taken into account
        newWndWidth  = (rect.right - rect.left) + widthChange;
        newWndHeight = (rect.bottom - rect.top) + heightChange;

        // Get work area dimensions
        RECT workArea = {};

        SystemParametersInfo(SPI_GETWORKAREA, NULL, &workArea, NULL);

        // Compute minimum window dimensions as a percentage of work area
        // dimensions
        minWidth  = (LONG) std::ceil((workArea.right - workArea.left) * 0.2);
        minHeight = (LONG) std::ceil((workArea.bottom - workArea.left) * 0.2);

        // Adjust window dimensions if minimum is reached
        if (newWndWidth <= minWidth) {
            newWndWidth = minWidth;
        }
        if (newWndHeight <= minHeight) {
            newWndHeight = minHeight;
        }

        // Compute edge coordinates used to center window
        leftEdgeXCoord = rect.left;
        topEdgeYCoord  = rect.top;

        // Adjust edge coordinates if window is resized
        if (newWndWidth != minWidth) {
            leftEdgeXCoord = rect.left - (widthChange / 2);
        }
        if (newWndHeight != minHeight) {
            topEdgeYCoord = rect.top - (heightChange / 2);
        }

        // TODO [H]: Account for misalignment of centering when minimum dimensions
        // are reached. Find a way to indicate when minimum is reached as to
        // not recalculate edge coordinates. (FEATURE, NOT BUG...)

        // Undo window maximization if necessary
        if (IsZoomed(hWnd)) {
            ShowWindow(hWnd, SW_RESTORE);
        }

        // Reposition window
        MoveWindow(hWnd,
                   leftEdgeXCoord,
                   topEdgeYCoord,
                   newWndWidth,
                   newWndHeight,
                   TRUE);

        return SUCCESS;
    } else {
        return BAD_RECT_READ;
    }
}

BOOL TrayIconExists(HWND hWnd) {
    // Get icon data
    NOTIFYICONIDENTIFIER icon = {};

    icon.cbSize   = sizeof(NOTIFYICONIDENTIFIER);
    icon.hWnd     = hWnd;
    icon.uID      = ID_TRAY_ICON;
    icon.guidItem = GUID_NULL;

    // Get icon coordinates
    RECT rect = {};

    HRESULT result = Shell_NotifyIconGetRect(&icon, &rect);

    // result assigned S_OK if icon coordinates are successfully copied to rect
    return result == S_OK;
}
