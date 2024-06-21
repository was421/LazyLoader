// dllmain.cpp : Defines the entry point for the DLL application.
#include "dinput8_proxy.h"
#include "iniConfig.h"
#include "LazyLoader.h"

bool MessageboxConfirmLoad() {
    if ((GetKeyState(VK_SCROLL) & 0x0001) != 0) {
        return MessageBoxA(NULL, "Would you like to load dll mods?", "LazyLoader", MB_YESNO) == IDYES;
    }
	return true;
}

void MessageboxShowDlls(std::vector<std::filesystem::path>& dlls,std::filesystem::path chainload) {
    if ((GetKeyState(VK_SCROLL) & 0x0001) != 0) {
        std::string message = "Dlls to be loaded:\n";
        for (const auto& dll : dlls) {
            message += dll.filename().string() + "\n";
        }
        message += "Chainload dll: " + chainload.filename().string();
        MessageBoxA(NULL, message.c_str(), "LazyLoader", MB_OK);
    }
}

void setup() {
    if (MessageboxConfirmLoad()) {

        iniConfig::LoadConfig();

        std::vector<std::filesystem::path> dlls;
        LazyLoader::GetAllDllsInFolder(iniConfig::dllModFolderName, dlls);
        LazyLoader::SortDllsWithPrioityMap(iniConfig::loadOrder, dlls);

        MessageboxShowDlls(dlls, iniConfig::dllChainloadName);

        LazyLoader::LoadDlls(dlls);
    }
    
    dinput8_proxy::ChainloadNext(iniConfig::dllChainloadName);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		setup();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

