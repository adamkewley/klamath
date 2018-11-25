===
FRM
===

This article delves into the raw data format of the FRM graphics file.
The same format was used for both Fallout and Fallout 2, giving all FRM
files complete compatibility with both game engines.

== General Information == The FRM format is used to store all of the
game's images for map objects, critter animations, talking heads and
their respective backgrounds, as well as all of the components which
comprise the Pip-Boy interface. Splash screens are the only images which
are stored in a [[RIX File Format\|separate format]]. Each FRM contains
one or more frame(s) of image data.

== Technical Information == === Pallete === FRMs are unpaletted
256-color image files containing either one or several image frame(s).
The palettes used for FRMs come in the form of [[PAL File
Format\|external palette files]]. The engine defaults to color.pal if
not specified, but FRMs may have their own distinct palette files. These
specific palette files have the same name as the FRM, but with the
extension .pal. For example, the palette file of helpscrn.frm is
helpscrn.pal.

An index color model is used to store raw pixel data, meaning each pixel
is represented by an index in an array of colors. For the default
palette (color.pal) which has 256 color indexes, the color index of 0
represents transparency.

=== Endianness === All values within the FRM are stored in Big-Endian
(Motorola) format as opposed to Little-Endian (Intel) format. For those
who don't know what the difference is, Big-Endian stores the
most-significant byte (largest digit places) closest to the number's
starting address, whereas Little-Endian stores the least-significant
byte (smallest digit places) closest to the starting address.

[[File:FRM\_Endianess.jpg]]

=== File Suffixes === An FRM may contain image data for one or all of
the 6 orientations used in Fallout. If an FRM has the extension .fr[0-5]
instead of the usual .frm, then that file contains image data for the
orientation given by the last digit in the extension. 0 represents the
northeast direction and each subsequent suffix rotates in the clockwise
direction (5 representing the northwest direction). Otherwise, the FRM
contains image data for either orientation 0, or all 6 orientations.

=== Frame Positioning === The FRM contains offset values used to
correctly align (or center) the image data. For example, image data
should be aligned so that each critter's feet should be on the same
level.

Each frame contained within the FRM has an offset from the previous
frame which must be applied before rendering. This offset ensures that
each frame is correctly aligned during an animation sequence.

The center of the image data is the center of the bottom edge of the
frame. To find the position of the top left corner of the frame (needed
for rendering):

left = center\_x - (frame\_width / 2) top = center\_y - frame\_height

==FRM header & data== {\| bgcolor="#ffffff" border="1" cellpadding="5"
cellspacing="0" ! bgcolor="#f0f0ff" \|Offset ! bgcolor="#f0f0ff" \|Size
! bgcolor="#f0f0ff" \|Datatype ! bgcolor="#f0f0ff" \|Possible values in
Fallout 1/2 .FRM's ! bgcolor="#f0f0ff" width="45%" \|Description \|-
\|0x0000 \|4 bytes \|unsigned \|0x04 \|'''Version number of the FRM
format''' \|- \|0x0004 \|2 bytes \|unsigned \|0x08 - 0x16, possibly more
\|'''FPS''' - Frames per second rate of the animation \|- \|0x0006 \|2
bytes \|unsigned \|0x01 -> 0xFF (if the FRM has 255 single frames which
is highly unlikely) \|'''Action frame''' - Frame of the animation on
which actions occur (shot, open doors, etc.) \|- \|0x0008 \|2 bytes
\|unsigned \|0x01 -> 0xFF \|'''Number of frames per direction''' - All
orientations of an FRM have the same number of frames. For a static
image, this number will be 1. For an animation of 10 frames, this value
will be 10, even though there would be a total of 60 frames stored (10
for each of the 6 orientations) \|- \|0x000A \|2 bytes \|signed
\|Unknown \|'''Pixel shift in the x direction, of frames with
orientation 0''' - This value is required so that the frames are
centered correctly. The logical center of the frame will not necessarily
be the absolute center of the frame (width/2, height/2). For example,
the center of a critter will be the position of its feet, while the
center of a tree will be its base. A positive value translates to
shifting the frame to the right of the screen when rendering as the
"origin" (0, 0) for images is the top left-most pixel \|- \|0x000C \|5
\* 2 bytes \|signed \|Unknown \|'''Pixel shift in the x direction, of
frames with orientations [1-5]''' \|- \|0x0016 \|6 \* 2 bytes \|signed
\|Unknown \|'''Pixel shift in the y direction, of frames with
orientations [0-5]''' - Positive values shift the frame down toward
bottom of the screen when rendering \|- \|0x0022 \|4 bytes \|unsigned
\|Unknown \|'''Offset of first frame in orientation 0 from beginning of
frame area''' - The "frame area" begins from offset 0x003E. This is
useful for seeking directly to where image data begins for each
orientation \|- \|0x0026 \|5 \* 4 bytes \|unsigned \|Unknown \|'''Offset
of first frame in orientation[1-5] from beginning of frame area''' \|-
\|0x003A \|4 bytes \|unsigned \|Unknown \|'''Size of frame data''' -
Used to allocate memory for all frame data in the file \|- \|
align="center" bgcolor="#343434" colspan="5" style="color:#ffffff" \|
'''END OF HEADER // BEGINNING OF FRAME AREA''' \|- \|0x003E \|2 bytes
\|unsigned \|Unknown \|'''FRAME-0-WIDTH: Width of frame 0''' - The width
(in pixels) of the 0th frame in the orientation 0 \|- \|0x0040 \|2 bytes
\|unsigned \|Unknown \|'''FRAME-0-HEIGHT: Height of frame 0''' - The
height (in pixels) of the 0th frame in the orientation 0 \|- \|0x0042
\|4 bytes \|unsigned \|FRAME-0-WIDTH \* FRAME-0-HEIGHT
\|'''FRAME-0-SIZE: Number of pixels for frame 0''' - The total area, and
hence number of bytes taken up by the 0th frame \|- \|0x0046 \|2 bytes
\|signed \|Unknown \|'''Pixel shift in x direction of frame 0''' - The
pixel offset of this frame from the previous frame. The frame shifts
left if negative, right if positive. \|- \|0x0048 \|2 bytes \|signed
\|Unknown \|'''Pixel shift in y direction of frame 0''' The frame shifts
up if negative, down if positive. \|- \|0x004A \|FRAME-0-SIZE bytes
\|unsigned \|0x00 -> 0x3F(hex) / 63(dec) \|'''colorIndex for frame 0'''
- Contains the colorIndex for the 0th frame. Each index is represented
by one byte; there are 256 possible indices and their RGB values can be
found in the appropriate .pal file. Pixel data starts at the top left
corner of the frame and increases left to right, then top to bottom \|-
\|0x004A + (FRAME-0-SIZE) \|flexible \|unsigned \|0x00 -> 0xFF
\|'''Frame data for all the following frames''' - Frames are stored for
all the rest of the frames of orientation 0, then moves on to
orientation 1 up to 5 (if these orientations are stored in the file) \|}
[[Category:Fallout and Fallout 2 file formats]]

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