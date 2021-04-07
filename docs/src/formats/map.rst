===
MAP
===

Introduction
============

The MAP File Format contains the information for a map
used in the game. A map consists of a grid that the player walks on,
plus all the associated walls, scenery, critters, objects etc. A MAP
file may contain up to 3 separate levels, typically used for
representing different elevations.

Each level contains a 200 by 200 hex grid, for a total of 40000 possible
positions on the grid. The format for locations on the grid is: 00 00 XX
YY, where XX is the horizontal coordinate [0 .. 199], and YY is the
vertical coordinate [0 .. 199].

The MAP file format consists of 5 parts:

1. `Header of the MAP file`_
2. `Global and Local Variables`_
3. `Tiles`_
4. `MAP Scripts`_
5. `MAP Objects`_

PID
===

The PID is a fundamental type used in the MAP file. It is an
identifier for describing objects. It consists of a 4 byte integer of
the form 0xaa00bbbb. The byte aa is the type of the object, while the 2
bytes bbbb are the id of the object. The id is typically an index into a
LST file. Valid types include:

* 00: items 
* 01: critters 
* 02: scenery 
* 03: walls 
* 04: tiles
* 05: misc 
* 06: intrface 
* 07: inven 
* 08: heads 
* 09: background

.. note::
   For PIDs that refer to critter :doc:`frm` files, the
   PID format is more complex. This format will be documented later.

------------

Header of the MAP file
======================

.. list-table:: MAP header
   :header-rows: 1
   :widths: 10, 10, 15, 65

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000
     - 4
     - | unsigned 
       | = 19 or 20
     - | Map version.
       | *Fallout 1* uses map version 19, while *Fallout 2* uses 20.
   * - 0x0004
     - 16
     - string
     - Map filename.
   * - 0x0014
     - 4
     - | signed 
       | = [0..39999]
     - | Default player position. 
       | The default hex grid that the player will start in when the map is entered, if not overridden.
   * - 0x0018
     - 4
     - | signed
       | = [0..2]
     - | Default map elevation. 
       | The default map elevation for the player to start in when the map is entered, if not overridden.
   * - 0x001C
     - 4
     - | signed
       | = [0..5]
     - | Default player orientation. 
       | The default orientation the player is facing when the map is entered.
   * - 0x0020
     - 4
     - signed
     - NUM-LOCAL-VARS : Number of local variables stored in map.
   * - 0x0024
     - 4
     - signed
     - | Script id for this map. 
       | Value of -1 means no map. 
       | Text string is found in MSG file scrname.msg at index [id + 101].
   * - 0x0028
     - 4
     - signed
     - | Elevation flags.
       * If (flag & 0x1) != 0 then the map is a savegame map (.SAV).
       * If (flag & 0x2) == 0 then the map has an elevation at level 0. 
       * If (flag & 0x4) == 0 then the map has an elevation at level 1. 
       * If (flag & 0x8) == 0 then the map has an elevation at level 2.
   * - 0x002C
     - 4
     - signed = 1
     - Map darkness (according to mapper2, not sure if used).
   * - 0x0030
     - 4
     - signed
     - NUM-GLOBAL-VARS : Number of global variables stored in map.
   * - 0x0034
     - 4
     - signed
     - | Map Id.
       * *Fallout 1*: Map filename found in map.msg 
       * *Fallout 2*: Map details found in data/maps.txt in section [Map id]
   * - 0x0038
     - 4
     - unsigned
     - | Time since the epoch. Number of time ticks since the epoch. 
       | A time tick is equivalent to 0.1 seconds in game time. The epoch for *Fallout 1*
         is "5 December 2161 00:00am", and for *Fallout 2* "25 July 2241 00:00am".
   * - 0x003C
     - 4 \* 44
     - signed = 0
     - Unknown.

Global and Local Variables
==========================

The global and local variables used by
the map scripts are stored here in arrays.

.. list-table:: Global and Local Variables format
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x00EC
     - 4 * NUM-GLOBAL-VARS
     - signed
     - Array of global variables.
   * - 0x00EC + (4 \* NUM-GLOBAL-VARS)
     - 4 \* NUM-LOCAL-VARS
     - signed
     - Array of local variables.

