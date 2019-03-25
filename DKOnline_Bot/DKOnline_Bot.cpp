#include <iostream>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <stdlib.h> // system("cls")
#include "proc.h"
#include "actions.h"

int main()
{
	Game g;

	//Get ProcId of the taget process
	g.ProcessId = GetProcId(L"DKonline.exe");

	//Get Module Base Address
	g.moduleBase = GetModuleBaseAddress(g.ProcessId, L"DKonline.exe");

	//Get Handle to the Process
	g.hProcess = 0; // Windows returns 0 on error checking.
	g.hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, g.ProcessId);

	///
	///
	///

	//Resolve base address of the pointer chain
	uintptr_t dynamic_healthaddr = g.moduleBase + 0x0075F7A8;
	uintptr_t dynamic_manaaddr = g.moduleBase + 0x0076C2F0;
	uintptr_t dynamic_itemdrop = g.moduleBase + 0x0075E650;
	uintptr_t dynamic_monsterId = g.moduleBase + 0x0075E488;

	//Pointer chain & combine + reverse if read top to bottom (cheat engine)
	std::vector<unsigned int> health_offsets = { 0x11C, 0x78, 0x5F0, 0x1E8, 0x5AC };
	std::vector<unsigned int> mana_offsets = { 0x1E8 };
	std::vector<unsigned int> itemdrop_offsets;
	std::vector<unsigned int> monsterId_offsets;
	std::reverse(health_offsets.begin(), health_offsets.end());
	std::reverse(mana_offsets.begin(), mana_offsets.end());

	uintptr_t health_address = FindDMAAddress(g.hProcess, dynamic_healthaddr, health_offsets);
	uintptr_t mana_address = FindDMAAddress(g.hProcess, dynamic_manaaddr, mana_offsets);
	uintptr_t item_drop_address = FindDMAAddress(g.hProcess, dynamic_itemdrop, itemdrop_offsets);
	uintptr_t monsterId_address = FindDMAAddress(g.hProcess, dynamic_monsterId, monsterId_offsets);

	int health_value = 0;
	int mana_value = 0;
	int itemdrop_value = 0;
	int monsterId_value = 0;

	while(1)
	{
		//Read Health Value

		ReadProcessMemory(g.hProcess, (BYTE*)health_address, &health_value, sizeof(health_address), nullptr);
		ReadProcessMemory(g.hProcess, (BYTE*)mana_address, &mana_value, sizeof(mana_address), nullptr);
		ReadProcessMemory(g.hProcess, (BYTE*)item_drop_address, &itemdrop_value, sizeof(item_drop_address), nullptr);
		ReadProcessMemory(g.hProcess, (BYTE*)monsterId_address, &monsterId_value, sizeof(monsterId_address), nullptr);

		if (monsterId_value != -1) {
			GenerateKey('1', TRUE);
			GenerateKey('1', TRUE);
			GenerateKey('1', TRUE);
			GenerateKey('1', TRUE);
			GenerateKey('1', TRUE);
		}

		std::cout << "Current Health: " << std::dec << health_value << std::endl;
		std::cout << "Current Mana: " << std::dec << mana_value << std::endl;
		std::cout << "Items on Ground: " << std::dec << itemdrop_value << std::endl;
		std::cout << "Selected MonsterID: " << std::dec << monsterId_value << std::endl;
		std::chrono::seconds dura(1);
		std::this_thread::sleep_for(dura);
		system("cls");
	}

	//Write Process Memory
	//int newHealth = 1337;
	//WriteProcessMemory(hProcess, (BYTE*)health_address, &newHealth, sizeof(health_address), nullptr);

	return 0;
}