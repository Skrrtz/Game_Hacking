#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include <tchar.h>
#include <TlHelp32.h>

using namespace std;

double uip = 444;
double readTest = 0;

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
	{
		do {
			if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
			{
				dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}

DWORD GetPointerAddress(HWND hwnd, DWORD gameBaseAddr, DWORD address, vector<DWORD> offsets)
{
	DWORD pID = NULL; // Game process ID
	GetWindowThreadProcessId(hwnd, &pID);
	HANDLE phandle = NULL;
	phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	if (phandle == INVALID_HANDLE_VALUE || phandle == NULL);

	DWORD offset_null = NULL;
	ReadProcessMemory(phandle, (LPVOID*)(gameBaseAddr + address), &offset_null, sizeof(offset_null), 0);
	DWORD pointeraddress = offset_null; // the address we need
	for (int i = 0; i < offsets.size() - 1; i++) // we dont want to change the last offset value so we do -1
	{
		ReadProcessMemory(phandle, (LPVOID*)(pointeraddress + offsets.at(i)), &pointeraddress, sizeof(pointeraddress), 0);
	}
	return pointeraddress += offsets.at(offsets.size() - 1); // adding the last offset
}

int main()
{
	HWND hwnd = FindWindowA(NULL, "AssaultCube");
	if (hwnd == NULL)
	{
		cout << "Cannot find window." << endl;
		Sleep(3000);
		exit(-1);
	}
	else
	{
		DWORD procID;
		GetWindowThreadProcessId(hwnd, &procID);
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

		if (procID == NULL)
		{
			cout << "Cannot obtain process." << endl;
			Sleep(3000);
			exit(-1);
		}
		else
		{
			cout << "procID: " << procID << endl;
			cout << "Handle: " << handle << endl;

			char gamemodule1[] = "ac_client.exe";
			DWORD gamebaseAddress1 = GetModuleBaseAddress(_T(gamemodule1), procID);

			DWORD ammoAddr = 0x0017E0A8;
			vector<DWORD> ammoOffsets{ 0x5AC, 0x724, 0x10, 0x62C, 0x490 }; //100% different pointer/offsets
			DWORD ammoPtrAddr = GetPointerAddress(hwnd, gamebaseAddress1, ammoAddr, ammoOffsets);

			DWORD healthAddr = 0x001A3344;
			vector<DWORD> healthOffset{ 0x0, 0xCC, 0x340, 0x168, 0x654 };
			DWORD healthPtrAddr = GetPointerAddress(hwnd, gamebaseAddress1, healthAddr, healthOffset);

			int val = 12773333;

			cout << ReadProcessMemory(handle, (LPVOID*)(ammoPtrAddr), &val, 4, 0);
			WriteProcessMemory(handle, (LPVOID*)(ammoPtrAddr), &val, 4, 0);
			//WriteProcessMemory(handle, (LPVOID*)(val), &val, 4, 0);
		}
	}

	return 0;
}
