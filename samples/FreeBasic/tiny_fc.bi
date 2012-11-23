#inclib "dsound"
#inclib "tiny_fc"
#inclib "stdc++"

' use this to play in-mem music (return 0 on success)
declare function initFC cdecl alias "FC_Initialize" (byval inMem as integer, byval pMusic as any ptr, byval length as uinteger, byval filename as zstring ptr) as integer
' same as above - only file-based
declare function startFC cdecl alias "FC_StartPlayer" () as integer
declare function stopFC cdecl alias "FC_StopPlayer" () as integer
