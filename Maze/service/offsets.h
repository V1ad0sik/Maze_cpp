#pragma once
#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")


void remove_file() { remove("addres.json"); }
void load_file() { URLDownloadToFile(NULL, "https://raw.githubusercontent.com/frk1/hazedumper/master/csgo.json", "addres.json", 0, 0); }

int read_file(const char* clss, const char* addres_info)
{
	std::ifstream file{ "addres.json" };
	bool file_status = file.is_open();

	if (file_status)
	{
		nlohmann::json offset = nlohmann::json::parse(file);
		file.close();

		DWORD addres = offset[clss][addres_info];

		return addres;
	}

	else
	{
		MessageBox(0, "Не удалось открыть файл (OFFSETS).", "Внимание!", MB_OK);
		exit(0);
	}
}

void get_offsets()
{
	load_file();

	dwClientState = read_file("signatures", "dwClientState");
	model_ambient_min = read_file("signatures", "model_ambient_min");

	m_iTeamNum = read_file("netvars", "m_iTeamNum");
	m_iGlowIndex = read_file("netvars", "m_iGlowIndex");
	m_iHealth = read_file("netvars", "m_iHealth");
	m_clrRender = read_file("netvars", "m_clrRender");
	m_fFlags = read_file("netvars", "m_fFlags");
	m_flFlashMaxAlpha = read_file("netvars", "m_flFlashMaxAlpha");
	m_iCrosshairId = read_file("netvars", "m_iCrosshairId");
	m_bSpotted = read_file("netvars", "m_bSpotted");
	m_iDefaultFOV = read_file("netvars", "m_iDefaultFOV");

	m_bUseCustomAutoExposureMin = read_file("netvars", "m_bUseCustomAutoExposureMin");
	m_bUseCustomAutoExposureMax = read_file("netvars", "m_bUseCustomAutoExposureMax");

	m_flCustomAutoExposureMin = read_file("netvars", "m_flCustomAutoExposureMin");
	m_flCustomAutoExposureMax = read_file("netvars", "m_flCustomAutoExposureMax");

	m_hMyWeapons = read_file("netvars", "m_hMyWeapons");
	m_iItemDefinitionIndex = read_file("netvars", "m_iItemDefinitionIndex");
	m_OriginalOwnerXuidLow = read_file("netvars", "m_OriginalOwnerXuidLow");
	m_iItemIDHigh = read_file("netvars", "m_iItemIDHigh");
	m_nFallbackPaintKit = read_file("netvars", "m_nFallbackPaintKit");
	m_iAccountID = read_file("netvars", "m_iAccountID");
	m_nFallbackStatTrak = read_file("netvars", "m_nFallbackStatTrak");
	m_nFallbackSeed = read_file("netvars", "m_nFallbackSeed");

	m_totalHitsOnServer = 0x103F8;

	remove_file();
}