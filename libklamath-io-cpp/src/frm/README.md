# frm files

WIP

From: https://falloutmods.fandom.com/wiki/FRM_File_Format

Used to store all game images (incl. animation frames). The only exception is splash screens (rix format).

# Technical specs

- 8-bit color information.
- Color value maps to color in external color palette file (pal format)
- Should default to `color.pal` if no color palette is specified
- Color palette specified by specifying file with same name as `.frm` file, but with `.pal` extension
- Each FRM file can contain one (static) or many (animation) images
- Multi-byte sequences are **BIG ENDIAN**

- Specifying orientations requires renaming the file from `.frm` to `.frX`, where `X` is between `[0` and `5]`
- Game has 6 orientations (e.g. character facing orientations). `0` represents north east

# `header`

```text

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |               frm_format_version (= 0x04)                     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |       fps (= 0x08 or 0x16)    |         action_frame          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
TODO

```
art\tiles\head003.frm