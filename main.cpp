#include <windows.h>
#include <string>

#define IDC_INPUT  101
#define IDC_BUTTON 102

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void DrawHeader(HDC hdc, RECT rect) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 191, 166));
    HFONT hFont = CreateFont(
        28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI"
    );
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    DrawText(hdc, L"ChatGPT Atlas — Windows Edition", -1, &rect, DT_CENTER | DT_TOP);
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"AtlasDemoWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = CreateSolidBrush(RGB(15, 15, 25));
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"ChatGPT Atlas — Windows Edition (Demo)",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 680, 480,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void OnSearchClick(HWND hwnd) {
    wchar_t input[256];
    GetWindowText(GetDlgItem(hwnd, IDC_INPUT), input, 256);

    std::wstring query = L"https://www.google.com/search?q=" + std::wstring(input);
    ShellExecute(nullptr, L"open", query.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hInput, hButton;

    switch (msg) {
    case WM_CREATE:
        CreateWindow(L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_BITMAP,
            20, 70, 100, 100, hwnd, nullptr, nullptr, nullptr);

        hInput = CreateWindow(L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            130, 120, 380, 30, hwnd, (HMENU)IDC_INPUT, nullptr, nullptr);

        hButton = CreateWindow(L"BUTTON", L"Ask ChatGPT",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            520, 120, 120, 30, hwnd, (HMENU)IDC_BUTTON, nullptr, nullptr);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON) {
            OnSearchClick(hwnd);
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        DrawHeader(hdc, rect);

        SetTextColor(hdc, RGB(230, 230, 230));
        SetBkMode(hdc, TRANSPARENT);
        HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                 ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                                 CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

        TextOut(hdc, 130, 170, L"Smart, AI-powered web browser demo", 36);
        TextOut(hdc, 130, 200, L"Type your question and click “Ask ChatGPT”", 40);

        SelectObject(hdc, oldFont);
        DeleteObject(hFont);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
