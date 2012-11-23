' Fireball

defint a - z

#include "tinyptc.bi"
#Include Once "tiny_fc.bi"
#Include "tune.bi"

const scrw = 320
const scrh = 240

if ptc_open("Fireball",scrw,scrh) = 0 then end

declare sub setupfireball
declare sub fireball

dim shared buffer(scrw * scrh)

dim shared pal(256)
dim shared tex(128,128)
dim shared cmap(128,128)
dim shared dmap(scrw shl 1,scrh shl 1) as double
dim shared amap(scrw shl 1,scrh shl 1) as double

dim shared fbox = scrw shr 1
dim shared fboy = scrh shr 1
dim shared cmapy

'initFC(0, @dummy, 0, "sample.fc")
initFC(1, @tune(0), 7804, "")
setupfireball
startFC()

Do
    fireball
    ptc_update @buffer(0)
loop until inkey$ = chr$(27)
stopFC()

End

sub setupfireball
dim r,g,b'dim r = 255,g = 255,b = 255
dim u,d,l
dim xd as double,yd as double
for i = 0 to 255
    if i <= 85 then
        r = i * 3
        g = i * 1.5
    elseif i <= 170 then
        i2 = i - 85
        g = 127 + (i2 * 1.5)
        b = i2 * 1.5
    else
        i2 = i - 170
        b = 127 + (i2 * 1.5)
    end if
'    if i <= 72 then
'        r = r - 3
'        g = r
'        b = r
'    else
'        r = r + 4
'        g = g + 2
'        b = b + 1
'    end if
'    if r < 0 then r = 0
'    if g < 0 then g = 0
'    if b < 0 then b = 0
'    if r > 255 then r = 255
'    if g > 255 then g = 255
'    if b > 255 then b = 255
    pal(i) = (r shl 16) + (g shl 8) + b
next i
for y = 0 to 126
    for x = 0 to 127
        cmap(x,y) = rnd * 5.0
    next x
next y
'for i = 0 to 1
    for y = 0 to 126
        u = (y - 1) and 127
        d = (y + 1) and 127
        for x = 0 to 127
            l = (x - 1) and 127
            r = (x + 1) and 127 
            cmap(x,y) = (cmap(l,y) + cmap(r,y) + cmap(x,u) + cmap(x,d)) shr 2
        next x
    next y
'next i
for y = 0 to (scrh shl 1) - 1
    yd = cdbl(y - scrh)
    for x = 0 to (scrw shl 1) - 1
        xd = cdbl(x - scrw)
        dmap(x,y) = sqr(xd * xd + yd * yd)
        amap(x,y) = atan2(yd,xd)
    next x
next y
end sub

sub fireball
dim ox,oy,cmy,u,d,l,r,c
dim an as double,s as double,dis as double
an = cdbl(cmapy) / 128.0 * 6.28 * .2
s = 1.25 + cos(an * .43)
fbox = (scrw shr 1) + (sin(an) * (scrw * .3))
fboy = (scrh shr 1) + (sin(an * .67) * (scrh * .28))
ox = scrw - fbox
oy = scrh - fboy
for i = 0 to 127
    tex(i,127) = 255'rnd * 255
next i
for y = 1 to 126
    u = (y - 1) and 127
    d = (y + 1) and 127
    cmy = (cmapy + y) and 127
    for x = 0 to 127
        l = (x - 1) and 127
        r = (x + 1) and 127
        c = ((tex(l,y) + tex(r,y) + tex(x,u) + tex(x,d)) shr 2) - cmap(x,cmy)
        if c < 0 then c = 0
        tex(x,y) = c
        tex(x,y - 1) = c
    next x
next y
for y = 0 to scrh - 1
    yi = y * scrw
    for x = 0 to scrw - 1
        dis = dmap(ox + x,oy + y) * s
        if dis < 0 or dis > 127.0 then
            buffer(yi + x) = pal(0)
        else
            buffer(yi + x) = pal(tex(((amap(ox + x,oy + y) + an) * 81.52) and 127,127 - cint(dis)))
        end if
    next x
next y
cmapy = cmapy + 1
end sub