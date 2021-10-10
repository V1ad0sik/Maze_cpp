#pragma once

bool GlowESP_default_status = false;
bool GlowESP_base_HP_status = false;

float ESP_color[3] = { 255.f, 255.f, 255.f };
float ESP_Line = 0.7f;

void GlowESP_default()
{
    while (GlowESP_default_status)
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
                    int my_team = readMem<DWORD>(local_player + m_iTeamNum);

                    if (entity_team_id != my_team)
                    {
                        int glow_manager = readMem<DWORD>(client + dwGlowObjectManager);
                        int entity_glow = readMem<DWORD>(entity + m_iGlowIndex);

                        writeMem<float>(glow_manager + entity_glow * 0x38 + 0x8, ESP_color[0]);
                        writeMem<float>(glow_manager + entity_glow * 0x38 + 0xC, ESP_color[1]);
                        writeMem<float>(glow_manager + entity_glow * 0x38 + 0x10, ESP_color[2]);
                        writeMem<float>(glow_manager + entity_glow * 0x38 + 0x14, ESP_Line);

                        writeMem<bool>(glow_manager + entity_glow * 0x38 + 0x28, true);
                    }
                }
            }
        }

        Sleep(20);
    }
}


void GlowESP_base_HP()
{
    int color[3];

    while (GlowESP_base_HP_status)
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
                    int my_team = readMem<DWORD>(local_player + m_iTeamNum);

                    if (entity_team_id != my_team)
                    {
                        int glow_manager = readMem<DWORD>(client + dwGlowObjectManager);
                        int entity_glow = readMem<DWORD>(entity + m_iGlowIndex);
                        int entity_hp = readMem<DWORD>(entity + m_iHealth);

                        if (entity_hp >= 60) { color[0] = 0; color[1] = 255; color[2] = 0; }
                        if (entity_hp < 60 && entity_hp >= 20) { color[0] = 255; color[1] = 255; color[2] = 0; }
                        if (entity_hp < 20 && entity_hp >= 1) { color[0] = 255; color[1] = 0; color[2] = 0; }

                        writeMem<float>(glow_manager + entity_glow * 0x38 + 0x8, color[0]);
                        writeMem<float>(glow_manager + entity_glow * 0x38 + 0xC, color[1]);
                        writeMem<float>(glow_manager + entity_glow * 0x38 + 0x10, color[2]);
                        writeMem<float>(glow_manager + entity_glow * 0x38 + 0x14, ESP_Line);

                        writeMem<bool>(glow_manager + entity_glow * 0x38 + 0x28, true);
                    }
                }
            }
        }

        Sleep(20);
    }
}