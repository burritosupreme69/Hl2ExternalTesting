// hackanygamept2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include "proc.h"
#include "mem.h"
#include "proc.h"

int main()
{
	HANDLE hProcess = 0;

	uintptr_t serverdllBaseAddress = 0, localPlayerPtr = 0, healthAddr = 0, shieldAddr = 0;
	//uintptr_t serverdllBaseAddress = 0, localPlayerPtr = 0;
	bool bHealth = false, bShield = false, bAmmo = false;

	const int newValue = 1337;


	
	

   // std::cout << "Hello World!\n";


	//get ProcId of the target process

	//DWORD procId = GetProcId(L"hl2.exe");

	DWORD procId = GetProcId(L"hl2.exe");
	

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

		serverdllBaseAddress = GetModuleBaseAddress(procId, L"server.dll");

		localPlayerPtr = serverdllBaseAddress + 0x634174;

			healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xe0 });

			shieldAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xd30 });

	}

	else
	{
		std::cout << "Process not found, press enter to exit\n";
		getchar();
		return 0;
	}
	
	DWORD dwExit = 0;

	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bHealth = !bHealth;

			
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bShield = !bShield;
\
		}
	


		//unlimited ammo patch
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bAmmo = !bAmmo;

			if (bAmmo)
			{
				//FF 06 = inc [esi]
				mem::PatchEx((BYTE*)(serverdllBaseAddress + 0xE71CA), (BYTE*)"\x89\x3E", 2, hProcess);
			}

			else
			{
				//FF 0E = dec [esi]
				mem::PatchEx((BYTE*)(serverdllBaseAddress + 0xE71CA), (BYTE*)"\x90\x90", 2, hProcess);
			}
		}







		//continues write or freeze
		if (bHealth)
		{
			mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hProcess);
		}
		Sleep(10);
		
		if (bShield)
		{
			mem::PatchEx((BYTE*)shieldAddr, (BYTE*)&newValue, sizeof(newValue), hProcess);
		}
		Sleep(10);

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			return 0;
		}


	}

	std::cout << "Process not found, press enter to exit\n";
	getchar();
	return 0;

		//Getmodulebaseaddress

		//right one for server dll that deals with health and shield
	//uintptr_t serverdllBaseAddress = GetModuleBaseAddress(procId, L"server.dll");

	//get handle to process
	//	HANDLE hProcess = 0;
	//	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	//resolve base adddress of the pointer chain for shield
	//	uintptr_t dynamicPtrBaseAddr = serverdllBaseAddress + 0x634174;

	//std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	//resolve our Shield pointer chain
	//	std::vector<unsigned int> shieldOffsets = { 0xD30 };
	//	uintptr_t shieldAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, shieldOffsets);

	//	std::cout << "shieldAddr = " << "0x" << std::hex << shieldAddr << std::endl;


	//read Shield value
	//	int shieldValue = 0;

	//	ReadProcessMemory(hProcess, (BYTE*)shieldAddr, &shieldValue, sizeof(shieldValue), nullptr);

	//	std::cout << "Current shield = " << std::dec << shieldValue << std::endl;


		//write to shield value

	//	int newShield = 500;
	//	WriteProcessMemory(hProcess, (BYTE*)shieldAddr, &newShield, sizeof(newShield), nullptr);


	//read out again

	//	ReadProcessMemory(hProcess, (BYTE*)shieldAddr, &shieldValue, sizeof(shieldValue), nullptr);

	//	std::cout << "New Shield = " << std::dec << shieldValue << std::endl;



	//	getchar();




	//	return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
