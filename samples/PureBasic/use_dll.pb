;
; ------------------------------------------------------------
;
;   PureBasic - Tiny FC Library
;
;    (c) 2007 - SLiPPY/VeCTRONiX!
;
; ------------------------------------------------------------
;

ImportC("tiny_native_fc.lib")
 FC_Initialize(inMem, pMusicBuffer, inMemSize, filename)
 FC_StartPlayer()
 FC_StopPlayer() 
EndImport

'If OpenLibrary(0, "tiny_fc.dll")

'  *FC_Init = GetFunction(0, "FC_Initialize")
'  *FC_Start = GetFunction(0, "FC_StartPlayer")
'
'  *FC_Stop = GetFunction(0, "FC_StopPlayer")
  

'  If *FC_Init
'    CallFunctionFast(*FC_Init, false, NULL, 0, "sample.fc")
'    CallFunctionFast(*FC_Start);
    
  FC_Initialize(0, 0, 0, "sample.fc")
  FC_StartPlayer()
  If OpenConsole()
    PrintN("Tiny FC library by SLiPPY/VeCTRONiX!")
    PrintN("Press ESC To exit...")
  
    Repeat
      KeyPressed$ = Inkey()
      Delay(20)     
    Until KeyPressed$ = Chr(27) 
  EndIf

    
  EndIf
'  CallFunctionFast(*FC_Stop)
  FC_StopPlayer()
  CloseLibrary(0)
'EndIf
; IDE Options = PureBasic v4.02 (Windows - x86)
; ExecutableFormat = Console
; CursorPosition = 45
; Folding = -
; Executable = play_fc_static.exe
; DisableDebugger