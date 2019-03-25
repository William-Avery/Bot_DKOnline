#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

class Game
{
public:
	DWORD ProcessId;
	uintptr_t moduleBase;
	HANDLE hProcess;
};

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindDMAAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
