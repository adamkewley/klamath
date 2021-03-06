===
FON
===

Fallout FON-files format.

This document describes FON- format files. The information was obtained
in the course of reviewing relevant functions in the file Mapper2.exe.
The names of the structures and their fields used in the program were
received from the debugging information in the file Mapper2.exe.

==Background==

Fonts from FON files are used for displaying information on the
[http://www.fallout.wikia.com/wiki/Fallout\_locations Fallout] and
[http://www.fallout.wikia.com/wiki/Fallout\_2\_locations Fallout 2]
worldmap (according to information provided by Wasteland Ghost). All
fonts are non-scalable, raster, and do '''''not''''' contain any
anti-aliasing information.

==Structure== The FON file format is given in Table 2.1.

{\| border="1" ! Offset !! Size !! Type !! !! Description \|- \| 0x0000
\|\| 4 \|\| int \| rowspan="5" \| Header \| The number of character
images in the file. A shorthand notation '''num''' is used
below.'''Note:''' Some characters are not included because they weren't
included in the file(s). \|- \| 0x0004 \|\| 4 \|\| int \|\| The height
of the font in points \|- \| 0x0008 \|\| 4 \|\| int \|\| The distance
between adjacent symbols in points \|- \| 0x000C \|\| 4 \|\| FontInfo\*
\|\| Pointer to an array of structures that describe the characters
'''Note:''' This field is only important in the memory of the computer.
Inside the file, it has no meaning. \|- \| 0x0010 \|\| 4 \|\| unsigned
char\* \|\| Pointer to an array of images of each individual character's
data. '''Note:''' This field is only important in the memory of the
computer. Inside the file, it has no meaning. \|- \| 0x0014 \|\| 4 \|\|
int \| rowspan="5" \| An array of characters descriptors \| Symbol 0
width \|- \| 0x0018 \|\| 4 \|\| int \|\| Symbol 0 position in a block of
data (what is this "block of data"? in memory or in the file? please
explain) \|- \| 0x001C \|\| 4 \|\| int \|\| Symbol 1 width \|- \| 0x0020
\|\| 4 \|\| int \|\| Symbol 1 position in a block of data (what is this
"block of data"? in memory or in the file? please explain) \|- \| ...
\|\| ... \|\| ... \|\| ... \|- \| 0x0014 + 8 \* num \|\| 1 \|\| unsigned
char \| rowspan="2" \| \| rowspan="2" \| Characters' images data \|- \|
... \|\| ... \|\| ... \|}

To work with FON files, mapper2.exe and fallout2.exe use the following
structure:

===Header===

.. raw:: html

   <pre>
    typedef struct {
       int num;                // The number of images of characters in the file 
       int height;             // height of the font in points 
       int spacing;            // The distance between adjacent symbols at the points 
       FontInfo* info;             // Pointer to the array structures, describing characters 
       unsigned char* data;        // Pointer to the array structures, describing symbol)
    } Font; 
   </pre>

===Symbol descriptor===

.. raw:: html

   <pre>
    typedef struct {
       int width;      // width character's points
       int offset;     // The image characters in a block of data 
    } FontInfo; 
   </pre>

The size of the block of data depicting characters determined as
follows: last = font.num-1; // Index of last character in the font size
= font.info[last].offset + (font.info[last].width + 7) / 8 \*
font.height;

===Through Construction=== (font.info[last].width + 7) / 8 determined by
the number of bytes needed to store information on one line symbol
image. Information on the image stored as a symbol bit matrix.

===Example=== Let there is a symbol of a size 8x16 points of the image
which is described as follows block data 00 00 7e 81 a5 81 81 bd 99 81
81 7e 00 00 00 00 The number of bytes needed to store information on one
line symbol image bytesPerLine = (8 + 7) / 8 = 1 Bit matrix takes the
form of 00 00000000 ........ 00 00000000 ........ 7e 01111110 .######.
81 10000001 #......# a5 10100101 #.#..#.# 81 10000001 #......# 81
10000001 #......# bd ==> 10111101 ==> #.####.# 99 10011001 #..##..# 81
10000001 #......# 81 10000001 #......# 7e 01111110 .######. 00 00000000
........ 00 00000000 ........ 00 00000000 ........ 00 00000000 ........

==FON viewer source code== This program was written by ''Anchorite'',
and was compiled in MSVS 2003.NET.

.. raw:: html

   <pre>
   // Fallout FON-file viewer
   #include <stdio.h>
   #include <stdlib.h>

   #pragma pack(1)        // Struct memory alignment - 1 bytes

   typedef struct {
       int width;
       int offset;
   } FontInfo;

   typedef struct {
       int num;
       int height;
       int spacing;
       FontInfo* info;
       unsigned char* data;
   } Font;


   int main(int argc, char* argv[])
   {
       FILE* fp;
       Font font;
       int last;
       int size;
       int i;

       printf("Fallout FON-files viewer, version 1.0\n");
       printf("Copyright (C) Anchorite (TeamX), 2005\n");
       printf("anchorite2001@yandex.ru\n");
       printf("\n");

       if (argc < 2) {
           printf("Usage: %s file.fon\n", argv[0]);
           return 1;
       }

       // Read file
       fp = fopen(argv[1], "rb");

       if (!fp) {
           printf("Error: Unable open %s\n", argv[1]);
           return -1;
       }


       if (fread(&font, sizeof(font), 1, fp) != 1) {
           printf("Error: Unable read FON-file header\n");
           fclose(fp);
           return -1;
       }

       font.info = malloc(font.num * sizeof(FontInfo));

       if (font.info == NULL) {
           printf("Error: Unable allocate memory for glyphs info\n");
           fclose(fp);
           return -1;
       }

       if (fread(font.info, sizeof(FontInfo), font.num, fp) != font.num) {
           printf("Error: Unable read info about glyphs\n");
           free(font.info);
           fclose(fp);
           return -1;
       }

       last = font.num - 1;
       size = font.info[last].offset + (font.info[last].width + 7) / 8 * font.height;

       font.data = malloc(size);

       if (font.data == NULL) {
           printf("Error: Unable allocate memory for glyphs\n");
           free(font.info);
           fclose(fp);
           return -1;
       }

       if (fread(font.data, 1, size, fp) != size) {
           printf("Error: Unable read glyphs\n");
           free(font.data);
           free(font.info);
           fclose(fp);
           return -1;
       }

       fclose(fp);

       // Font info
       printf("Number of glyphs: %d\n", font.num);
       printf("Height: %d\n", font.height);
       printf("Spacing: %d\n", font.spacing);
       printf("\n");

       // Glyphs
       for(i = 0; i < font.num; i++) {
           printf("Glyph %d (0x%02X)\n", i, i);
           printf("================\n");

           printf("Width: %d\n", font.info[i].width);
           printf("Offset: 0x%08X (%d)\n", font.info[i].offset, font.info[i].offset);
           printf("Offset from begin of file: 0x%08X (%d)\n",
                  sizeof(font) + sizeof(FontInfo) * font.num + font.info[i].offset,
                  sizeof(font) + sizeof(FontInfo) * font.num + font.info[i].offset);
           printf("\n");

           if (font.height * font.info[i].width != 0) {
               int offset = font.info[i].offset;
               int bytesPerLine = (font.info[i].width + 7) / 8;
               int j;
               int h;

               for(h = 0; h < font.height; h++) {
                   for(j = 0; j < font.info[i].width; j++) {
                       if (font.data[offset + h * bytesPerLine + (j / 8)] & (1 << (7 - (j % 8)))) {
                           printf("#");
                       }
                       else {
                           printf(".");
                       }
                   }

                   printf("\n");
               }

               printf("\n");
               printf("\n");
           }
           else {
               printf("Empty\n");
               printf("\n");
               printf("\n");
           }
       }

       // Free font data
       free(font.data);
       free(font.info);

       return 0;
   }
   </pre>

===Program update=== The following code was 99.9% based off of
Anchorite's previous version. I ([[User:Ghouly89\|Ghouly89]]) have
simply taken the liberty to update the code, and make this program more
user-friendly. I do not claim any credit for the FON-viewer source code.
This program was compiled in Dev-Cpp 4.9.9.2.

.. raw:: html

   <pre>
   // Fallout FON-file viewer
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>

   #pragma pack(1)        // Struct memory alignment - 1 bytes

   /* 
      Changes by Ghouly89:
          
          * Removed argument counter / argument string array variables so user can deal straight through the program console
          * Added character string "fileName" to hold the .FON file name to be opened and read
          * Made all text output to a local (same directory) file titled "font.txt"
   */

   typedef struct {
       int width;
       int offset;
   } FontInfo;

   typedef struct {
       int num;
       int height;
       int spacing;
       FontInfo* info;
       unsigned char* data;
   } Font;


   int main()
   {
       FILE* fontFile;
       FILE* fontText;
       Font font;
       int last;
       int size;
       int i;
       char fileName[100];

       system("TITLE Fallout font viewer");
       system("COLOR 0A");
       
       printf("Enter the name of the .FON file to open (output can be found in font.txt) : ");
       printf("\n\n\n\t");
       gets(fileName);

       fontText = fopen("font.txt", "w");
       
       fprintf(fontText, "Fallout FON-files viewer, version 1.0\n");
       fprintf(fontText, "Copyright (C) Anchorite (TeamX), 2005\n");
       fprintf(fontText, "anchorite2001@yandex.ru\n");
       fprintf(fontText, "\n");

       // Read file
       fontFile = fopen(fileName, "rb");
       

       if (!fontFile) {
           fprintf(fontText, "Error: Unable to open %s\n", fileName);
           return -1;
       }


       if (fread(&font, sizeof(font), 1, fontFile) != 1) {
           fprintf(fontText, "Error: Unable to read FON-file header\n");
           fclose(fontFile);
           fclose(fontText);
           return -1;
       }

       font.info = malloc(font.num * sizeof(FontInfo));

       if (font.info == NULL) {
           fprintf(fontText, "Error: Unable to allocate memory for glyphs info\n");
           fclose(fontFile);
           fclose(fontText);
           return -1;
       }

       if (fread(font.info, sizeof(FontInfo), font.num, fontFile) != font.num) {
           fprintf(fontText, "Error: Unable to read info about glyphs\n");
           free(font.info);
           fclose(fontFile);
           fclose(fontText);
           return -1;
       }

       last = font.num - 1;
       size = font.info[last].offset + (font.info[last].width + 7) / 8 * font.height;

       font.data = malloc(size);

       if (font.data == NULL) {
           fprintf(fontText, "Error: Unable to allocate memory for glyphs\n");
           free(font.info);
           fclose(fontFile);
           fclose(fontText);
           return -1;
       }

       if (fread(font.data, 1, size, fontFile) != size) {
           fprintf(fontText, "Error: Unable to read glyphs\n");
           free(font.data);
           free(font.info);
           fclose(fontFile);
           fclose(fontText);
           return -1;
       }

       fclose(fontFile);

       // Font info
       fprintf(fontText, "Number of glyphs: %d\n", font.num);
       fprintf(fontText, "Height: %d\n", font.height);
       fprintf(fontText, "Spacing: %d\n", font.spacing);
       fprintf(fontText, "\n");

       // Glyphs
       for(i = 0; i < font.num; i++) {
           fprintf(fontText, "Glyph %d (0x%02X)\n", i, i);
           fprintf(fontText, "================\n");

           fprintf(fontText, "Width: %d\n", font.info[i].width);
           fprintf(fontText, "Offset: 0x%08X (%d)\n", font.info[i].offset, font.info[i].offset);
           fprintf(fontText, "Offset from beginning of file: 0x%08X (%d)\n",
                  sizeof(font) + sizeof(FontInfo) * font.num + font.info[i].offset,
                  sizeof(font) + sizeof(FontInfo) * font.num + font.info[i].offset);
           fprintf(fontText, "\n");

           if (font.height * font.info[i].width != 0) {
               int offset = font.info[i].offset;
               int bytesPerLine = (font.info[i].width + 7) / 8;
               int j;
               int h;

               for(h = 0; h < font.height; h++) {
                   for(j = 0; j < font.info[i].width; j++) {
                       if (font.data[offset + h * bytesPerLine + (j / 8)] & (1 << (7 - (j % 8)))) {
                           fprintf(fontText, "#");
                       }
                       else {
                           fprintf(fontText, ".");
                       }
                   }

                   fprintf(fontText, "\n");
               }

               fprintf(fontText, "\n");
               fprintf(fontText, "\n");
           }
           else {
               fprintf(fontText, "Empty\n");
               fprintf(fontText, "\n");
               fprintf(fontText, "\n");
           }
       }
       
       printf("\n\n");
       printf("Font data read to file.");
       getch();
       
       // Close the .FON file and the "font.txt" file
       fclose(fontFile);
       fclose(fontText);

       // Free font data
       free(font.data);
       free(font.info);

       return 0;
   }
   </pre>

==External links== \* The original document can be found on
[http://www.teamx.ru/rus/bd/fon\_format.shtml teamX's website]. Original
document author: '''Anchorite'''. [[Category:Fallout and Fallout 2 file
formats]]
