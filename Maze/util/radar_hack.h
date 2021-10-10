#pragma once

bool Radar_Hack_Status = false;

void Radar_Hack()
{
	while (Radar_Hack_Status)
	{
		int local_player = readMem<DWORD>(client + dwLocalPlayer);

		if (local_player)
		{
			int team = readMem<DWORD>(local_player + m_iTeamNum);

			for (int i = 0; i < 64; i++)
			{
				int entity = readMem<DWORD>(client + dwEntityList + i * 0x10);
				int entity_team = readMem<DWORD>(entity + m_iTeamNum);

				if (entity_team != team) { writeMem<int>(entity + m_bSpotted, 1); }
			}
		}

		Sleep(100);
	}
}