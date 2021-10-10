#pragma once

bool Anti_Falsh_status = false;
int FlashMaxAlpha = 0;

void Anti_Flash()
{
	while (Anti_Falsh_status)
	{
		int local_player = readMem<DWORD>(client + dwLocalPlayer);

		if (local_player)
		{
			writeMem<float>(local_player + m_flFlashMaxAlpha, FlashMaxAlpha);
			Sleep(1000);
		}
	}
}

void Anti_Flash_Reset()
{
	int local_player = readMem<DWORD>(client + dwLocalPlayer);

	if (local_player) {writeMem<float>(local_player + m_flFlashMaxAlpha, 255);}
}
