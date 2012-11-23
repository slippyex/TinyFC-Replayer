;
; ------------------------------------------------------------
;
;   PureBasic - Tiny FC Library
;
;    (c) 2007 - SLiPPY/VeCTRONiX!
;
; ------------------------------------------------------------
;

ImportC("tiny_fc.lib")
 FC_Initialize(inMem, pMusicBuffer, inMemSize, filename$)
 FC_StartPlayer()
 FC_StopPlayer() 
EndImport

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

FC_StopPlayer()
CloseLibrary(0)

; IDE Options = PureBasic v4.02 (Windows - x86)
; ExecutableFormat = Console
; CursorPosition = 10
; Folding = -
; Executable = play_fc_static.exe
; DisableDebugger