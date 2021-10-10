#pragma once
float Nigth_mod_value = 1.5f;

void Nigth_Mod()
{
	for (int i = 0; i < 2048; i++)
	{
		int entity = readMem<DWORD>(client + dwEntityList + i * 0x10);

		if (entity)
		{
			int EnityClassID = readMem<DWORD>(entity + 0x8);
			
			int a = readMem<DWORD>(EnityClassID + 2 * 0x4);
			int b = readMem<DWORD>(a +  0x1);
			int result = readMem<DWORD>(b + 0x14);

			if (result == 69)
			{
				writeMem<int>(entity + m_bUseCustomAutoExposureMin, 1);
				writeMem<int>(entity + m_bUseCustomAutoExposureMax, 1);

				writeMem<float>(entity + m_flCustomAutoExposureMin, Nigth_mod_value);
				writeMem<float>(entity + m_flCustomAutoExposureMax, Nigth_mod_value);
			}
		}	
	}	
}
