#pragma once

bool BHop_status = false;

void BHop()
{
	while (BHop_status)
	{
		if (active_game())
		{
			int local_player = readMem<DWORD>(client + dwLocalPlayer);

			if (local_player)
			{
				if (GetAsyncKeyState(VK_SPACE))
				{
					int on_ground = readMem<DWORD>(local_player + m_fFlags);

					if (on_ground == 257 || on_ground == 263)
					{
						int MouseEnable = readMem<byte>(client + dwMouseEnable);

						if (MouseEnable != 104)
						{
							int force_jump = client + dwForceJump;

							writeMem<int>(force_jump, 6);
							Sleep(20);
							writeMem<int>(force_jump, 4);
						}
					}
				}
			}
		}
	}
}

