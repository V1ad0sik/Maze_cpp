#pragma once

bool r_modelAmbientMin_status = false;
bool sv_cheats_status = false;
bool r_drawothermodels_status = false;
bool mat_showlowresimage_status = false;
bool cl_grenadepreview_status = false;
float r_aspectratio_value = 1.5f;

void r_modelAmbientMin() { writeMem<int>(engine + model_ambient_min, 255); }

void sv_cheats_true() { SetValueINT("sv_cheats", 1); }
void sv_cheats_false() { SetValueINT("sv_cheats", 0); }

void r_drawothermodels_true() { SetValueINT("r_drawothermodels", 2); }
void r_drawothermodels_false() { SetValueINT("r_drawothermodels", 1); }

void r_aspectratio(float value) { { SetValueFLOAT("r_aspectratio", value); } }

void over_slow_texture_true() { SetValueINT("mat_showlowresimage", 1); } 
void over_slow_texture_false() { SetValueINT("mat_showlowresimage", 0); }

void nade_helper_true() { SetValueINT("cl_grenadepreview", 1); }
void nade_helper_false() { SetValueINT("cl_grenadepreview", 0); }
