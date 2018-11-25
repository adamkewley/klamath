===
AAF
===

The AAF Font File Format is used to store fonts in ''Fallouts 1 & 2''
Only the AAF files used in the English versions are discussed here,
there may or may not be variations for other languages.

The AAF file stores a separate glyph for each of the 256 position in
the ASCII table. The fonts in an AAF file are typically non-fixed
size, i.e.  each glyph has it's own width and height. A glyph is
defined by a rectangular bitmap representing the pixels of the glyph,
corresponding to the way that it appears on the screen. Each pixel can
have a possible value in the range of 0 to 9.

-  0 means that the pixel is transparent.
-  The values 1 to 9 represent the relative brightness of that pixel,
   with 9 being the brightest. There does not appear to be a linear
   increase in brightness from values 1 to 9.

Here is an example of the bitmap of a glyph. This glyph is from the
file FONT1.AAF and represents the character 'b' (ASCII 98).

.. code::

   77
   77
   777763
   77  76
   77  77
   77  76
   777763

Pixels with value 0 are not shown as they are transparent. The higher
numbers 7 and 6 represent the brighter parts of the glyph, while the 3
would be much duller in colour. This would be expected for the rounder
corners of the b.


**Multi-byte sequences are stored in big-endian format, NOT LITTLE
ENDIAN**

-------------
Binary Format
-------------


~~~~~~~~
``file``
~~~~~~~~

.. code:: text


             0                   1                   2                   3
             0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |       "A"       |     "A"       |      "F"      |     "F"     |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |         max_glyph_height        |       letter_spacing        |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |           space_width           |        line_spacing         |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    0x000c: |                               .                               |
            |                               .                               |
            |                       glyph_description                       |
            |                len = sizeof(glyph_description) * 255          |
            |                               .                               |
            |                               .                               |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    0x080c: |                              .                                |
            |                               .                               |
            |                          glyph_data                           |
            |                               .                               |
            |                               .                               | : 0x? (depends on glyph dimensions)
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

~~~~~~~~~~~~~~~~~~~~~
``glyph_description``
~~~~~~~~~~~~~~~~~~~~~

.. code:: text


        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |            glyph_width          |        glyph_height         |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                      offset_in_glyph_data                     |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


~~~~~~~~~~~~~~
``glyph_data``
~~~~~~~~~~~~~~

.. code:: text


    xy coordinates = x,y
    min(opacity) == 0
    max(opacity) = 9
    w = glyph_width
    h = glyph_height


        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       | opacity @ 0,0 | opacity @ 1,0 | opacity @ 2,0 | opacity @ x,y |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |   ...etc...   | ...until...   | opa @ w-2,h-1 | opa @ w-1,h-1 | : 0x(w*h)
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

----------
References
----------

Documentation sourced from:

- `Fandom <https://falloutmods.fandom.com/wiki/AAF_File_Format>`_
- `Falltergeist <https://github.com/falltergeist/falltergeist>`_ source code
- Original description derived by `TeamX <http://www.teamx.ru/>`_ ,
  which appears to be shutdown now, but provides a link to archived
  documentation.
