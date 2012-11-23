#ifndef _TINY
#include <stdio.h>
#endif

#include "../tiny_native_fc/fc.h"
#include "../tiny_native_fc/SoundSubSystemDX.h"
static int fcMusicSize;
static int result = 0;
static char * pInternalMusicBuffer;
static	CDXSoundSubSystem soundSubSys;
extern void mixerFillBuffer( void*, udword );
extern void mixerInit(); //udword); //,int,int,uword);
extern "C" __declspec(dllexport) int FC_Initialize(bool inMem, void* pMusicBuffer, int inMemSize, char* filename);
extern "C" __declspec(dllexport) int FC_StartPlayer();
extern "C" __declspec(dllexport) int FC_StopPlayer();

static void soundSubSysCallback(void *pBuffer,long bufferLen)
{
	char *pSample = (char*)pBuffer;
	mixerFillBuffer(pSample,bufferLen); 
}

#ifndef _TINY
static int loadFromFile(const char * fname, int * size) {
	FILE * pFile;
	long lSize;
	pFile = fopen ( fname , "rb" );
	if (pFile==NULL) {
		fputs ("File error",stderr); 
		return -1;
	}
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	*size = (int)lSize;
	rewind (pFile);

	// allocate memory to contain the whole file:
	pInternalMusicBuffer = (char*) malloc (sizeof(char)*lSize);
	if (pInternalMusicBuffer == NULL) {
		return -2;
	}

	// copy the file into the buffer:
	fread (pInternalMusicBuffer,1,lSize,pFile);
	// terminate
	fclose (pFile);
	return 0;
}
#endif

int FC_Initialize(bool inMem, void* pMusicBuffer, int inMemSize, char* filename) {
    mixerInit(); //,16,2,0);
#ifndef _TINY
	if(inMem == false) {
		// load FC module
		result = loadFromFile(filename, &fcMusicSize);
	} else {
		pInternalMusicBuffer = (char*)pMusicBuffer;
		fcMusicSize = inMemSize;
	}
#else
		pInternalMusicBuffer = (char*)pMusicBuffer;
		fcMusicSize = inMemSize;
#endif
	// and initialize the replayer
	if(!FC_init(pInternalMusicBuffer, fcMusicSize, 0, 0)) {
		return -1;
	}
	return result;
}

int FC_StartPlayer() {
	// fire background thread ...
	if(!soundSubSys.open(soundSubSysCallback,2500)) {
		return -1;
	}
}

int FC_StopPlayer() {
	soundSubSys.close();
	return 0;
}
