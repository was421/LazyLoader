#pragma once
#include <filesystem>
#include <windows.h>
#include <iostream>
#include <map>

namespace LazyLoader {

	__declspec(selectany) std::map<std::filesystem::path, HMODULE> loadedDlls;

	void getAllDllsInFolder(std::filesystem::path folder, std::vector<std::filesystem::path>& dlls) {
		if (!std::filesystem::exists(folder)) {
			return;
		}
		for (const auto& entry : std::filesystem::directory_iterator(folder)) {
			if (entry.is_regular_file() && entry.path().extension() == ".dll") {
				std::filesystem::path dll = entry.path();
				if(!dll.is_absolute()){
					dll = std::filesystem::absolute(dll);
				}
				dlls.push_back(dll);
			}
		}
	}

	void SortDllsWithPrioityMap(std::map<int, std::wstring>& loadOrder, std::vector<std::filesystem::path>& dlls) {
		std::vector<std::filesystem::path> sortedDlls;
		for (const auto& [key, value] : loadOrder) {
			for (const auto& dll : dlls) {
				if (dll.filename() == value) {
					sortedDlls.push_back(dll);
				}
			}
		}
		for (const auto& dll : dlls) {
			if (std::find(sortedDlls.begin(), sortedDlls.end(), dll) == sortedDlls.end()) {
				sortedDlls.push_back(dll);
			}
		}
		dlls = sortedDlls;
	}

	void LoadDlls(std::vector<std::filesystem::path>& dlls) {
		for (const auto& dll : dlls) {
			try{
				HMODULE h = LoadLibraryW(dll.c_str());
				loadedDlls[dll] = h;
			}
			catch (std::exception e) {
				std::cerr << "[LazyLoader]" << "Failed to load dll: " << dll << " Exception: " << e.what() << std::endl;
				if ((GetKeyState(VK_SCROLL) & 0x0001) != 0) {
					MessageBoxA(NULL, std::string("Failed to load dll: " + dll.filename().string() + "\n" + e.what()).c_str(), "LazyLoader", MB_OK);
				}
			}
		}
	}
}