Tiles
=====

Each level of the map consists of tile data for both the roof
and floor. These tiles are on an isometric grid, which is independent of
the hexagonal grid used for critters, scenery, objects etc. The grid
size is 100 by 100, which gives a total of 20000 tiles for each level
(including both floor and roof). For each elevation that exists in the
MAP file, there is the following tile information.

.. list-table:: Tiles format
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (offset)
     - 2
     - unsigned
     - | Roof tile id for tile position 0. 
       | The id is the filename of the FRM file containing the tile image data. 
       | The filename can be found in the LST file art/tiles/tiles.lst using this id. 
       | An id of 1 means no tile.
   * - 0x0002 + (offset)
     - 2
     - unsigned
     - Floor tile id for tile position 0. Same comments apply as above.
   * - 0x0004 + (offset)
     - 2
     - unsigned
     - Roof tile id for tile position 1.
   * - 0x0006 + (offset)
     - 2
     - unsigned
     - Floor tile id for tile position 1.
   * - 0x0008 + (offset)
     - (2 + 2) \* (20000 - 2)
     - unsigned
     - Roof/Floor tiles id for tile positions [2-9999].

MAP Scripts
===========

This section of the file stores information about the
scripts connected to objects in this map. This section is not really
understood very well.

There are 5 types of scripts found in the MAP file. The type of the
script is found in the PID.

Scripts in this section are always grouped in multiples of 16, rounding
up. After each group of 16 scripts is a check integer.

Here is some sample C code used to skip over the script section.

.. code-block:: c

   /* read in each sequence of scripts */
   for (i = 0; i < 5; i++) {

      /* number of scripts used in this sequence */
      count = read_int32_big_endian(stream);
      if (count > 0) {

         /* loop counter must be modulo 16 (rounded up) */
         loop = MODULO_16(count);

         check = 0;

         /* read in all the scripts of this sequence */
         for (j = 0; j < loop; j++) {
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
            set_error_message("error reading scripts: check is incorrect");
            okay = FALSE;
            break;
         }
      }
   }

   return okay;


The read\_script() function reads in a script of the following format.

.. list-table:: Script format
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (offset)
     - 4
     - PID
     - PID : PID of the script.  
   * - 0x0004 + (offset)
     - 4
     - signed = -1
     - Next script. Unused.
   * - 0x0008 + (offset) 
     - 4
     - signed
     - | Timer script time, or
       | Spatial script hex. First two bytes are elevation:
       * 0x0000 - 1
       * 0x2000 - 2
       * 0x4000 - 3
       | Only read this if PID has type 1 or 2 (spatial or timer)
   * - 0x000C + (offset)
     - 4
     - signed
     - | Spatial script radius. 
       | Only read this if PID has type 1 (spatial)
   * - 0x0010 + (offset)
     - 4
     - signed
     - Script flags (0 in maps, value in saves).
   * - 0x0014 + (offset)
     - 4
     - signed
     - | Script id.
       | Script filename is found in LST file script.lst at index id.
   * - 0x0018 + (offset)
     - 4
     - signed
     - Unknown 5.
   * - 0x001C + (offset)
     - 4
     - signed
     - Script oid.
   * - 0x0020 + (offset)
     - 4
     - signed = -1
     - Local var offset (-1 in maps, value on saves).
   * - 0x0024 + (offset)
     - 4
     - signed
     - Num local vars (0 in maps, value in saves).
   * - 0x0028 + (offset)
     - 4
     - signed
     - Unknown 9.
   * - 0x002C + (offset)
     - 4
     - signed
     - Unknown 10.
   * - 0x0030 + (offset)
     - 4
     - signed
     - Unknown 11.
   * - 0x0034 + (offset)
     - 4
     - signed = -1
     - Unknown 12.
   * - 0x0038 + (offset)
     - 4
     - signed
     - Unknown 13.
   * - 0x003C + (offset)
     - 4
     - signed
     - Unknown 14.
   * - 0x0040 + (offset)
     - 4
     - signed
     - Unknown 15.
   * - 0x0044 + (offset)
     - 4
     - signed
     - Unknown 16.

