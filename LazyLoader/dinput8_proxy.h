#pragma once


#include <windows.h>
#include <dinput.h>
#include <filesystem>

namespace dinput8_proxy {
    __declspec(selectany) HMODULE hDinput8 = NULL;

	std::filesystem::path getRealDinput8Path() {
		WCHAR path[MAX_PATH];
		GetSystemDirectoryW(path, MAX_PATH);
		return std::filesystem::path(path).append(L"dinput8.dll");
	}

	bool ChainloadNext(std::filesystem::path dllPath) {
		if (hDinput8) {
			return true;
		}
		if (!dllPath.empty()) {
			if (!dllPath.is_absolute()) {
				dllPath = std::filesystem::absolute(dllPath);
			}
			hDinput8 = LoadLibraryW(dllPath.c_str());
		}
		if (!hDinput8) {
			hDinput8 = LoadLibraryW(getRealDinput8Path().c_str());
			if (!hDinput8) {
				return false;
			}
		}
		return true;
	}
}

typedef HRESULT(WINAPI* DirectInput8Create_t)(
	HINSTANCE hinst,
	DWORD dwVersion,
	REFIID riidltf,
	LPVOID* ppvOut,
	LPUNKNOWN punkOuter
);
HRESULT _DirectInput8Create(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter
) {
    if (!dinput8_proxy::hDinput8) {
        return E_FAIL;
	}
    DirectInput8Create_t DirectInput8Create_fn = (DirectInput8Create_t)GetProcAddress(dinput8_proxy::hDinput8, "DirectInput8Create");
    if (!DirectInput8Create_fn) {
        return E_FAIL;
    }
    return DirectInput8Create_fn(hinst, dwVersion, riidltf, ppvOut, punkOuter);
};


typedef HRESULT(WINAPI* DllCanUnloadNow_t)();
HRESULT _DllCanUnloadNow() {
	if (!dinput8_proxy::hDinput8) {
		return E_FAIL;
	}
	DllCanUnloadNow_t DllCanUnloadNow_fn = (DllCanUnloadNow_t)GetProcAddress(dinput8_proxy::hDinput8, "DllCanUnloadNow");
	if (!DllCanUnloadNow_fn) {
		return E_FAIL;
	}
	return DllCanUnloadNow_fn();
};

typedef HRESULT(WINAPI* DllGetClassObject_t)(
	REFCLSID rclsid,
	REFIID riid,
	LPVOID* ppv
);
HRESULT _DllGetClassObject(
	REFCLSID rclsid,
	REFIID riid,
	LPVOID* ppv
) {
	if (!dinput8_proxy::hDinput8) {
		return E_FAIL;
	}
	DllGetClassObject_t DllGetClassObject_fn = (DllGetClassObject_t)GetProcAddress(dinput8_proxy::hDinput8, "DllGetClassObject");
	if (!DllGetClassObject_fn) {
		return E_FAIL;
	}
	return DllGetClassObject_fn(rclsid, riid, ppv);
};

typedef HRESULT(WINAPI* DllRegisterServer_t)();
HRESULT _DllRegisterServer() {
	if (!dinput8_proxy::hDinput8) {
		return E_FAIL;
	}
	DllRegisterServer_t DllRegisterServer_fn = (DllRegisterServer_t)GetProcAddress(dinput8_proxy::hDinput8, "DllRegisterServer");
	if (!DllRegisterServer_fn) {
		return E_FAIL;
	}
	return DllRegisterServer_fn();
};

typedef HRESULT(WINAPI* DllUnregisterServer_t)();
HRESULT _DllUnregisterServer() {
	if (!dinput8_proxy::hDinput8) {
		return E_FAIL;
	}
	DllUnregisterServer_t DllUnregisterServer_fn = (DllUnregisterServer_t)GetProcAddress(dinput8_proxy::hDinput8, "DllUnregisterServer");
	if (!DllUnregisterServer_fn) {
		return E_FAIL;
	}
	return DllUnregisterServer_fn();
};

typedef LPCDIDATAFORMAT(WINAPI* GetdfDIJoystick_t)();
LPCDIDATAFORMAT _GetdfDIJoystick() {
	if (!dinput8_proxy::hDinput8) {
		return nullptr;
	}
	GetdfDIJoystick_t GetdfDIJoystick_fn = (GetdfDIJoystick_t)GetProcAddress(dinput8_proxy::hDinput8, "GetdfDIJoystick");
	if (!GetdfDIJoystick_fn) {
		return nullptr;
	}
	return GetdfDIJoystick_fn();
};