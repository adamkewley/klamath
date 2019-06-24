===
PAL
===

This document describes the raw data format of the color palette (PAL)
file. The same format was used for both Fallout and Fallout 2, giving
all PAL files complete compatibility with both game engines.

==General Information== Information about PAL files was obtained by
various [[TeamX]] members through analyzing relevant functions in
MAPPER2.exe and FALLOUT2.exe, as well as trying to implement a simple
FRM display in a program.

==Technical Information== PAL files contain a palette used when
rendering FRM frame data. Additionally, they contain a table for
transforming RGB values into an index in the palette (speculated as
being used for extracting fonts), and also several tables concatenated
beyond the main file data whose uses are not clear.

===Required Fields=== As referenced below, the '''Palette''' area
(0x00000000 - 0x000002FF) and the '''RBG Conversion Table''' area
(0x00000300 - 0x000082FF) are required elements in PAL files.

===.exe-Generated Data=== The purpose of the '''.exe-Generated Data'''
is not clear. When loading PAL files, if these tables don't exist
FALLOUT2.exe and MAPPER2.exe will generate them.

Also note that values contained in custom PAL files provided for
individual FRM files are subject to certain changes in the game and
mapper engines.

==PAL data== {\| bgcolor="#ffffff" border="1" cellpadding="5"
cellspacing="0" style="text-align:center" ! bgcolor="#f0f0ff" \|Offset !
bgcolor="#f0f0ff" \|Size ! bgcolor="#f0f0ff" \|Type ! bgcolor="#f0f0ff"
\|Value ! bgcolor="#f0f0ff" colspan="2" \|Description \|- \|0x00000000
\|1 \|unsigned byte \| 0..63 \|Red000 \| rowspan="10" \|'''Palette'''
'''Colors in the palette are in the range 0..63'''. The first element
(index 0) is always considered transparent (actual color is ignored). If
the value of one color component is not in the range 0..63, in a special
table the index is marked as unused, and all components of the color are
set to 0.

Elements from 229 to 254 are used as 'animation colors' and Fallout sets
their values itself. 229 to 232 are animated green (for radioactive
waste), 238 to 247 are orange, red and yellow (for fires), and the rest
are bright blue (computer screens).

Since normal RGB goes from 0..255, using palette colors (0..63) directly
to draw the image will make it very dark. The easiest way to get correct
colors is to multiply the value by 4, so that a palette entry of "1,2,3"
actually becomes "4,8,12" in RGB. \|- \|0x00000001 \|1 \|\|--"--
\|\|--"-- \|Green000 \|- \|0x00000002 \|1 \|\|--"-- \|\|--"-- \|Blue000
\|- \|0x00000003 \|1 \|\|--"-- \|\|--"-- \|Red001 \|- \|0x00000004 \|1
\|\|--"-- \|\|--"-- \|Green001 \|- \|0x00000005 \|1 \|\|--"-- \|\|--"--
\|Blue001 \|- \|... \|... \|... \|... \|... \|- \|0x000002FD \|1
\|\|--"-- \|\|--"-- \|Red255 \|- \|0x000002FE \|1 \|\|--"-- \|\|--"--
\|Green255 \|- \|0x000002FF \|1 \|\|--"-- \|\|--"-- \|Blue255 \|-
\|0x00000300 \|1 \|unsigned byte \|0..255 \|Element00000 \| rowspan=3
\|'''RGB Conversion Table''' Table for converting RGB values => index in
the palette. \|- \|... \|... \|... \|... \|... \|- \|0x000082FF \|1
\|unsigned byte \|0..255 \|Element32767 \|- \| align="center"
bgcolor="#343434" colspan="6" style="color:#ffffff" \| '''.exe-Generated
Data''' \|- \|0x00008300 \|4 \|char[4] \|'NEWC' \| \|Tag for the
availability of additional tables.? \|- \|0x00008304 \|65536
\|byte[65536] \| \| \|Additional table 1 \|- \|0x00018304 \|65536
\|byte[65536] \| \| \|Additional table 2 \|- \|0x00028304 \|65536
\|byte[65536] \| \| \|Additional table 3 \|}

==External Links== [http://www.teamx.ru/rus/bd/pal\_format.shtml
Original document] at teamx.ru (in russian) [[Category:Fallout and
Fallout 2 file formats]][[Category:Translated TeamX documents]]


# pal spec

Used to store color palettes.

# `data`

```text

value of rn/gn/bn = 0 <= x < 64
multiply raw value ([0,64)) with 4 to get "close enough" colors
0 = transparent

animation colors:
  229 <= x <= 232 == green animation (radioactive waste)
  232 < x < 238  == bright blue (computer)
  238 <= x <= 247 == orange, red, yellow (fires)
  247 < x <= 254 == bright blue (computer)


    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |       r1      |      g1       |      b1       |       r2      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |       b2      |      g2       |      rn       |       bn      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |       gn      |    ..etc..    |   ..until..   |      r255     | : 0x000002fd
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |      g255     |      b255     |   element_1   |   element_2   | : 0x00000301
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |   element_n   |    ..etc..    |   ..until..   | element_32767 | : 0x000082ff
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |      "N"      |      "E"      |      "W"      |       "C"     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                               .                               |
   |                               .                               |
   |                       .exe-Generated Data                     |
   |                    (unclear what it's used for)               |
   |                               .                               |
   |                               .                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```
