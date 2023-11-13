#pragma once

#include <Windows.h>
#include <vector>

namespace meme
{
	DWORD Ammo = 0x00183828;
	std::vector<DWORD> ammo_offsets = { 0x8, 0x4E8, 0x16C, 0x98, 0x38C }; //Pointer/Offsets might be diff
}
