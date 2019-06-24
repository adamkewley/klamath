===
MAP
===

==Introduction== The MAP File Format contains the information for a map
used in the game. A map consists of a grid that the player walks on,
plus all the associated walls, scenery, critters, objects etc. A MAP
file may contain up to 3 separate levels, typically used for
representing different elevations.

Each level contains a 200 by 200 hex grid, for a total of 40000 possible
positions on the grid. The format for locations on the grid is: 00 00 XX
YY, where XX is the horizontal coordinate [0 .. 199], and YY is the
vertical coordinate [0 .. 199].

The MAP file format consists of 5 parts:

Header of the MAP file
======================

Global and Local Variables
==========================

Tiles
=====

MAP Scripts
===========

MAP Objects
===========

==PID== The PID is a fundamental type used in the MAP file. It is an
identifier for describing objects. It consists of a 4 byte integer of
the form 0xaa00bbbb. The byte aa is the type of the object, while the 2
bytes bbbb are the id of the object. The id is typically an index into a
LST file. Valid types include:

*00: items *\ 01: critters *02: scenery *\ 03: walls *04: tiles *\ 05:
misc *06: intrface *\ 07: inven *08: heads *\ 09: background

.. raw:: html

   <HR WIDTH="80%" ALIGN="Left">

Note: for PIDs that refer to critter [[FRM File Format\|FRM files]], the
PID format is more complex. This format will be documented later.

.. raw:: html

   <HR WIDTH="80%" ALIGN="Left">

==Header of the MAP file==

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center" width="50">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center" width="40">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center" width="80">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned = 19 or 20

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Map version.

''Fallout 1'' uses map version 19, while ''Fallout 2'' uses 20.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

16

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

string

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Map filename.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0014

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed = [0..39999]

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Default player position. The default hex grid that the player will start
in when the map is entered, if not overridden.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0018

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed = [0..2]

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Default map elevation. The default map elevation for the player to start
in when the map is entered, if not overridden.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x001C

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed = [0..5]

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Default player orientation. The default orientation the player is facing
when the map is entered.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0020

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

NUM-LOCAL-VARS : Number of local variables stored in map.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0024

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Script id for this map. Value of -1 means no map. Text string is found
in MSG file scrname.msg at index [id + 101].

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0028

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Elevation flags.

\*If (flag & 0x1) == 0 then ?? unknown.

*If (flag & 0x2) == 0 then the map has an elevation at level 0. *\ If
(flag & 0x4) == 0 then the map has an elevation at level 1. \*If (flag &
0x8) == 0 then the map has an elevation at level 2.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x002C

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed = 1

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Map darkness (according to mapper2, not sure if used).

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0030

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

NUM-GLOBAL-VARS : Number of global variables stored in map.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0034

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Map Id.

*''Fallout 1'': Map filename found in map.msg *''Fallout 2'': Map
details found in data/maps.txt in section [Map id]

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0038

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Time since the epoch. Number of time ticks since the epoch. A time tick
is equivalent to 0.1 seconds in game time. The epoch for ''Fallout 1''
is "5 December 2161 00:00am", and for ''Fallout 2'' "25 July 2241
00:00am".

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x003C

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4 \* 44

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed = 0

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

==Global and Local Variables== The global and local variables used by
the map scripts are stored here in arrays.

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td>

0x00EC

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4 \* NUM-GLOBAL-VARS

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Array of global variables.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x00EC + (4 \* NUM-GLOBAL-VARS)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4 \* NUM-LOCAL-VARS

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Array of local variables.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

==Tiles== Each level of the map consists of tile data for both the roof
and floor. These tiles are on an isometric grid, which is independent of
the hexagonal grid used for critters, scenery, objects etc. The grid
size is 100 by 100, which gives a total of 20000 tiles for each level
(including both floor and roof). For each elevation that exists in the
MAP file, there is the following tile information.

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center" width="110">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center" width="70">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center" width="65">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

2

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Roof tile id for tile position 0. The id is the filename of the FRM file
containing the tile image data. The filename can be found in the LST
file art/tiles/tiles.lst using this id. An id of 1 means no tile.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0002 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

2

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Floor tile id for tile position 0. Same comments apply as above.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

2

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Roof tile id for tile position 1.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0006 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

2

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Floor tile id for tile position 1.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0008 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

(2 + 2) \* (20000 - 2)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Roof/Floor tiles id for tile positions [2-9999].

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

==MAP Scripts== This section of the file stores information about the
scripts connected to objects in this map. This section is not really
understood very well.

There are 5 types of scripts found in the MAP file. The type of the
script is found in the PID.

Scripts in this section are always grouped in multiples of 16, rounding
up. After each group of 16 scripts is a check integer.

Here is some sample C code used to skip over the script section.

.. raw:: html

   <pre>
   /* read in each sequence of scripts */
   for (i = 0; i &lt; 5; i++) {

      /* number of scripts used in this sequence */
      count = read_int32_big_endian(stream);
      if (count &gt; 0) {

         /* loop counter must be modulo 16 (rounded up) */
         loop = MODULO_16(count);

         check = 0;

         /* read in all the scripts of this sequence */
         for (j = 0; j &lt; loop; j++) {
            read_script(stream);

            /* after every 16 scripts is the check block */
            if ((j % 16) == 15) {
               v = read_int32_big_endian(stream);
               check += v;

               /* don't know what this is for, so ignore it for now */
               v = read_int32_big_endian(stream);
            }
         }
         if (check != count) {
            set_error_message(&quot;error reading scripts: check is incorrect&quot;);
            okay = FALSE;
            break;
         }
      }
   }

   return okay;
   </pre>

The read\_script() function reads in a script of the following format.

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

PID

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

