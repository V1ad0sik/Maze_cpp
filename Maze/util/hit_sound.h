#pragma once
bool Hit_Sound_status = false;

void Hit_Sound()
{
	int hit = 0, last_hit = 0;

	while (Hit_Sound_status)
	{
		int local_player = readMem<DWORD>(client + dwLocalPlayer);

		if (local_player)
		{
			hit = readMem<DWORD>(local_player + m_totalHitsOnServer);

			if (hit > last_hit)
			{
				last_hit = hit;
				PlaySound("res/sound/sound.wav", NULL, SND_ASYNC | SND_NOSTOP);
			}

		}

		Sleep(10);
	}
}