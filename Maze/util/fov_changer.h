#pragma once
int FOV_values = 90;

void FOV()
{
	int local_player = readMem<DWORD>(client + dwLocalPlayer);
	if (local_player) { writeMem<int>(local_player + m_iDefaultFOV, FOV_values); }
}
