=========
PAL FILES
=========

== Introduction == This article describes the structure of the
proprietery PAL palette file format that was used in Fallout 1 and
Fallout 2.

== Purpose of PAL files == Palette files provide a conversion table from
RGB values (called "rgb" in this article) to Fallout's color index
(called "colorIndex" in this article) and vice versa.

== Structure == \* 33536 bytes total, devided in two parts \* First 768
bytes: RGB values for colorIndexes 0 - 255. '''colorIndex >> rgb'''
conversion table \*\* colorIndex 0 serves as transparency colorIndex
\*\*\* If a FRM file contains colorIndex 0 in his data section the pixel
will be displayed transparent in the Fallout 1/2 engine \*\*\* All color
components for colorIndex 0 are set to 255 in the color.pal file \*\*
colorIndexes 1 - 228 contain real RGB values (rgb values between 0 ->
63) \*\* colorIndexes 229 - 254 contain fake values (255 for each color
component) and are used for [[#Animated\_colors\|animated colors]]
\*\*\* The real rgb values for these special colorIndexes are hardcoded
in ''Fallout2.exe'' (rgb values between 0 -> 255) \*\* colorIndex 255
seems to be ununsed in Fallout .FRM files and is set to 255 for each
color component. \* Last 32768 bytes: colorIndexes for a 32 \* 32 \* 32
cubic matrix of rgb values. '''rgb >> colorIndex''' conversion table

=== colorIndex >> rgb === The first 768 bytes of a PAL palette files are
simply an array of 256 R,G,B structures. For those who don't know, the
RGB color triplet represents the red, green and blue component of a
color. Each component uses a single byte and can only range from 0 to
63(dec) in brightness. These 768 first bytes are important if you want
to convert the colorIndexes of a .FRM file into useful RGB values to
display the image or manipulate it.

The first part of the palette file got this structure: colorIndex:0
(r,g,b); colorIndex:1 (r,g,b,); ...; colorIndex:255 (r,g,b,)

The Fallout engine used a special brightness modifier (called
'''brightnessMod''' in this article) to create a day / night lighting
system. So before the final rgb values were painted on the screen, they
were multiplied with the brightness modifier: 1 -> night 2 -> morning,
dawn 3 -> forenoon, afternoon 4 -> high noon

=== rgb >> colorIndex === All data after the first 768 bytes is the
'''rgb >> colorIndex''' conversion table. We assume that this table is
needed as Fallout used a special lighting system. What we can say for
sure is that you'll need this second part of the palette if you want to
create new .FRM images. If you create new images with image editing
software it will use the standard rgb palette. If you want to save it as
a .FRM file now the programme needs to find the best matching colorIndex
for the each single pixel.

== Animated colors == === General information === The color indexes from
229 to 254 are used for '''animated colors''' in Fallout; details about
it can be found in [[Pal\_animations#Animated\_colors\|a separate
article]]. The Fallout developers decided to hardcode the RGB values for
these animated colors in the ''Fallout2.exe'' even though there was
space for them in the ''color.pal''. They set the RGB values for the
color indexes 229-255 to FF(hex) / 255(dec) in the .pal file and read
the real RGB values just from the Fallout2.exe.

== Palette for Gimp == Follow [[color.pal\|this link]] to get a palette
file for the Gimp; save it as a .gpl file. It should be the exact
palette with Fallout colours — it's original hex values (here converted
to decimal RGB values) are ripped from FIFE source (palette.h), which is
ripped from frm2bmp.

== Links == \*
[http://www.teamx.ru/files/srcs/Fallout2\_Palette\_src.rar PAL animation
example programme for Win32]

[[Category:Fallout 2 documentation]][[Category:Fallout and Fallout 2
file formats]]
