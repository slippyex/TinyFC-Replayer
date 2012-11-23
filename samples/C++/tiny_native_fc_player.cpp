#include <windows.h>
#include <stdio.h>
extern "C" int FC_Initialize(bool inMem, void* pMusicBuffer, int inMemSize, char* filename);
extern "C" int FC_StartPlayer();
extern "C" int FC_StopPlayer();

int main(int na, char **a)
{
	if(a[1] == NULL) {
		printf("usage ::\n%s [FC_MODULE.smod/fc]\n\n", a[0]);
		printf("as you didn't provide any fc filename now\nI'm going to use \"slipstream2.fc\" for you ;)\n\n\n");
		a[1] = "slipstream2.fc";
	}
	printf("tiny Future Composer library replayer v.0.97\nby SLiPPY/VeCTRONiX! rel. 10/04/2007\n");
	printf("=====================================\n");
	printf("Now playing %s...\n", a[1]);
	FC_Initialize(false, NULL, 0, a[1]);
	FC_StartPlayer();
	// that should do it ...
	while (GetAsyncKeyState(VK_ESCAPE)>=0) {
		Sleep(10);
	}
	FC_StopPlayer();
	return 0;
}

