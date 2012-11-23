'Option Explicit
'Option Static


#Include Once "tinyPTC.bi"
#Include Once "tiny_fc.bi"
#Include Once "Windows.bi"

Const XRES=640
Const YRES=480
Const ARES=XRES*YRES

Dim Shared ScreenBuffer( ARES )
Dim Shared dummy As Integer

initFC(0, @dummy, 0, "sample.fc")
startFC()

If( ptc_open( "testing", XRES, YRES ) = 0 ) Then
    End -1                                    
End If




Dim Key As String

While Key<>Chr(27)
    
    
    PTC_Update @ScreenBuffer(0)
    key=Inkey

Wend


PTC_Close()
stopFC()