PID : PID of the script.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed = -1

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Next script. Unused.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0008 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Timer script time, or

Spatial script hex. First two bytes are elevation:

0x0000 - 1

0x2000 - 2

0x4000 - 3

Only read this if PID has type 1 or 2 (spatial or timer)

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x000C + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Spatial script radius.

Only read this if PID has type 1 (spatial)

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0010 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Script flags (0 in maps, value in saves).

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0014 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Script id.

Script filename is found in LST file script.lst at index id.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0018 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 5.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x001C + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Script oid.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0020 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed = -1

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Local var offset (-1 in maps, value on saves).

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0024 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Num local vars (0 in maps, value in saves).

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0028 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 9.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x002C + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 10.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0030 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 11.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0034 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed = -1

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 12.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0038 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 13.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x003C + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 14.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0040 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 15.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0044 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 16.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

==MAP Objects== The objects contain the scenery, walls, items,
containers, keys and critters that appear on the map. There is an array
of objects for each elevation of the map.

*4 byte integer containing total number of objects on all levels *\ for
each of the three levels **4 byte integer containing number of objects
on this level **\ array on map objects

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center" width="125">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center" width="100">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 0. I don't think this is part of the object, but some kind of
separator.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

= [-1..39999]

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Position of this object. Hex grid id that the object resides in. A value
of -1 means that the object is not on the grid (typically it is in an
inventory).

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0008 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

X (Unknown 1).

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x000C + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Y (Unknown 2).

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0010 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

SX (Unknown 3).

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0014 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

SY (Unknown 4).

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0018 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Frame number. This is the frame index of the frame in the FRM file this
is currently being displayed.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x001C + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned = [0-5]

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Orientation of this object.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0020 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

PID

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

FRM PID of this object. PID of the filename used to display this object
on the hex grid.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0024 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown Flags. Collection of flags about this object.

*If (flag & 0x01000000) == 1 then this item is held in the right hand
*\ If (flag & 0x02000000) == 1 then this item is held in the left hand
\*If (flag & 0x04000000) == 1 then this armour is worn This is the same
flags as in PRO files. They override values from prototype

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0028 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned = [0..2]

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Map elevation this object is on.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x002C + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

PID

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

PROTO-PID">PROTO-PID : Prototype PID this object is based on.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0030 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Critter index number. (Only for in-battle .SAV ) -1 for normal objects

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0034 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Light radius (in hexes)

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0038 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Light intensity (0..65536, interpreted as 0-100%)

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x003C + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned = 0

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Outline color. (Only for in-battle .SAV)

0x0 - no outline

0x1 - red

0x20 - yellow

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0040 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

PID

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

PID of MAP Scripts.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0044 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Script id.

Script filename is found in LST file script.lst at index id. A value of
-1 means no script.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0048 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Number of map objects in this object's inventory. If this is non zero,
then after reading this map object, the objects in the inventory must be
read. The objects in the inventory are map objects as well, and they
follow this object, being preceeded by a 4 byte integer being the count
of this map object in the inventory.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x004C + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Maximum number of slots in critter inventory.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0050 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 10.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0054 + (offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Unknown 11.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for critters===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Reaction to player (not sure).

Only valid for .SAV (not sure)

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Current mp (?).

Only valid for .SAV

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0008 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Combat results.

Only valid for .SAV

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x000C + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Damage last turn.

Only valid for .SAV

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0010 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

AI packet number.

Packet number of critter AI, found in data/AI.txt.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0014 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Group id.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0018 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Who hit me.

Only valid for .SAV

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x001C + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Current Hit Points.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0020 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Current Rad.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0024 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Current Poison.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for ammo===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Amount of ammo in magazine. Number of bullets or charges in this
magazine.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for keys===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

KeyCode

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for misc items===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Charges.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for weapons===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center" width="150">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Ammo count. Amount of ammunition loaded in this weapon.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

signed

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Id of ammo prototype. Ammo prototype filename is found in LST file
proto/items/items.lst at index id.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for ladder bottom===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

DestHex and DestElev.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Destination map num.

Only in MAP version 20.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for ladder top===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

DestHex and DestElev.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Destination map num.

Only in MAP version 20.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for portals/doors===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

WalkThrough.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for stairs===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

DestHex and DestElev.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Destination map num.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for elevators===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

ElevType.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

ElevLevel.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

===Extra fields for exit grids===

.. raw:: html

   <table border="1">

.. raw:: html

   <tr>

.. raw:: html

   <td align="center" width="150">

Offset

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Size

.. raw:: html

   </td>

.. raw:: html

   <td align="center" width="130">

Data Type

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

Description

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0000 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

EXIT-MAP-ID">EXIT-MAP-ID : Map Id. The id of the map that this exit grid
leads to.

*Fallout 1: Map filename found in map.msg *\ Fallout 2: Map details
found in data/maps.txt in section [Map id]

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0004 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned = [0..39999]

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Player position. Position on the hex grid that the player will start in
when moving to map EXIT-MAP-ID.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x0008 + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned = [0..2]

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Map elevation. Elevation of map EXIT-MAP-ID that this exit grid leads
to.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   <tr>

.. raw:: html

   <td align="center">

0x000C + (0x58 + offset)

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

4

.. raw:: html

   </td>

.. raw:: html

   <td align="center">

unsigned = [0..5]

.. raw:: html

   </td>

.. raw:: html

   <td align="left">

Player orientation. Orientation of the player when entering EXIT-MAP-ID
from this exit grid.

.. raw:: html

   </td>

.. raw:: html

   </tr>

.. raw:: html

   </table>

==Copyright== (c) by TeamX, taken from their
[http://www.teamx.ru/files/docs/map.rar website][[Category:Fallout and
Fallout 2 file formats]]
