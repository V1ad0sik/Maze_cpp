#pragma once

bool Chams_status = false;
float Chams_color[3] = { 255, 255, 255 };

void Chams()
{
	while (Chams_status)
	{
		int local_player = readMem<DWORD>(client + dwLocalPlayer);

		if (local_player)
		{
			for (int i = 0; i < 32; i++)
			{
				int entity = readMem<DWORD>(client + dwEntityList + i * 0x10);

				if (entity)
				{
					int entity_team_id = readMem<DWORD>(entity + m_iTeamNum);
					int team = readMem<DWORD>(local_player + m_iTeamNum);

					if (entity_team_id != team)
					{
						writeMem<int>(entity + m_clrRender, Chams_color[0] * 255);
						writeMem<int>(entity + m_clrRender + 1, Chams_color[1] * 255);
						writeMem<int>(entity + m_clrRender + 2, Chams_color[2] * 255);
						writeMem<int>(entity + m_clrRender + 3, 1);
					}
				}
			}
		}

		Sleep(50);
	}
}

void Chams_Reset()
{
	int player = readMem<DWORD>(client + dwLocalPlayer);

	if (player)
	{
		for (int i = 0; i < 32; i++)
		{
			int entity = readMem<DWORD>(client + dwEntityList + i * 0x10);

			if (entity)
			{
				int entity_team_id = readMem<DWORD>(entity + m_iTeamNum);
				int team = readMem<DWORD>(player + m_iTeamNum);

				if (entity_team_id != team)
				{
					writeMem<int>(entity + m_clrRender, 255);
					writeMem<int>(entity + m_clrRender + 1, 255);
					writeMem<int>(entity + m_clrRender + 2, 255);
					writeMem<int>(entity + m_clrRender + 3, 1);
				}
			}
		}
	}
}
