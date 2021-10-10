#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_impl_win32.h"
#include "service\include.h"

#include <dinput.h>
#include <tchar.h>
#include <dwmapi.h>
#include <d3d9.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d9.lib")


#define DIRECTINPUT_VERSION 0x0800
#define LOADERW 600
#define LOADERH 350

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Maze", NULL };
    ::RegisterClassExW(&wc);
    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - LOADERW / 2;
    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - LOADERH / 2;
    HWND hwnd = ::CreateWindowExW(WS_EX_LAYERED, wc.lpszClassName, L"Maze", WS_POPUP, x, y, LOADERW, LOADERH, NULL, NULL, wc.hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    static bool opened = true;

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    int tab_index = 0;

    get_addres_point();
    pattern_start();
    get_offsets();

    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(LOADERW, LOADERH));

            ImGui::Begin("Maze", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

            if(ImGui::Button("Visual")) {tab_index = 1;}
            ImGui::SameLine(0.f, 5.f);

            if(ImGui::Button("TriggerBot")) { tab_index = 2;}
            ImGui::SameLine(0.f, 5.f);

            if(ImGui::Button("Misc")) { tab_index = 3;}
            ImGui::SameLine(0.f, 5.f);

            if(ImGui::Button("Cvar")) { tab_index = 4;}
            ImGui::SameLine(0.f, 5.f);



            if (tab_index == 1) // Visual
            {
                ImGui::NewLine();

                if (ImGui::CollapsingHeader("Glow ESP"))
                {
                    if (ImGui::Checkbox("Enable default", &GlowESP_default_status))
                    {
                        GlowESP_base_HP_status = false;

                        std::thread ESP_def(GlowESP_default);
                        ESP_def.detach();
                    }

                    if (ImGui::Checkbox("Enable HP base", &GlowESP_base_HP_status))
                    {
                        GlowESP_default_status = false;

                        std::thread ESP_HP(GlowESP_base_HP);
                        ESP_HP.detach();
                    }

                    ImGui::ColorEdit3("Color [ESP]", ESP_color);
                    ImGui::SliderFloat("Line", &ESP_Line, 0.3f, 1.f);
                }

                if (ImGui::CollapsingHeader("Chams"))
                {
                    if (ImGui::Checkbox("Enable", &Chams_status))
                    {
                        if (Chams_status)
                        {
                            std::thread Chams_(Chams);
                            Chams_.detach();
                        }

                        else { Chams_Reset(); }
                    }

                    ImGui::ColorEdit3("Color [CHAMS]", Chams_color);
                }

                if (ImGui::CollapsingHeader("FOV"))
                {
                    if (ImGui::SliderInt("Value [FOV] (default = 90)", &FOV_values, 10, 170)) { FOV(); }
                }

                if (ImGui::CollapsingHeader("Nigth Mod"))
                {
                    if (ImGui::SliderFloat("Value [NIGTH MOD]", &Nigth_mod_value, 0.010f, 1.5f)) { Nigth_Mod(); }
                }
            }

            if (tab_index == 2) // Trigger Bot
            {
                ImGui::NewLine();

                if (ImGui::Checkbox("Enabel", &TriggetBot_default_status))
                {
                    TriggerBot_P_status = false;

                    std::thread Trigger_D(TriggerBot_default);
                    Trigger_D.detach();
                }

                if (ImGui::Checkbox("Enabel [improved]", &TriggerBot_P_status))
                {
                    TriggetBot_default_status = false;

                    std::thread Trigger_P(TriggerBot_P);
                    Trigger_P.detach();
                }

                ImGui::SliderInt("Delay [m/s]", &TriggerBot_delay, 10, 1000);
            }

            if (tab_index == 3)
            {
                ImGui::NewLine();

                if (ImGui::Checkbox("Auto pistol", &Auto_Pistol_Status))
                {
                    std::thread AutoP(Auto_Pistol);
                    AutoP.detach();
                }

                if (ImGui::Checkbox("Bunny Hop", &BHop_status))
                {
                    std::thread BP(BHop);
                    BP.detach();
                }

                if (ImGui::Checkbox("Shit Hit sound", &Hit_Sound_status))
                {
                    std::thread Hit_sound(Hit_Sound);
                    Hit_sound.detach();
                }

                if (ImGui::Checkbox("Skinchanger", &Skin_changer_status))
                {
                    std::thread Skin(Skin_changer);
                    Skin.detach();
                }

                if (ImGui::Checkbox("Radar Hack", &Radar_Hack_Status))
                {
                    std::thread Radar_H(Radar_Hack);
                    Radar_H.detach();
                }

                if (ImGui::Checkbox("Model Ambien", &r_modelAmbientMin_status))
                {
                    r_modelAmbientMin();
                }

                if (ImGui::CollapsingHeader("Anti flash"))
                {
                    if (ImGui::Checkbox("Enabel", &Anti_Falsh_status))
                    {
                        if (Anti_Falsh_status)
                        {
                            std::thread AF(Anti_Flash);
                            AF.detach();
                        }

                        else { Anti_Flash_Reset(); }
                    }

                    ImGui::SliderInt("Value [ANTI FLASH]", &FlashMaxAlpha, 10, 255);
                }
            }

            if (tab_index == 4)
            {
                ImGui::NewLine();

                if(ImGui::Checkbox("sv_cheats", &sv_cheats_status))
                {
                    if (sv_cheats_status) { sv_cheats_true(); }
                    else { sv_cheats_false(); }
                }

                if (ImGui::Checkbox("r_drawothermodels", &r_drawothermodels_status))
                {
                    if (r_drawothermodels_status) { r_drawothermodels_true(); }
                    else { r_drawothermodels_false(); }
                }

                if (ImGui::Checkbox("mat_showlowresimage", &mat_showlowresimage_status))
                {
                    if (mat_showlowresimage_status) { over_slow_texture_true(); }
                    else { over_slow_texture_false(); }
                }

                if (ImGui::Checkbox("cl_grenadepreview", &cl_grenadepreview_status))
                {
                    if (cl_grenadepreview_status) { nade_helper_true(); }
                    else { nade_helper_false(); }
                }

                if (ImGui::CollapsingHeader("r_aspectratio"))
                {
                    if (ImGui::SliderFloat("Value [ASPECTRATIO]", &r_aspectratio_value, 0.1f, 2.f))
                    {
                        r_aspectratio(r_aspectratio_value);
                    }
                }
            }

            ImGui::End();
        }

        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();

        if (!opened) msg.message = WM_QUIT;
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

POINTS m_Pos;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_LBUTTONDOWN:
    {
        m_Pos = MAKEPOINTS(lParam);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (wParam == MK_LBUTTON)
        {
            POINTS p = MAKEPOINTS(lParam);
            RECT rect;
            GetWindowRect(hWnd, &rect);
            rect.left += p.x - m_Pos.x;
            rect.top += p.y - m_Pos.y;
            if (m_Pos.x >= 0 && m_Pos.x <= LOADERW - 20 && m_Pos.y >= 0 && m_Pos.y <= ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f)
                SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
        }
        return 0;
    }
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}