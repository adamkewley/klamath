RIX
===

In Fallout 2 there are six RIX files. All are in master.dat, in the
art:raw-latex:`\splash `folder, and appear when loading the game.

== RIX File Structure ==

Files with the RIX extension appear to be a kind of ColorRIX Image File.
ColorRIX is an outdated graphics editor for VGA and SVGA cards, running
under MS-DOS. There were about half a dozen formats, of which Fallout 2
only used one - SCF (renamed RIX). ColorRIX came with some VGA graphics
cards and a description of the format can be found on some sites with
FTP archives.

This format lets/allows us to save uncompressed images in 640x480
resolution, 256 colors (indexed). It's analogous to the Microsoft
Windows Bitmap File format (BMP).

All of the control values it the file are stored in little-endian
format. (I.e. least-significant byte first)

{\| class="wikitable" border="1" ! Offset !! Size !!Description \|- \|
0x0000 \|\| 4 \|\| "RIX3" - the file signature. \|- \| 0x0004 \|\| 2
\|\| 0x280 = 640 - width of the image. \|- \| 0x0006 \|\| 2 \|\| 0x1E0 =
480 - height of the image. \|- \| 0x0008 \|\| 1\|\| 0xAF = VGA - Palette
type. \|- \|0x009 \|1 \|0x00 = linear - Storage type. \|- \| 0x000A \|\|
256\ *3 = 768 \|\| The palette. \|- \| 0x030A \|\| 640*\ 480 = 307200
\|\| The image data. \|} === Header ===

The header consists of ten bytes. We'll use some C-style code to
describe the structure - this isn't what's actually written in the file.

.. raw:: html

   <pre>
       struct RIXheader
       {
           char signature[4];  //file signature (RIX3 in ASCII)
           WORD Width;         //width of the image (640 pixels, 0x280)
           WORD Height;        //height of the image (480 pixels, 0x1E0)
           CHAR PaletteType;   //typically 0xAF, for VGA
           CHAR StorageType;   //typically 0x00, linear
       }
   </pre>

Immediately after the header is the palette.

=== Palette ===

The palette contains RGB color codes used in the picture, and consists
of 256 values (256 colors), three bytes for each. Therefore, the palette
is 256\*3=768 bytes and starts at offset 0xA in the RIX file.

.. raw:: html

   <pre>
       struct Palette
       {
           RGB entries[256];
       }

       struct RGB
       {
           unsigned char Red;  // Intensity (0..255) of the red component.
           unsigned char Green;    // The same, for green.
           unsigned char Blue;     // The same, for blue.
       }
   </pre>

Only the first six bits of each byte contain values, so the color depth
is 18-bit. When trying to display the image in 24-bit color, it will
look dark, so each byte must be shifted two bits to the left (<< 2).
After the palette is the image data.

=== Data ===

The data occupies 640\*480 = 307200 bytes and starts at offset 0x30A in
the RIX file. The data is the bytes that comprise the actual image. One
element of data, corresponding to one pixel on the screen, occupies one
byte. The value of this byte is the serial number (index) of the color
in the palette, made of 256 pieces. For example, if a byte in the image
is 0x5, for the representation of that pixel on the screen you need to
take the fifth color from the palette; i.e.

.. raw:: html

   <pre>
       5(color number) * 3(number of bytes for a single color in the palette) = 15.
   </pre>

In the palette, at offset 15 (0xF) from the start, are three bytes
indicating the intensities of red, green, and blue light at that pixel
respectively. Pixels are stored in order left to right, top to bottom on
the screen.

==Legal stuff==

.. raw:: html

   <pre>ColorRIX format is described in the manual for the program "ColorRIX VGA Paint". Last known address of
   RIX Softworks (program developer):
   RIX SoftWorks Inc. 
   Attn: Richard Brownback or Paul Harker 
   18023 Sky Park Circle, Suite J 
   Irvine, CA 92714 
   Voice: 714.476.8266 
   Voice: 714.476.8486</pre>

[http://www.teamx.ru/rus/bd/rix\_format.shtml Original document] at
teamx.ru (in russian) [[Category:Fallout and Fallout 2 file
formats]][[Category:Translated TeamX documents]]
