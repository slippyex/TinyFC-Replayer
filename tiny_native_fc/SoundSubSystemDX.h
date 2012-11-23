/*
 *                      OldSkoolMusaxxsEngine - API
 *      Copyright (C) 2007 SLiPPY/VeCTRONiX! <slippy@vectronixhq.de>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
#ifndef __DXSOUNDSUBSYSTEM__
#define __DXSOUNDSUBSYSTEM__

#include <dsound.h>

#define	DXREPLAY_RATE				44100
#define	DXREPLAY_DEPTH				16
#define	DXREPLAY_SAMPLESIZE			(DXREPLAY_DEPTH/4)
#define	DXREPLAY_BUFFERLEN			(16*1024)

typedef void (*DXUSER_CALLBACK) (void *pBuffer,long bufferLen);

class	CDXSoundSubSystem {
public:
		CDXSoundSubSystem();
		~CDXSoundSubSystem();
		bool	open(DXUSER_CALLBACK pUserCallback, long totalBufferLen);
		void	close(void);
		bool	update(void);
private:
		void				reset(void);
		DXUSER_CALLBACK		m_pUserCallback;
		LPDIRECTSOUND8		m_pDS8;
		HWND				m_hWnd;
		LPDIRECTSOUNDBUFFER	m_pPrimary;
		LPDIRECTSOUNDBUFFER	m_pBuffer;
		HANDLE				m_hThread;
		HANDLE				m_hFadeOutThread;
		DWORD				m_writePos;
		volatile	bool	m_bThreadRunning;
		volatile	bool	m_bThreadKillRequired;

};

#endif
