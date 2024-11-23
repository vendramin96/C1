#include "windows_main.h"

internal_function bool
LoadEngineDLL(windows_engine_dll *EngineDLL)
{
    bool Result = 0;

    EngineDLL->Library = LoadLibraryA(ENGINE_DLL_FILE_NAME);
    if(!EngineDLL->Library)
    {
        Log("LoadLibraryA() failed: 0x%X\n", GetLastError());
        return Result;
    }

    EngineDLL->UpdateAndRender = (update_and_render *)GetProcAddress(EngineDLL->Library, "UpdateAndRender");
    if(!EngineDLL->UpdateAndRender)
    {
        Log("GetProcAddress() failed: 0x%X\n", GetLastError());
        return Result;
    }

    Result = 1;
    return Result;
}

internal_function bool
WindowsGetWindowDimension(HWND Window, window_dimension *WindowDimension)
{
    bool Result = 0;

    RECT ClientRectangle = {0};
    if(!GetClientRect(Window, &ClientRectangle))
    {
        Log("GetClientRect() failed: 0x%X\n", GetLastError());
        return Result;
    }

    WindowDimension->Width = ClientRectangle.right - ClientRectangle.left;
    WindowDimension->Height = ClientRectangle.bottom - ClientRectangle.top;

    Result = 1;
    return Result;
}

internal_function bool
InitializeVideo(windows_video *Video, int Width, int Height, int BytesPerPixel)
{
    bool Result = 0;

    platform_video *State = &Video->State;
    State->Width = Width;
    State->Height = Height;
    State->BytesPerPixel = BytesPerPixel;
    State->Pitch = Width * BytesPerPixel;
    BITMAPINFO *Information = &Video->Information;
    Information->bmiHeader.biSize = sizeof(Information->bmiHeader);
    Information->bmiHeader.biWidth = Width;
    Information->bmiHeader.biHeight = Height;
    Information->bmiHeader.biPlanes = 1;
    Information->bmiHeader.biBitCount = (WORD)(State->BytesPerPixel * 8);
    Information->bmiHeader.biCompression = BI_RGB;
    State->Memory = PlatformAllocateMemory(Width * Height * BytesPerPixel);
    if(!State->Memory)
    {
        return Result;
    }
    
    Result = 1;
    return Result;
}

LRESULT
WindowsMainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_CLOSE:
        {
            DestroyWindow(Window);
        } break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;

        default:
        {
            Result = DefWindowProcW(Window, Message, WParam, LParam);
        } break;
    }

    return Result;
}

int APIENTRY 
WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, int ShowCode)
{
    int Result = -1;

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    windows_engine_dll WindowsEngineDLL = {0};
    if(!LoadEngineDLL(&WindowsEngineDLL))
    {
        Log("Failed to load engine dll\n");
        return Result;
    }

    windows_video WindowsVideo = {0};
    if(!InitializeVideo(&WindowsVideo, WINDOW_WIDTH, WINDOW_HEIGHT, 4))
    {
        Log("Failed to initialize video\n");
        return Result;
    }

    platform_memory WindowsMemory = {0};
    WindowsMemory.PermanentMemorySize = Megabytes(256);
    WindowsMemory.PermanentMemory = PlatformAllocateMemory(WindowsMemory.PermanentMemorySize);
    if(!WindowsMemory.PermanentMemory)
    {
        Log("Failed to allocate memory for platform_memory WindowsMemory\n");
        return Result;
    }

    WNDCLASSEXW WindowClass = {0};
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClass.lpfnWndProc = WindowsMainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon;
    WindowClass.hCursor = LoadCursorA(0, IDC_ARROW);
    //WindowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    WindowClass.lpszClassName = L"Windows Main";
    if(!RegisterClassExW(&WindowClass))
    {
        Log("RegisterClassExW() failed: 0x%X\n", GetLastError());
        return Result;
    }

    HWND Window = CreateWindowExW(0, WindowClass.lpszClassName, WindowClass.lpszClassName, WS_OVERLAPPEDWINDOW, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, Instance, 0);
    if(!Window)
    {
        Log("CreateWindowExW() failed: 0x%X\n", GetLastError());
        return Result;
    }

    HDC DeviceContext = GetDC(Window);
    if(!DeviceContext)
    {
        Log("GetDC() failed\n");
        return Result;
    }

    RECT WindowRectangle = {0};
    WindowRectangle.right = WINDOW_WIDTH;
    WindowRectangle.bottom = WINDOW_HEIGHT;
    if(!AdjustWindowRectExForDpi(&WindowRectangle, WS_OVERLAPPEDWINDOW, FALSE, 0, GetDpiForWindow(Window)))
    {
        Log("AdjustWindowRectExForDpi() failed: 0x%X\n", GetLastError());
        return Result;
    }

    if(!SetWindowPos(Window, 0, 0, 0, WindowRectangle.right - WindowRectangle.left, WindowRectangle.bottom - WindowRectangle.top, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW))
    {
        Log("SetWindowPos() failed: 0x%X\n", GetLastError());
        return Result;
    }

    RECT C = {0};
    GetClientRect(Window, &C);

    bool IsRunning = 1;
    while(IsRunning)
    {
        MSG Message = {0};
        while(PeekMessageW(&Message, 0, 0, 0, PM_REMOVE))
        {
            switch(Message.message)
            {
                case WM_QUIT:
                {
                    TranslateMessage(&Message);
                    DispatchMessageW(&Message);

                    IsRunning = 0;
                } break;

                default:
                {
                    TranslateMessage(&Message);
                    DispatchMessageW(&Message);
                } break;
            }
        }

        if(!IsRunning)
        {
            break;
        }

        if(WindowsEngineDLL.UpdateAndRender)
        {
            platform_memory EngineMemory = WindowsMemory;
            platform_video EngineVideo = WindowsVideo.State;            
            WindowsEngineDLL.UpdateAndRender(&EngineMemory, &EngineVideo);
        }

        window_dimension WindowDimension = {0};
        if(WindowsGetWindowDimension(Window, &WindowDimension))
        {
            StretchDIBits(DeviceContext, 0, 0, WindowDimension.Width, WindowDimension.Height, 0, 0, WindowsVideo.State.Width, WindowsVideo.State.Height, WindowsVideo.State.Memory, &WindowsVideo.Information, DIB_RGB_COLORS, SRCCOPY);
        }
    }

    Result = 0;
    return Result;
}