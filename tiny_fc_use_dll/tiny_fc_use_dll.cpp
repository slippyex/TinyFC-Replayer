// we need: windows...
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "dsound.lib")

// DLL function signatures
typedef int (*importInitialize)(bool, void*, int, const char*);
typedef int (*importStart)();
typedef int (*importStop)();
static importInitialize FC_Initialize;
static importStart FC_StartPlayer;
static importStop FC_StopPlayer;

static HINSTANCE hinstLib;

int loadEngineDLL() {
	
    // Load DLL file
    hinstLib = LoadLibrary(TEXT("tiny_fc.dll"));
    if (hinstLib == NULL) {
		printf("tiny_fc.dll not found");
        return -1;
    }

    // Get function pointer
	FC_Initialize = (importInitialize)GetProcAddress(hinstLib, "FC_Initialize");
	FC_StartPlayer = (importStart)GetProcAddress(hinstLib, "FC_StartPlayer");
	FC_StopPlayer = (importStop)GetProcAddress(hinstLib, "FC_StopPlayer");

    if (FC_Initialize == NULL) {
		FreeLibrary(hinstLib);
		printf("error while trying to initialize dll functions");
        return -1;
    }
	return 1;
}
//extern "C" void mainCRTStartup()
int main(int na, char **a)
{
	int iResult = loadEngineDLL();
	if(iResult > 0) {
		printf("Tiny FutureComposer player (DLL version)\nv0.97 (2007/10/04) by SLiPPY/VeCTRONiX! ...\n");
		// start playing
		FC_Initialize(false,  NULL, 0, "sample.fc");
		FC_StartPlayer();
		printf("\n\npress ESC to quit\n");
		while (GetAsyncKeyState(VK_ESCAPE)>=0) 
		{ 
			Sleep(20);
		}
		// stop and deinit the player
		FC_StopPlayer();
		// Unload DLL file
		FreeLibrary(hinstLib);
	}
	return 0;
}

