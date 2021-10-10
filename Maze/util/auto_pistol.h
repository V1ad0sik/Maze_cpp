#pragma once

bool Auto_Pistol_Status = false;

void Auto_Pistol()
{
	while (Auto_Pistol_Status)
	{
		if (active_game())
		{
			if (GetAsyncKeyState(1) == -32768)
			{
				int MouseEnable = readMem<byte>(client + dwMouseEnable);

				if (MouseEnable != 104)
				{
					writeMem<int>(client + dwForceAttack, 6);
					Sleep(20);
				}
			}
		}
	}
}
