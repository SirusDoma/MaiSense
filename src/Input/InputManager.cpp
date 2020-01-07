#include "pch.h"

#include <MaiSense/Input/InputManager.hpp>
#include <MaiSense/Input/InputController.hpp>
#include <MaiSense/Process.hpp>

#include <detours.h>
#include <string>

namespace MaiSense
{
    HWND(WINAPI* TrueCreateWindowExA)(
        DWORD     dwExStyle,
        LPCSTR    lpClassName,
        LPCSTR    lpWindowName,
        DWORD     dwStyle,
        int       X,
        int       Y,
        int       nWidth,
        int       nHeight,
        HWND      hWndParent,
        HMENU     hMenu,
        HINSTANCE hInstance,
        LPVOID    lpParam
    ) = CreateWindowExA;

    DWORD(WINAPI* TrueGameInput)() = (DWORD(WINAPI*)())0x00516107;

    HHOOK   InputManager::hHook;
    HWND    InputManager::hWnd;
    Sensor *InputManager::sensor;
    std::vector<InputController*> InputManager::controllers;

    void InputManager::Hook()
    {
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)TrueCreateWindowExA, HookCreateWindowExA);
        DetourAttach(&(PVOID&)TrueGameInput, HookGameInput);
        DetourTransactionCommit();
    }

    void InputManager::Unhook()
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)TrueCreateWindowExA, HookCreateWindowExA);
        DetourDetach(&(PVOID&)TrueGameInput, HookGameInput);
        DetourTransactionCommit();
    }

    bool InputManager::Install(InputController *controller)
    {
        if (controller)
        {
            controllers.push_back(controller);
            return true;
        }

        return false;
    }

    Sensor *InputManager::GetSensor()
    {
        if (!sensor)
            sensor = new Sensor();

        return sensor;
    }

    HHOOK InputManager::GetHookHandle()
    {
        return hHook;
    }

    HWND InputManager::GetGameWindow()
    {
        return hWnd;
    }

    bool InputManager::Ready()
    {
        return !DetourIsHelperProcess();
    }

    DWORD __stdcall InputManager::HookGameInput()
    {
        GetSensor()->Reset();
        auto result = TrueGameInput();
        GetSensor()->ProcessQueue();

        return result;
    }

    HWND WINAPI InputManager::HookCreateWindowExA(
        DWORD     dwExStyle,
        LPCSTR    lpClassName,
        LPCSTR    lpWindowName,
        DWORD     dwStyle,
        int       X,
        int       Y,
        int       nWidth,
        int       nHeight,
        HWND      hWndParent,
        HMENU     hMenu,
        HINSTANCE hInstance,
        LPVOID    lpParam
    )
    {
        hWnd = TrueCreateWindowExA(
            dwExStyle,
            lpClassName,
            lpWindowName,
            dwStyle,
            X,
            Y,
            nWidth,
            nHeight,
            hWndParent,
            hMenu,
            hInstance,
            lpParam
        );

        hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, hInstance, GetCurrentThreadId());
        if (hHook == NULL)
        {
            MessageBoxA(NULL, ("MAISENSE: Failed to install hook: " + std::to_string(GetLastError())).c_str(), "Hook", MB_ICONEXCLAMATION);
            CloseWindow(hWnd);

            return NULL;
        }

        return hWnd;
    }

    LRESULT WINAPI InputManager::GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        auto msg = (LPMSG)lParam;
        if (nCode >= 0 && msg != NULL)
        {
            for (auto controller : controllers)
            {
                if (controller->Check(msg->message))
                    controller->OnInput(nCode, wParam, lParam);
            }
        }

        return CallNextHookEx(hHook, nCode, wParam, lParam);
    }
    
}