MAP Objects
===========

The objects contain the scenery, walls, items,
containers, keys and critters that appear on the map. There is an array
of objects for each elevation of the map.

* 4 byte integer containing total number of objects on all levels 
* for each of the three levels 
   * 4 byte integer containing number of objects on this level
   * array on map objects

.. list-table:: Map object format
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (offset)
     - 4
     - unsigned
     - Unknown 0. I don't think this is part of the object, but some kind of separator.
   * - 0x0004 + (offset)
     - 4
     - = [-1..39999]
     - | Position of this object. 
       | Hex grid id that the object resides in. 
         A value of -1 means that the object is not on the grid (typically it is in an inventory).
   * - 0x0008 + (offset)
     - 4
     - unsigned
     - X (Unknown 1).
   * - 0x000C + (offset)
     - 4
     - unsigned
     - Y (Unknown 2).
   * - 0x0010 + (offset)
     - 4
     - signed
     - SX (Unknown 3).
   * - 0x0014 + (offset)
     - 4
     - signed
     - SY (Unknown 4).
   * - 0x0018 + (offset)
     - 4
     - unsigned
     - | Frame number. 
       | This is the frame index of the frame in the FRM file this is currently being displayed.
   * - 0x001C + (offset)
     - 4
     - unsigned = [0-5]
     - Orientation of this object.
   * - 0x0020 + (offset)
     - 4
     - PID
     - | FRM PID of this object. 
       | PID of the filename used to display this object on the hex grid.
   * - 0x0024 + (offset)
     - 4
     - unsigned
     - | Unknown Flags. Collection of flags about this object.
       * If (flag & 0x01000000) == 1 then this item is held in the right hand 
       * If (flag & 0x02000000) == 1 then this item is held in the left hand
       * If (flag & 0x04000000) == 1 then this armour is worn 
       | These are the same flags as in PRO files. They override values from prototype
   * - 0x0028 + (offset)
     - 4
     - unsigned = [0..2]
     - Map elevation this object is on.
   * - 0x002C + (offset)
     - 4
     - PID
     - PROTO-PID">PROTO-PID : Prototype PID this object is based on.
   * - 0x0030 + (offset)
     - 4
     - signed
     - Critter index number. (Only for in-battle .SAV ) -1 for normal objects
   * - 0x0034 + (offset)
     - 4
     - unsigned
     - Light radius (in hexes)
   * - 0x0038 + (offset)
     - 4
     - unsigned
     - Light intensity (0..65536, interpreted as 0-100%)
   * - 0x003C + (offset)
     - 4
     - unsigned = 0
     - | Outline color. (Only for in-battle .SAV)
       * 0x0 - no outline
       * 0x1 - red
       * 0x20 - yellow
   * - 0x0040 + (offset)
     - 4
     - PID
     - PID of MAP Scripts.
   * - 0x0044 + (offset)
     - 4
     - signed
     - | Script id.
       | Script filename is found in LST file script.lst at index id. 
       | A value of -1 means no script.
   * - 0x0048 + (offset)
     - 4
     - unsigned
     - | Number of map objects in this object's inventory. 
       | If this is non zero, then after reading this map object, the objects in the inventory must be read. 
       | The objects in the inventory are map objects as well, and they follow this object, 
         being preceeded by a 4 byte integer being the count of this map object in the inventory.
   * - 0x004C + (offset)
     - 4
     - unsigned
     - Maximum number of slots in critter inventory.
   * - 0x0050 + (offset)
     - 4
     - unsigned
     - Unknown 10.
   * - 0x0054 + (offset)
     - 4
     - unsigned
     - Unknown 11.

Extra fields for critters
-------------------------

