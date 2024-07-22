#ifndef UNICODE
#define UNICODE
#endif

#include "../include/ReSize.h"

int WINAPI wWinMain(
    _In_ HINSTANCE     hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR        lpCmdLine,
    _In_ int           nCmdShow) {

    // Register main window class
    LPCTSTR mainWndClassName = MAIN_WND_CLASS_NAME;
    LPCTSTR mainWndTitle     = APP_TITLE;

    WNDCLASSEX mainWndClass = {};

    mainWndClass.cbSize        = sizeof(WNDCLASSEX);
    mainWndClass.lpfnWndProc   = WindowProc;
    mainWndClass.hInstance     = hInstance;
    mainWndClass.hIcon         = (HICON) icon;
    mainWndClass.lpszClassName = mainWndClassName;

    RegisterClassEx(&mainWndClass);

    // Generate main window
    HWND mainWnd = CreateWindowEx(
        0,
        mainWndClassName,
        mainWndTitle,
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        HWND_MESSAGE,
        NULL,
        hInstance,
        NULL);

    if (mainWnd == NULL) {
        return BAD_HWND;
    }

    ShowWindow(mainWnd, nCmdShow);
    CenterWindow(mainWnd, MOD_WND_SIZE, 10, 35);

    // Register hotkeys
    if (!RegisterHotKey(mainWnd, ID_HK_DEC_WND_SZ, MOD_CONTROL, '1') ||
        !RegisterHotKey(mainWnd, ID_HK_INC_WND_SZ, MOD_CONTROL, '2') ||
        !RegisterHotKey(mainWnd, ID_HK_CEN_WND, MOD_CONTROL, '3') ||
        !RegisterHotKey(mainWnd,
                        ID_HK_ALT_CEN_WND_1,
                        MOD_CONTROL | MOD_SHIFT,
                        '1') ||
        !RegisterHotKey(mainWnd,
                        ID_HK_ALT_CEN_WND_2,
                        MOD_CONTROL | MOD_SHIFT,
                        '2') ||
        !RegisterHotKey(mainWnd,
                        ID_HK_ALT_CEN_WND_3,
                        MOD_CONTROL | MOD_SHIFT,
                        '3')) {
        return BAD_KEY;
    }

    // Start message loop
    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unregister hotkeys
    UnregisterHotKey(NULL, ID_HK_DEC_WND_SZ);
    UnregisterHotKey(NULL, ID_HK_INC_WND_SZ);
    UnregisterHotKey(NULL, ID_HK_CEN_WND);
    UnregisterHotKey(NULL, ID_HK_ALT_CEN_WND_1);
    UnregisterHotKey(NULL, ID_HK_ALT_CEN_WND_2);

    return (int) msg.wParam;
}
