#pragma once
bool TriggetBot_default_status = false;
bool TriggerBot_P_status = false;

int TriggerBot_delay = 50;

bool scann()
{
	int local_player = readMem<DWORD>(client + dwLocalPlayer);

	if (local_player)
	{
		int entity_id = readMem<DWORD>(local_player + m_iCrosshairId);
		int entity = readMem<DWORD>(client + dwEntityList + (entity_id - 1) * 0x10);

		if (entity)
		{
			int entity_team = readMem<DWORD>(entity + m_iTeamNum);
			int player_team = readMem<DWORD>(local_player + m_iTeamNum);

			if (entity_id > 0 && entity_id <= 64 && player_team != entity_team) { return true; }
			else { return false; }
		}
	}

	else { return false; }
}

void TriggerBot_default()
{
	while (TriggetBot_default_status)
	{
		if (scann())
		{
			Sleep(TriggerBot_delay);
			writeMem<int>(client + dwForceAttack, 6);
		}
	}
}

void TriggerBot_P()
{
	while (TriggerBot_P_status)
	{
		if (scann())
		{
			Sleep(TriggerBot_delay);
			if (scann()) { writeMem<int>(client + dwForceAttack, 6); }
		}
	}
}
