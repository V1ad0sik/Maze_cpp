#pragma once
bool Skin_changer_status = false;

nlohmann::json skin;

void read_skin_config()
{
	std::ifstream file{ "res/skins/setting.json" };
	bool file_status = file.is_open();

	if (file_status)
	{
		skin = nlohmann::json::parse(file);
		file.close();
	}

	else
	{
		MessageBox(0, "Не удалось открыть файл (SKIN).", "Внимание!", MB_OK);
		exit(0);
	}
}

int get_index(int number)
{
	int index = 0;

	switch (number)
	{
		case 32: index = skin["P2000"]; break;
		case 61: index = skin["USP-S"]; break;
		case 4: index = skin["Glock"]; break;
		case 2: index = skin["Dual Berettas"]; break;
		case 36: index = skin["P250"]; break;
		case 30: index = skin["Tec-9"]; break;
		case 63: index = skin["CZ75-Auto"]; break;
		case 1: index = skin["Desert Eagle"]; break;
		case 3: index = skin["Five-SeveN"]; break;
		case 64: index = skin["R8"]; break;
		case 35: index = skin["Nova"]; break;
		case 25: index = skin["XM1014"]; break;
		case 27: index = skin["MAG-7"]; break;
		case 14: index = skin["M249"]; break;
		case 28: index = skin["Negev"]; break;
		case 29: index = skin["Sawed-Off"]; break;
		case 17: index = skin["MAC-10"]; break;
		case 23: index = skin["MP5-SD"]; break;
		case 24: index = skin["UMP-45"]; break;
		case 19: index = skin["P90"]; break;
		case 26: index = skin["PP-19"]; break;
		case 34: index = skin["MP9"]; break;
		case 33: index = skin["MP7"]; break;
		case 10: index = skin["FAMAS"]; break;
		case 16: index = skin["M4A4"]; break;
		case 60: index = skin["M4A1-S"]; break;
		case 40: index = skin["SSG 08"]; break;
		case 8: index = skin["AUG"]; break;
		case 9: index = skin["AWP"]; break;
		case 38: index = skin["SCAR-20"]; break;
		case 13: index = skin["Galil"]; break;
		case 7: index = skin["AK-47"]; break;
		case 39: index = skin["SG 553"]; break;
		case 11: index = skin["C3SG1"]; break;
		default: index = 0; break;
	}

	return index;
}


void Skin_changer()
{
	int index = 0;
	read_skin_config();

	while (Skin_changer_status)
	{
		int local_player = readMem<DWORD>(client + dwLocalPlayer);

		if (local_player)
		{
			for (int i = 0; i < 8; i++)
			{
				int my_weapons = readMem<DWORD>(local_player + m_hMyWeapons + (i - 1) * 0x4) & 0xFFF;
				int weapon_address = readMem<DWORD>(client + dwEntityList + (my_weapons - 1) * 0x10);

				if (weapon_address)
				{
					int weapon_id = readMem<DWORD>(weapon_address + m_iItemDefinitionIndex);
					int weapon_owner = readMem<DWORD>(weapon_address + m_OriginalOwnerXuidLow);

					index = get_index(weapon_id);

					writeMem<int>(weapon_address + m_iItemIDHigh, -1);
					writeMem<int>(weapon_address + m_nFallbackPaintKit, index);
					writeMem<int>(weapon_address + m_iAccountID, weapon_owner);
					writeMem<int>(weapon_address + m_nFallbackStatTrak, 1337);
					writeMem<int>(weapon_address + m_nFallbackSeed, 520);
				}
			}
		}

		if (GetAsyncKeyState(VK_F6))
		{
			int engine_state = readMem<DWORD>(engine + dwClientState);
			writeMem<int>(engine_state + 0x174, -1);
		}
	}
}