.. list-table:: Extra fields for critters
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - | Reaction to player (not sure).
       | Only valid for .SAV (not sure)
   * - 0x0004 + (0x58 + offset)
     - 4
     - unsigned
     - | Current mp (?).
       | Only valid for .SAV
   * - 0x0008 + (0x58 + offset)
     - 4
     - unsigned
     - | Combat results.
       | Only valid for .SAV
   * - 0x000C + (0x58 + offset)
     - 4
     - unsigned
     - | Damage last turn.
       | Only valid for .SAV
   * - 0x0010 + (0x58 + offset)
     - 4
     - signed
     - | AI packet number.
       | Packet number of critter AI, found in data/AI.txt.
   * - 0x0014 + (0x58 + offset)
     - 4
     - unsigned
     - Group id.
   * - 0x0018 + (0x58 + offset)
     - 4
     - unsigned
     - | Who hit me.
       | Only valid for .SAV
   * - 0x001C + (0x58 + offset)
     - 4
     - unsigned
     - Current Hit Points.
   * - 0x0020 + (0x58 + offset)
     - 4
     - unsigned
     - Current Rad.
   * - 0x0024 + (0x58 + offset)
     - 4
     - unsigned
     - Current Poison.

Extra fields for ammo
---------------------

.. list-table:: Extra fields for ammo
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - | Amount of ammo in magazine. 
       | Number of bullets or charges in this magazine.

Extra fields for keys
---------------------

.. list-table:: Extra fields for keys
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - KeyCode

Extra fields for misc items
---------------------------

.. list-table:: Extra fields for misc items
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - Charges.

Extra fields for weapons
------------------------

.. list-table:: Extra fields for weapons
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - Ammo count. Amount of ammunition loaded in this weapon.
   * - 0x0004 + (0x58 + offset)
     - 4
     - signed
     - | Id of ammo prototype. 
       | Ammo prototype filename is found in LST file ``proto/items/items.lst`` at index id.

Extra fields for ladder bottom
------------------------------

.. list-table:: Extra fields for ladder bottom
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - DestHex and DestElev.
   * - 0x0004 + (0x58 + offset)
     - 4
     - unsigned
     - | Destination map num.
       | Only in MAP version 20.

Extra fields for ladder top
---------------------------

.. list-table:: Extra fields for ladder top
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - DestHex and DestElev.
   * - 0x0004 + (0x58 + offset)
     - 4
     - unsigned
     - | Destination map num.
       | Only in MAP version 20.

Extra fields for portals/doors
------------------------------

.. list-table:: Extra fields for portals/doors
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - WalkThrough.

Extra fields for stairs
-----------------------

.. list-table:: Extra fields for stairs
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - DestHex and DestElev.
   * - 0x0004 + (0x58 + offset)
     - 4
     - unsigned
     - Destination map num.

Extra fields for elevators
--------------------------

.. list-table:: Extra fields for elevators
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - ElevType.
   * - 0x0004 + (0x58 + offset)
     - 4
     - unsigned
     - ElevLevel.

Extra fields for exit grids
---------------------------

.. list-table:: Extra fields for exit grids
   :header-rows: 1

   * - Offset
     - Size
     - Data Type
     - Description
   * - 0x0000 + (0x58 + offset)
     - 4
     - unsigned
     - | EXIT-MAP-ID">EXIT-MAP-ID : Map Id. The id of the map that this exit grid leads to.
       * *Fallout 1*: Map filename found in map.msg 
       * *Fallout 2*: Map details found in data/maps.txt in section [Map id]
   * - 0x0004 + (0x58 + offset)
     - 4
     - unsigned = [0..39999]
     - | Player position. 
       | Position on the hex grid that the player will start in when moving to map EXIT-MAP-ID.
   * - 0x0008 + (0x58 + offset)
     - 4
     - unsigned = [0..2]
     - | Map elevation. 
       | Elevation of map EXIT-MAP-ID that this exit grid leads to.
   * - 0x000C + (0x58 + offset)
     - 4
     - unsigned = [0..5]
     - | Player orientation. 
       | Orientation of the player when entering EXIT-MAP-ID from this exit grid.

Copyright (c) by TeamX, taken from their website http://www.teamx.ru/files/docs/map.rar
