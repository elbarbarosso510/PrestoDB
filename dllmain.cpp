#include <Windows.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <bitset>
#include "mono.h"
#include "emono.h"
#include <mutex>
#include "GameEngine.h"
#include "Globals.h"
typedef struct RES_INFO
{
    BYTE k;
    char hash[25];
    UINT16 data_len;
    unsigned char data[1];
};
mutex _thread_locker;
List<RES_INFO*> infos = List<RES_INFO*>();
GameWorld* world = nullptr;
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define __loop(x) for(int TOKENPASTE2(__val__, __LINE__)=0;TOKENPASTE2(__val__, __LINE__)<x;TOKENPASTE2(__val__, __LINE__)++)
#define __for(x,v) for(int v=0;v<x;v++)
#define CHECK_TIMEOUT(ms,code)\
static ULONGLONG TOKENPASTE2(__val__, __LINE__) = 0; \
ULONGLONG TOKENPASTE2(__val_1__, __LINE__) = GetTickCount64();\
if (TOKENPASTE2(__val_1__, __LINE__) - TOKENPASTE2(__val__, __LINE__) > ms)\
 {  TOKENPASTE2(__val__, __LINE__) = TOKENPASTE2(__val_1__, __LINE__);code}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        InitMono();
        Globals::InitProcs();
        auto SHARP = new CSharp();
    rep:;
        while (true)
        {
            if (GetAsyncKeyState(VK_UP))
            {
                auto scenes = LocationScene::LoadedScenes();
                if (scenes)
                {
                    for (int i = 0; i < scenes->Count(); i++)
                    {
                        auto scene = scenes->get(i);
                        auto BorderZones = scene->BorderZones;
                        if (BorderZones->IsVaild())
                        {
                            for (int j = 0; j < BorderZones->Count(); j++)
                            {
                                auto zone = BorderZones->get(j);
                                auto pos3 = zone->gameObject()->transform()->position();
                                printf("{%.3f,%.3f,%.3f};\n", pos3.x, pos3.y, pos3.z);
                            }
                        }
                    }
                }
            }
            Sleep(500);
        }
        goto rep;
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
