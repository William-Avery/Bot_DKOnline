#include "actions.h"
#include "proc.h"

void GenerateKey(int vk, BOOL bExtended)
{
	KEYBDINPUT  kb = { 0 };
	INPUT    Input = { 0 };
	// generate down 
	if (bExtended)
		kb.dwFlags = KEYEVENTF_EXTENDEDKEY;
	kb.wVk = vk;
	Input.type = INPUT_KEYBOARD;

	Input.ki = kb;
	::SendInput(1, &Input, sizeof(Input));

	// generate up 
	::ZeroMemory(&kb, sizeof(KEYBDINPUT));
	::ZeroMemory(&Input, sizeof(INPUT));
	kb.dwFlags = KEYEVENTF_KEYUP;
	if (bExtended)
		kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;

	kb.wVk = vk;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;
	::SendInput(1, &Input, sizeof(Input));
}