#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <map>
#include "ini.h"

namespace iniConfig {

	const char* defaultFileName = "lazyLoad.ini";

	const char* defaultFile =
	";LazyLoader by Church Guard\n\n"
	"; NOTE: THIS METHOD DOES NOT WORK FOR ALL DLL MODS!!!\n"
	";       IF YOU ARE CRASHING OR MODS ARE NOT WORKING THAT MEANS\n"
	";       ONE OF THE MODS MUST BE PROPERLY LOADED IN [CHAINLOAD].\n"
	";       THERE CAN ONLY BE ONE PROPERLY LOADED MOD AT A TIME\n\n"
	"; Define the folder containing dll files to be lazy loaded.\n"
	"[LAZYLOAD]\n"
	"dllModFolderName=dllMods\n"
	"\n"
	"; Here you can define the order in which dlls are loaded from\n"
	"; the folder defined above. LazyLoader will follow this order and if there\n"
	"; are any additional dlls to load they will be loaded by windows default order.\n"
	"; Below is an example, when you add your own entries please do so without\n"
	"; the \";\" at the start of the line and ensure they are below [LOADORDER].\n"
	"; 0=honestHerchant.dll\n"
	"; 1=wexdust.dll\n"
	"; 2=modengine.dll\n"
	"[LOADORDER]\n"
	"\n"
	"\n"
	"; To load another dll that reqires it be dinput8.dll, simply rename the\n"
	"; other dll file, place it the same folder as this and enter its name below\n"
	"; Depending on what the other dll does this may not work, but usually does\n"
	"; example: dll = wexdust.dll\n"
	"[CHAINLOAD]\n"
	"dll=\n";

	namespace util {
		//https://stackoverflow.com/questions/6691555/converting-narrow-string-to-wide-string
		inline std::wstring convert_s2ws(const std::string& as)
		{
			// deal with trivial case of empty string
			if (as.empty())    return std::wstring();

			// determine required length of new string
			size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);

			// construct new string of required length
			std::wstring ret(reqLength, L'\0');

			// convert old string to new string
			::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());

			// return new string ( compiler should optimize this away )
			return ret;
		}
	}


	__declspec(selectany) std::filesystem::path dllModFolderName = {};
	__declspec(selectany) std::filesystem::path dllChainloadName = {};
	__declspec(selectany) std::map<int, std::wstring> loadOrder = {};

	void writeDefaultConfigIfNotExists() {
		std::filesystem::path filePath = std::filesystem::current_path().append(defaultFileName);
		if (!std::filesystem::exists(filePath)) {
			std::ofstream file(filePath);
			file << defaultFile;
			file.close();
		}
	}

	void LoadConfig() {
		writeDefaultConfigIfNotExists();
		std::filesystem::path filePath = std::filesystem::current_path().append(defaultFileName);
		mINI::INIFile cfgFile(filePath.string());
		mINI::INIStructure ini;
		if (cfgFile.read(ini)) {
			dllModFolderName = util::convert_s2ws(ini.get("LAZYLOAD").get("dllModFolderName"));
			if(!dllModFolderName.empty() && !dllModFolderName.is_absolute()) {
				dllModFolderName = std::filesystem::absolute(dllModFolderName);
			}
			dllChainloadName = util::convert_s2ws(ini.get("CHAINLOAD").get("dll"));
			if(!dllChainloadName.empty() && !dllChainloadName.is_absolute()) {
				dllChainloadName = std::filesystem::absolute(dllChainloadName);
			}
			for (auto& [key, value] : ini.get("LOADORDER")) {
				try {
					loadOrder[std::stoi(key)] = util::convert_s2ws(value);
				}
				catch (std::invalid_argument) {
					std::cerr << "[LazyLoader]" << "Invalid load order key: " << key << std::endl;
				}
			}
		}

	}

}