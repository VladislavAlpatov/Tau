#pragma once
#include <Windows.h>

namespace SSDK
{
	class CAbstractInterface
	{
	protected:
		template<typename FuncType>
		FuncType GetVirtualFunction(unsigned int index)
		{
			return reinterpret_cast<FuncType>( (*(uintptr_t**)this)[index] );
		}

	};

	template <typename T>
	T* GetInterface(const char* dllname, const char* interfacename)
	{
		typedef void* (__cdecl* tCreateInterface)(const char* name, int* returncode);
		tCreateInterface CreateInterFace = reinterpret_cast<tCreateInterface>(GetProcAddress(GetModuleHandleA(dllname), "CreateInterface"));

		int returnCode = 0;

		return reinterpret_cast<T*>(CreateInterFace(interfacename, &returnCode));

	}

}

