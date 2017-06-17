#include "AlgoDLL.h"
#include <winapifamily.h>
#include <functional>
#include <windows.h>
#include <memory>
#include <vector>

bool AlgoDLL::loadGetAlgFuncFromDLL(string path)
{
		UINT oldMode = SetErrorMode(0);
		SetErrorMode(oldMode | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
		HINSTANCE hDll;
		try {
			// Load dynamic library 
			hDll = LoadLibraryA(path.c_str());
			using FunctionPtr = IBattleshipGameAlgo* (*) ();
			if (hDll)
			{
				// GetAlgorithm function
				auto GetAlgorithm = reinterpret_cast<FunctionPtr>(GetProcAddress(hDll, "GetAlgorithm"));
				this->getAlgoFunc = getAlgoFunc;
			}
			else {
				Logger("AlgoDLL").Warning("Failed to load library: " + path);
				return false;
			}
		}
		catch (exception e) {
			Logger("AlgoDLL").Warning("Failed to load library: " + path);
			return false;
		}

		return true;

		
}
