========
SAVE.DAT
========

'''SAVE.DAT''' files are '''archived data''' files that contain data for
savegames. Everything about the player, party members, global variables,
and state of the world map is stored here. The only thing not included
is information about each town, which is stored in individual [[SAV File
Format \|.SAV files]]. While they have the .DAT extension, they do not
have the same format as [[DAT file format \|other .DAT files]]. These
files are found in
:raw-latex:`\data`:raw-latex:`\SAVEGAME`:raw-latex:`\SlotXX`, XX
representing the number of the save game.

As with various other Fallout data files, if you're viewing this in a
Hex editor, make sure it's in "Big Endian" (non-PC) display format.

==Introduction== Almost everything has been verified for the ''Mac''
version of [http://fallout.wikia.com/wiki/Fallout\_2 Fallout 2]; there
may be some differences between this and the Windows version.

A little explanation of the data types:

:\* ASCII (C-style) strings end with a 0 byte - the hex value 0x0, not
the character '0'. So, if the length of an ASCII string is 32, it can
contain 31 characters with 0x0 at the end.

:\* Most numbers are written as four byte integers, in big-endian (most
significant byte first) format. Negative numbers are stored in two's
complement format (e.g. -1 is 0xFFFFFFFF).

:\* The data in a save.dat file is not written all at once, but in a
series of separate functions. This can be seen when examining debug
files (debug.log). When saving it has the following entries:

.. raw:: html

   <pre>
   LOADSAVE: Save file header size written: XXXX bytes.

   LOADSAVE: Save function #X data size written: XXXX bytes.
   ... 
   </pre>

where X is some number

This is done because different functions save different types of data
(about quests, items, etc.). All the data is recorded sequentially.
Therefore, to describe the file we divide the data into separate
functions. There are 27 functions (not including the header). The
numbering starts with zero.

Because parts of the data have variable sizes, it can be fairly
difficult to find a particular part of the data. It is considerably
easier with the help of the file description and looking in debug.log
for the size of the right function.

Note: offsets for each function (beginning with the fourth) are shown
relative to the end of the previous one.

==Structure==

===Header=== Size: 0x7563 (decimal 30051) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x18 \|\| Value: 'FALLOUT SAVE FILE ' Description:
SAVE.DAT file signature \|- \| 0x18 \|\| 0x04 \|\|Version of the game
which made the save file. For example, for version 1.02 it is 00 01 00
02. \|- \| 0x1C \|\| 0x01 \|\| The letter 'R'. Ask the developers why.
Could be 'R' for Release? \|- \|0x1D \|\| 0x20 \|\| Player name. ASCII
string, so the longest possible name is 31 characters. \|- \| 0x3D \|\|
0x1E \|\| Savegame name. ASCII string. \|- \| 0x5B \|\| 0x02 \|\| The
day the game was saved. Realtime, not gametime. \|- \| 0x5D \|\| 0x02
\|\| The month. \|- \| 0x5F \|\| 0x02 \|\| The year. \|- \| 0x61 \|\|
0x04 \|\| Minutes + hours (24-hour format), realtime. E.g. if the time
is 16:45, then it is 0x0000003D. (This is a bit odd, as 17:44 also gives
0x3D . Perhaps it was supposed to be two 2-byte numbers (0x0010, 0x002D
for 16:45). It doesn't seem to be used in the game in any case.) \|- \|
0x65 \|\| 0x02 \|\| The in-game month; for example, June is 0x0007 .
Used only for displaying on the load/save screen. \|- \| 0x67 \|\| 0x02
\|\| The in-game day, also used only for the load/save screen. \|- \|
0x69 \|\| 0x02 \|\| The in-game year, also used only for the load/save
screen. \|- \| 0x6B \|\| 0x04 \|\| The in-game date. This field stores
the time – the number of seconds since 25.07.2241 0:00:00 multiplied by
10 (the number of 'ticks'). [Fallout 1 starts at December 5th 2161] \|-
\| 0x6F \|\| 0x04 \|\| Number of the map you are currently on (from
maps.txt). First two bytes are map level, last two are map number. \|-
\| 0x73 \|\| 0x10 \|\| Filename of the map you are currently on. I.e. if
you are in the Arroyo temple, it is ARTEMPLE.sav. An ASCII string. \|-
\| 0x83 \|\| 0x7460 \|\| This is a bitmap (bmp) image, with no header or
palette. The palette is taken from master.dat or patch000.dat. Image
size is 224x133. \|- \| 0x74E3 \|\| 0x80 \|\| This section is always
filled with zeroes. \|}

===Function 0 - Unused=== Size: 0x00 (decimal 0) bytes. This function
never writes anything.

===Function 1 - Unknown=== Size: 0x04 (decimal 4) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x7563 \|\| 0x04 \|\| Unknown \|}

===Function 2 - GVARS=== Size: variable. In Fallout 2 v1.02d it is
0x0AE0 (decimal 2784) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x7567 \|\| 0x04 \* NG \|\| An array of global variables (GVAR).
NG is the number of GVARs in vault13.gam . \|}

===Function 3 - Maps=== Size: variable.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x8047 \|\| 0x04 \|\| The number of open maps. Also the number of
\*.sav files in the save folder. \|- \| 0x804B \|\| variable \|\| The
size of the next field varies depending on the number of opened maps. It
is a list of file names of maps which have already been visited when the
game is saved. These are ASCII strings, each ending with a 0x00 byte.
\|- \| after map list \|\| 0x04 \|\| The size of AUTOMAP.DB - the
temporary file game creates while playing in the
data:raw-latex:`\maps `and removes after. \|}

===Function 4 - GVARS=== Size: variable. In Fallout 2 v1.02d it is
0x0AE0 (decimal 2784) bytes.

This is an exact duplicate of Function #2. It's not known why this was
done.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \* NG \|\| An array of global variables (GVAR). NG
is the number of GVARs in vault13.gam . \|}

===Function 5 - Player and inventory=== Size: variable. This appears to
be in the same format used for [[MAP File Format#MAP\_Objects \|objects
on maps]].

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \|\| 0x0,0x0,'FP' [0x00004650] - these characters
are always here. Also, they appear only once in the file. They're
followed by a list of the player's characteristics. \|- \| 0x04 \|\|
0x04 \|\| Player's coordinates on the map. \|- \| 0x08 \|\| 0x04 \|\|
Unknown \|- \| 0x0C \|\| 0x04 \|\| Unknown \|- \| 0x10 \|\| 0x04 \|\|
Unknown \|- \| 0x14 \|\| 0x04 \|\| Unknown \|- \| 0x18 \|\| 0x04 \|\|
Unknown \|- \| 0x1C \|\| 0x04 \| Player's facing: 0 - northeast 1 - east
2 - southeast 3 - southwest 4 - west 5 - northwest \|- \| 0x20 \|\| 0x04
\|\| Player FID number (appearance). Changes depending on what armor is
worn. [0100000B] is the jumpsuit. Note that the engine will always
change this value back to what you are wearing, so it's pointless to
edit it here. \|- \| 0x24 \|\| 0x04 \|\| Unknown, usually 0x60002420.
The last byte has some interesting properties. Setting the 0x10 bit puts
the player in a permanent, always-successful sneak mode. Setting the
0x08 bit will draw the player under other critters and scenery. Setting
the last byte to 00 will turn off the player light source. \|- \| 0x28
\|\| 0x04 \|\| Which level of the map the player is on. \|- \| 0x2C \|\|
0x04 \|\| Unknown \|- \| 0x30 \|\| 0x18 \|\| Unknown - Light distance
and intensity? Some of it have to to with lighting up the player. \|- \|
0x48 \|\| 0x04 \|\| Number of items in inventory. \|- \| 0x4C \|\| 0x18
\|\| Unknown. \|- \| 0x64 \|\| 0x04 \|\| Bit field. Each bit indicates a
crippled body part: 0x0001 - eyes 0x0002 - right arm 0x0004 - left arm
0x0008 - right leg 0x0010 - left leg 0x0080 - dead \|- \| 0x68 \|\| 0xC
\|\| Unknown \|- \| 0x74 \|\| 0x04 \|\| Current hitpoints. \|- \| 0x78
\|\| 0x04 \|\| Radiation level. \|- \| 0x7C \|\| 0x04 \|\| Poison level.
\|- \| 0x80 \|\| variable \|\| A list of [[SAVE.DAT File
Format#Inventory item format \|items in inventory]]. \|}

===Function 6 - Player stats=== Size: 0x0178 (decimal 376) bytes.

This section has the same format as [[PRO File Format \|PRO]] files for
critters, starting at offset 0x30 in the PRO file.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \| 0x04 \| Unknown \|- \| 0x04 \| 0x04 \| 0x07 = Tabs flags.
Bitmask: 0x01 - Sneak, 0x02 - Radiated, 0x08 - Level, 0x10 - Addict. \|-
\| 0x08 \| 0x04 \| Base strength. \|- \| 0x0C \| 0x04 \| Base
perception. \|- \| 0x10 \| 0x04 \| Base endurance. \|- \| 0x14 \| 0x04
\| Base charisma. \|- \| 0x18 \| 0x04 \| Base intelligence. \|- \| 0x1C
\| 0x04 \| Base agility. \|- \| 0x20 \| 0x04 \| Base luck. \|- \| 0x24
\| 0x04 \| Base hitpoints. \|- \| 0x28 \| 0x04 \| Base action points.
\|- \| 0x2C \| 0x04 \| Base armor class. \|- \| 0x30 \| 0x04 \| Unused.
In define.h, it was unarmed damage, but that is commented out. \|- \|
0x34 \| 0x04 \| Base melee damage. \|- \| 0x200 \| 0x04 \| Base carry
weight. \|- \| 0x3C \| 0x04 \| Base sequence. \|- \| 0x40 \| 0x04 \|
Base healing rate. \|- \| 0x44 \| 0x04 \| Base critical chance. \|- \|
0x48 \| 0x04 \| Base critical hit table roll modifier. \|- \| 0x4C \|
0x04 \| Base damage threshold (normal) \|- \| 0x50 \| 0x04 \| Base
damage threshold (laser) \|- \| 0x54 \| 0x04 \| Base damage threshold
(fire) \|- \| 0x58 \| 0x04 \| Base damage threshold (plasma) \|- \| 0x5C
\| 0x04 \| Base damage threshold (electrical) \|- \| 0x60 \| 0x04 \|
Base damage threshold (EMP) \|- \| 0x64 \| 0x04 \| Base damage threshold
(explosive) \|- \| 0x68 \| 0x04 \| Base damage resistance (normal) \|-
\| 0x6C \| 0x04 \| Base damage resistance (laser) \|- \| 0x70 \| 0x04 \|
Base damage resistance (fire) \|- \| 0x74 \| 0x04 \| Base damage
resistance (plasma) \|- \| 0x78 \| 0x04 \| Base damage resistance
(electrical) \|- \| 0x7C \| 0x04 \| Base damage resistance (EMP) \|- \|
0x80 \| 0x04 \| Base damage resistance (explosive) \|- \| 0x84 \| 0x04
\| Base radiation resistance \|- \| 0x88 \| 0x04 \| Base poison
resistance \|- \| 0x8C \| 0x04 \| Player's starting age (i.e. at the
beginning of the game). \|- \| 0x90 \| 0x04 \| Player's gender. 0 is
male, 1 (or non-zero) is female \|- ! colspan="3" \| Attention: Here are
bonuses to basic statistics, starting with strength. For example, if you
have strength 6 and are wearing Advanced Power Armor, 4 is recorded
here, and 6 in base strength. Penalties can be given with negative
numbers. \|- \| 0x94 \| 0x04 \| Bonus to strength \|- \| 0x98 \| 0x04 \|
Bonus to perception. \|- \| 0x9C \| 0x04 \| Bonus to endurance. \|- \|
0xA0 \| 0x04 \| Bonus to charisma. \|- \| 0xA4 \| 0x04 \| Bonus to
intelligence. \|- \| 0xA8 \| 0x04 \| Bonus to agility. \|- \| 0xAC \|
0x04 \| Bonus to luck. \|- \| 0xB0 \| 0x04 \| Bonus to maximum hit
points. \|- \| 0xB4 \| 0x04 \| Bonus action points. \|- \| 0xB8 \| 0x04
\| Bonus AC. \|- \| 0xBC \| 0x04 \| Unused. In define.h, it was unarmed
damage, but that is commented out. \|- \| 0xC0 \| 0x04 \| Bonus melee
damage. \|- \| 0xC4 \| 0x04 \| Bonus carry weight. \|- \| 0xC8 \| 0x04
\| Bonus sequence. \|- \| 0xCC \| 0x04 \| Bonus healing rate. \|- \|
0xD0 \| 0x04 \| Bonus critical chance. \|- \| 0xD4 \| 0x04 \| Bonus to
critical hit table rolls. \|- \| 0xD8 \| 0x04 \| Bonus to damage
threshold (normal). \|- \| 0xDC \| 0x04 \| Bonus to damage threshold
(laser). \|- \| 0xE0 \| 0x04 \| Bonus to damage threshold (fire). \|- \|
0xE4 \| 0x04 \| Bonus to damage threshold (plasma). \|- \| 0xE8 \| 0x04
\| Bonus to damage threshold (electrical) \|- \| 0xEC \| 0x04 \| Bonus
to damage threshold (EMP) \|- \| 0xF0 \| 0x04 \| Bonus to damage
threshold (explosive). \|- \| 0xF4 \| 0x04 \| Bonus to damage resistance
(normal). \|- \| 0xF8 \| 0x04 \| Bonus to damage resistance (laser). \|-
\| 0xFC \| 0x04 \| Bonus to damage resistance (fire). \|- \| 0x0100 \|
0x04 \| Bonus to damage resistance (plasma). \|- \| 0x0104 \| 0x04 \|
Bonus to damage resistance (electrical). \|- \| 0x0108 \| 0x04 \| Bonus
to damage resistance (EMP). \|- \| 0x010C \| 0x04 \| Bonus to damage
resistance (explosive). \|- \| 0x0110 \| 0x04 \| Bonus to radiation
resistance. \|- \| 0x0114 \| 0x04 \| Bonus to poison resistance. \|- \|
0x0118 \| 0x04 \| Bonus to age. \|- \| 0x011C \| 0x04 \| Additional
gender. Apparently sex-change operations are still available in the
post-nuclear world. \|- \| 0x0120 \| 0x04 \* 0x12 \| [[SAVE.DAT File
Format#List of skills \|Skills]]; value is the number of points over the
base (from primary stats, traits, and perks). \|- \| 0x0168 \| 0x04 \|
Proto ID (-0x0400) \|- \| 0x016C \| 0x08 \| Unknown \|- \| 0x174 \| 0x04
\| Value is X, where 0x1450+X is the the line describing that type of
critter in Proto.msg . \|}

===Function 7 - Kill counts=== Size: 0x4C (decimal 76) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \* 0x13 \|\| Count of killed [[SAVE.DAT File
Format#List of critter types \|critters]], same order as on the
character sheet. \|}

===Function 8 - Tag skills=== Size: 0x10 (decimal 16) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \* 0x04 \|\| Tag [[SAVE.DAT File Format#List of
skills \|skills]]; -1 if unused. \|}

===Function 9 - Perks=== Size: 0x02C8 (decimal 712) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \* 0xB2 \|\| [[SAVE.DAT File Format#List of perks
\|Perks]]. Note that some have an effect only when they are taken, so
changing values here may not do anything. \|}

===Function 10 - Party=== Size: variable. In Fallout 2 v1.02d it is
0x3058 (decimal 12376) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \* 0x77 \* NP \|\| Where NP is the number of party
members from party.txt . Contains 119 (0x77) values for each party
member. \|}

===Function 11 - Combat=== Size: 0x04 (decimal 4) when not in combat;
variable when in combat.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \|\| Combat status. (02 - not in combat; 03 - in
combat) \|- \| *0x04 \|\| 0x10 \|\| Unknown. \|- \| *\ 0x14 \|\| 0x04
\|\| Unknown. \|- \| *0x18 \|\| 0x04 \|\| Number of NPC at the current
map level, including party members, plus a player at the beginning of
the battle (or perhaps corpses are counted too). \|- \| *\ 0x1С \|\|
0x04 \|\| Perhaps player's ID. \|- \| *0x20 \|\| 0x04*\ NR \|\| NR from
offset 0x18. Perhaps this array indicates the turn order of NPCs and
player. \|- \| *0x20+0x4*\ NR \|\| 0x10\ *NR \|\| Unknown. \|} Data
marked * only present when in combat.

===Function 12 - Party AI=== Size: variable.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| variable \|\| Records about party members from
"AI.TXT". The number of records varies. The organization of these
records is unclear: they can be duplicated, or disappear in later
savegames. A description of them is given [[SAVE.DAT File Format#Party
member AI record format \|below]]. Each record is 0xB4 bytes long, so
the size is a multiple of that. \|}

===Function 13 - Experience and level=== Size: 0x14 (decimal 20) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \|\| Number of skill points; should be less than
100. \|- \| 0x04 \|\| 0x04 \|\| Level. \|- \| 0x08 \|\| 0x04 \|\|
Experience. \|- \| 0x0C \|\| 0x08 \|\| Unknown. \|}

===Function 14 - Unused=== Size: 0x00 (decimal 0) bytes. This function
never writes anything.

===Function 15 - Traits=== Size: 0x08 (decimal 8) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \* 0x02 \|\| [[SAVE.DAT File Format#List of traits
\|Traits]]; -1 if unused. \|}

===Function 16 - Unknown=== Size: 0x04 (decimal 4) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \|\| Unknown \|}

===Function 17 - Preferences=== Size: 0x50 (decimal 80) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \| game\_difficulty values: 0 (easy), 1 (normal),
2 (hard) default: 1 \|- \| 0x04 \|\| 0x04 \| combat\_difficulty values:
0 (wimpy), 1 (normal), 2 (rough) default: 1 \|- \| 0x08 \|\| 0x04 \|
violence\_level values: 0 (none), 1 (minimal), 2 (normal), 3 (maximum
blood) default: 3 \|- \| 0x0C \|\| 0x04 \| target\_highlight values: 0
(off), 1 (on), 2 (targeting only) default: 2 \|- \| 0x10 \|\| 0x04 \|
combat\_looks values: 0 (off), 1 (on) default: 0 \|- \| 0x14 \|\| 0x04
\| combat\_messages values: 0 (brief), 1 (verbose) default: 1 \|- \|
0x18 \|\| 0x04 \| combat\_taunts values: 0 (off), 1 (on) default: 1 \|-
\| 0x1C \|\| 0x04 \| language\_filter values: 0 (off), 1 (on) default: 0
\|- \| 0x20 \|\| 0x04 \| running values: 0 (normal), 1 (always) default:
0 \|- \| 0x24 \|\| 0x04 \| subtitles values: 0 (off), 1 (on) default: 0
\|- \| 0x28 \|\| 0x04 \| item\_highlight values: 0 (off), 1 (on)
default: 1 \|- \| 0x2C \|\| 0x04 \| combat\_speed values: 0 (normal) -
50 (fastest) default: 0 \|- \| 0x30 \|\| 0x04 \| player\_speedup (affect
player speed) values: 0 (off), 1 (on) default: 0 \|- \| 0x34 \|\| 0x04
\| text\_base\_delay values: 1.0 (fastest) - 6.0 (slowest) default: 3.5
(normal) \|- \| 0x38 \|\| 0x04 \| master\_volume values: 0 (off) - 32767
(loudest) default: 22281 \|- \| 0x3C \|\| 0x04 \| music\_volume values:
0 (off) - 32767 (loudest) default: 22281 \|- \| 0x40 \|\| 0x04 \|
sndfx\_volume values: 0 (off) - 32767 (loudest) default: 22281 \|- \|
0x44 \|\| 0x04 \| speech\_volume values: 0 (off) - 32767 (loudest)
default: 22281 \|- \| 0x48 \|\| 0x04 \| brightness values: 1.0 (normal)
- 1.18 (brightest) default: 1.0 \|- \| 0x4C \|\| 0x04 \|
mouse\_sensitivity values: 0.5 (normal) - 2.5 (fastest) default: 1.0 \|}

A 'text\_line\_delay' variable also exists, but doesn't seem to be
stored here.

The Mac version has a "1" value between speech volume and brightness;
its meaning is unknown.

===Function 18 - Character window=== Size: 0x05 (decimal 5) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \|\| Level. When you enter the character window,
this level becomes equal to the level from Function 13, and you get
skill points (LevelFromFunction13-LevelFromFunction18) \* SpPerLevel.
\|- \| 0x04 \|\| 0x01 \|\| Indicates if you have any outstanding perks
you may have saved for later. \|}

===Function 19 - World map=== Size: variable. In Fallout 2 v1.02d it is
0x0AE0 (decimal 2784) bytes.

The contents of this function are the same as worldmap.dat.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x8 \|\| Unknown \|- \| 0x08 \|\| 0x04 \|\| Current
position on world map (X coordinate) \|- \| 0x0C \|\| 0x04 \|\| Current
position on world map (Y coordinate) \|- \| 0x10 \|\| 0x14 \|\| Unknown
\|- \| 0x24 \|\| 0x04 \|\| Number of the map the car is on. \|- \| 0x28
\|\| 0x04 \|\| Amount of fuel in the car (maximum is 80,000). \|- \|
0x2C \|\| 0x04 \|\| The number of cities (from city.txt) \|- \| 0x30
\|\| 0x654 \|\| [[SAVE.DAT File Format#City record format \|City
information]] (from city.txt). The number of records matches the number
of cities (49). \|- \| 0x684 \|\| 0x04 \|\| Number if tiles on the world
map (from worldmap.txt) \|- \| 0x688 \|\| 0x04 \|\| Value of
num\_horizontal\_tiles (from worldmap.txt) \|- \| 0x68C \|\| 0xD20 \|\|
Data for world map 'fog'. All 840 squares (each tile is 7x6 squares;
there are 20 tiles) have corresponding 4-byte values: 0x00 - hidden,
0x01 - partly visible, 0x02 - visible. \|- \| 0x13AC \|\| 0x04 \|\| The
number of Special Encounters (373). \|- \| 0x13B0 \|\| 0x117C \|\|
Records of [[SAVE.DAT File Format#Special encounter record format
\|Special encounters]]. \|}

===Function 20 - Unused=== Size: 0x00 (decimal 0) bytes. This function
never writes anything.

===Function 21 - Video Archives=== Size: 0x11 (decimal 17) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x11 \|\| First two bytes are iplogo and intro, and the
last is for credits. None of these show up in the Video Archives screen.

The rest each add a video entry to the Video Archives in the Pipboy. 00
is OFF and 01 is ON. \|}

===Function 22 - Skill use=== Size: 0xD8 (decimal 216) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x48 \|\| Unknown; there's space for all 18 skills, but
only First Aid and Doctor matter \|- \| 0x48 \|\| 3 \* 0x04 \|\| The
times of the last three uses of the First Aid skill. Stored in 'ticks'
since the start of the game. \|- \| 0x54 \|\| 3 \* 0x04 \|\| The times
of the last three uses of the Doctor skill. \|- \| 0x60 \|\| 0x78 \|\|
Unknown \|}

===Function 23 - Party=== Size: variable.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \|\| Number of NPCs in the party \|- \| 0x04 \|\|
0x04 \* CP \|\| Where CP is the number of party members. Unknown \|- \|
0x? \|\| 0x12 \* (CP-1) \|\| Where CP is the number of party members -
(CP-1) excludes the player (party member 0). Unknown \|}

===Function 24 - Event queue=== Size: variable.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| variable \|\| Unknown \|}

===Function 25 - Interface=== Size: 0x10 (decimal 16) bytes.

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \| 0x04 \| Description: The state of the user interface.
Similar to the functions (game\_ui\_disable \| game\_ui\_enable), but
unlike them it does not prevent loading and saving games.Values: 0x01 -
enabled; 0x0 - disabled \|- \| 0x04 \| 0x04 \| Description:State of the
player panel (with the weapon 'button', pip-boy, etc.) Values: 0x01 -
enabled; 0x0 - disabled \|- \| 0x08 \| 0x04 \| 0x0B = Active hand slot,
00 for slot 1, 01 for slot 2. Rest unknown. \|- \| 0x0C \| 0x04 \|State
of the combat panel (open/closed). \|}

===Function 26 - Unused=== Size: 0x00 (decimal 0) bytes. This function
never writes anything.

==Inventory item format== This appears to be the same format as [[MAP
File Format#MAP Objects \|items on maps]]. (This is correct for the Mac
version of FO2; the PC version may be slightly different.)

{\| class="wikitable" border="1" ! Offset ! Size (bytes) ! Description
\|- \| 0x00 \|\| 0x04 \|\| The quantity of this item. \|- \| 0x04 \|\|
0x04 \|\| Always 0. \|- \| 0x08 \|\| 0x04 \|\| Location of item.
Necessarily always -1 (in inventory) \|- \| 0x0C \|\| 0x04 \|\| Always
0. \|- \| 0x10 \|\| 0x04 \|\| Always 0. \|- \| 0x14 \|\| 0x04 \|\|
Unknown, 0 for most objects. Only non-zero for some items with
special/quest uses. (E.g. poison tank, rubber boots, tool). \|- \| 0x18
\|\| 0x04 \|\| Unknown, like field 0x14, only nonzero for items with
special uses. \|- \| 0x1C \|\| 0x04 \|\| Unknown, always 0. \|- \| 0x20
\|\| 0x04 \|\| Unknown, usually 0. For plant spikes, this is 1, and for
sharpened poles, 2. \|- \| 0x24 \|\| 0x04 \|\| Item's on-ground image.
\|- \| 0x28 \|\| 0x04 \|\| A bitfield: 0x01000000 indicates the item is
held in the right hand, 0x02000000 - in the left hand, and 0x04000000 -
worn (as armor). Use of other bits is unknown. \|- \| 0x2C \|\| 0x04
\|\| 0, 1, or 2. Map level, which is irrelevant since it's in inventory.
\|- \| 0x30 \|\| 0x04 \|\| Object ID. \|- \| 0x34 \|\| 0x04 \|\| Always
-1. \|- \| 0x38 \|\| 0x04 \|\| For Roentgen Rum and Gamma Gulp Beer,
this is 0x00000001, for all others 0. \|- \| 0x3C \|\| 0x04 \|\| For
Roentgen Rum and Gamma Gulp Beer, this is 0x00010000, for all others 0.
\|- \| 0x40 \|\| 0x04 \|\| Always 0. \|- \| 0x44 \|\| 0x04 \|\| Script
ID; -1 for none. \|- \| 0x48 \|\| 0x04 \|\| Always -1. \|- \| 0x4C \|\|
0x04 \|\| Usually 0; for containers it is the number of items in the
container. The enclosed items are the ones immediately following it. \|-
\| 0x50 \|\| 0x04 \|\| Unknown. Non-zero only for containers. \|- \|
0x54 \|\| 0x04 \|\| Unknown. Non-zero only for containers. \|- \| 0x58
\|\| 0x04 \|\| Always 0. \|- \| 0x5C \|\| 0x04 \|\| Many uses. For
weapons, the number of loaded rounds. For ammunition, the number of
rounds in the 'top' package (the one that is partly full). Number of
charges for geiger counters/motion sensors. May be a script ID in some
cases, like 0x44. For everything else, it is usually -1 or 0xCCCCCCCC.
Not present for armor, containers, or drugs. \|- \| 0x60 \|\| 0x04 \|\|
For weapons, the type of ammunition it uses; -1 if it needs no ammo.
Only present for weapons. \|}

==City record format== {\| class="wikitable" border="1" ! Offset ! Size
(bytes) ! Description \|- \| 0x00 \|\| 0x04 \|\| City coordinate (X) \|-
\| 0x04 \|\| 0x04 \|\| City coordinate (Y) \|- \| 0x08 \|\| 0x04 \|\|
State of the city on the world map (accessible or not) \|- \| 0x0C \|\|
0x04 \|\| City button on world map (accesssible or not) \|- \| 0x10 \|\|
0x04 \|\| Number of entrances \|- \| 0x14 \|\| 0x04 \* number of
entrances \|\| entrance\_X=On/Off \|}

==Special encounter record format== {\| class="wikitable" border="1" !
Offset ! Size (bytes) ! Description \|- \| 0x00 \|\| 0x04 \|\| Encounter
Table number \|- \| 0x04 \|\| 0x04 \|\| enc\_X (number in the Encounter
Table) \|- \| 0x08 \|\| 0x04 \|\| Already encountered (or not) \|}

==Party member AI record format== {\| class="wikitable" border="1" !
Offset ! Size (bytes) ! Description \|- \| 0x00 \|\| 0x04 \|\|
packet\_num \|- \| 0x04 \|\| 0x04 \|\| max\_dist \|- \| 0x08 \|\| 0x04
\|\| min\_to\_hit \|- \| 0x0C \|\| 0x04 \|\| min\_hp \|- \| 0x10 \|\|
0x04 \|\| aggression \|- \| 0x14 \|\| 0x04 \|\| hurt\_too\_much \|- \|
0x18 \|\| 0x04 \|\| secondary\_freq \|- \| 0x1C \|\| 0x04 \|\|
called\_freq \|- \| 0x20 \|\| 0x04 \|\| font \|- \| 0x24 \|\| 0x04 \|\|
color \|- \| 0x28 \|\| 0x04 \|\| outline\_color \|- \| 0x2C \|\| 0x04
\|\| chance \|- \| 0x30 \|\| 0x04 \|\| run\_start \|- \| 0x34 \|\| 0x04
\|\| run\_end \|- \| 0x38 \|\| 0x04 \|\| move\_start \|- \| 0x3C \|\|
0x04 \|\| move\_end \|- \| 0x40 \|\| 0x04 \|\| attack\_start \|- \| 0x44
\|\| 0x04 \|\| attack\_end \|- \| 0x48 \|\| 0x04 \|\| miss\_start \|- \|
0x4C \|\| 0x04 \|\| miss\_end \|- \| 0x50 \|\| 0x04 \|\|
hit\_head\_start \|- \| 0x54 \|\| 0x04 \|\| hit\_head\_end \|- \| 0x58
\|\| 0x04 \|\| hit\_left\_arm\_start \|- \| 0x5C \|\| 0x04 \|\|
hit\_left\_arm\_end \|- \| 0x60 \|\| 0x04 \|\| hit\_right\_arm\_start
\|- \| 0x64 \|\| 0x04 \|\| hit\_right\_arm\_end \|- \| 0x68 \|\| 0x04
\|\| hit\_torso\_start \|- \| 0x6C \|\| 0x04 \|\| hit\_torso\_end \|- \|
0x70 \|\| 0x04 \|\| hit\_right\_leg\_start \|- \| 0x74 \|\| 0x04 \|\|
hit\_right\_leg\_end \|- \| 0x78 \|\| 0x04 \|\| hit\_left\_leg\_start
\|- \| 0x7C \|\| 0x04 \|\| hit\_left\_leg\_end \|- \| 0x80 \|\| 0x04
\|\| hit\_eyes\_start \|- \| 0x84 \|\| 0x04 \|\| hit\_eyes\_end \|- \|
0x88 \|\| 0x04 \|\| hit\_groin\_start \|- \| 0x8C \|\| 0x04 \|\|
hit\_groin\_end+1 \|- \| 0x90 \|\| 0x04 \|\| area\_attack\_mode \|- \|
0x94 \|\| 0x04 \|\| best\_weapon \|- \| 0x98 \|\| 0x04 \|\| distance \|-
\| 0x9C \|\| 0x04 \|\| attack\_who \|- \| 0xA0 \|\| 0x04 \|\| chem\_use
\|- \| 0xA4 \|\| 0x04 \|\| run\_away\_mode \|- \| 0xA8 \|\| 0x04 \|\|
Unknown \|- \| 0xAC \|\| 0x04 \|\| Unknown \|- \| 0xB0 \|\| 0x04 \|\|
Unknown \|}

==List of skills==

.. raw:: html

   <pre>
   0x00: Small Guns
   0x01: Big Guns
   0x02: Energy Weapons
   0x03: Unarmed
   0x04: Melee Weapons
   0x05: Throwing
   0x06: First Aid
   0x07: Doctor
   0x08: Sneak
   0x09: Lockpick
   0x0A: Steal
   0x0B: Traps
   0x0C: Science
   0x0D: Repair
   0x0E: Speech
   0x0F: Barter
   0x10: Gambling
   0x11: Outdoorsman
   </pre>

==List of critter types==

.. raw:: html

   <pre>
   0x00: Men
   0x01: Women
   0x02: Children
   0x03: Super mutants
   0x04: Ghouls
   0x05: Brahmin
   0x06: Radscorpions
   0x07: Rats
   0x08: Floaters
   0x09: Centaurs
   0x0a: Robots
   0x0b: Dogs
   0x0c: Manti
   0x0d: Deathclaws
   0x0e: Plants
   0x0f: Geckos
   0x10: Aliens
   0x11: Giant ants
   0x12: Big Bad Boss
   </pre>

==List of perks== Note: Starred (\*) perks change other stats when they
are chosen in-game, and have no effect in and of themselves. Daggers (†)
indicate special perks that are used by the engine and not normally seen
on the character screen.

.. raw:: html

   <pre>
   0x00: Awareness
   0x01: Bonus HtH Attacks
   0x02: Bonus HtH Damage*
   0x03: Bonus Move
   0x04: Bonus Ranged Damage
   0x05: Bonus Rate of Fire
   0x06: Earlier Sequence*
   0x07: Faster Healing*
   0x08: More Criticals*
   0x09: Night Vision
   0x0A: Presence
   0x0B: Rad Resistance
   0x0C: Toughness*
   0x0D: Strong Back*
   0x0E: Sharpshooter
   0x0F: Silent Running
   0x10: Survivalist
   0x11: Master Trader
   0x12: Educated
   0x13: Healer
   0x14: Fortune Finder
   0x15: Better Criticals*
   0x16: Empathy
   0x17: Slayer
   0x18: Sniper
   0x19: Silent Death
   0x1A: Action Boy*
   0x1B: Mental Block (FO1 only)
   0x1C: Lifegiver
   0x1D: Dodger*
   0x1E: Snakeeater
   0x1F: Mr. Fixit
   0x20: Medic
   0x21: Master Thief
   0x22: Speaker
   0x23: Heave Ho!
   0x24: Unimplemented, DO NOT USE! (Friendly Foe in FO1)
   0x25: Pickpocket
   0x26: Ghost
   0x27: Cult of Personality
   0x28: Scrounger (FO1 only)
   0x29: Explorer
   0x2A: Flower Child (FO1 only)
   0x2B: Pathfinder
   0x2C: Animal Friend (FO1 only)
   0x2D: Scout
   0x2E: Mysterious Stranger
   0x2F: Ranger
   0x30: Quick Pockets
   0x31: Smooth Talker
   0x32: Swift Learner
   0x33: Tag!*
   0x34: Mutate!*
   0x35: Nuka-Cola Addiction
   0x36: Buffout Addiction
   0x37: Mentats Addiction
   0x38: Psycho Addiction
   0x39: Radaway Addiction
   0x3A: Weapon Long Range†
   0x3B: Weapon Accurate†
   0x3C: Weapon Penetrate†
   0x3D: Weapon Knockback†
   0x3E: Powered Armor†
   0x3F: Combat Armor†
   0x40: Weapon Scope range†
   0x41: Weapon Fast reload†
   0x42: Weapon Night sight†
   0x43: Weapon Flameboy†
   0x44: Armor Advanced I†
   0x45: Armor Advanced II†
   0x46: Jet Addiction
   0x47: Tragic Addiction
   0x48: Armor Charisma†
   0x49: Gecko Skinning
   0x4A: Dermal Impact Armor
   0x4B: Dermal Impact Assault Enhancements(*)
   0x4C: Phoenix Armor Implants
   0x4D: Phoenix Assault Enhancements(*)
   0x4E: Vault City Inoculations
   0x4F: Adrenaline Rush
   0x50: Cautious Nature
   0x51: Comprehension
   0x52: Demolition Expert
   0x53: Gambler
   0x54: Gain Strength
   0x55: Gain Perception
   0x56: Gain Endurance
   0x57: Gain Charisma
   0x58: Gain Intelligence
   0x59: Gain Agility
   0x5A: Gain Luck
   0x5B: Harmless
   0x5C: Here and Now*
   0x5D: HtH Evade
   0x5E: Kama Sutra Master
   0x5F: Karma Beacon
   0x60: Light Step
   0x61: Living Anatomy
   0x62: Magnetic Personality
   0x63: Negotiator
   0x64: Pack Rat*
   0x65: Pyromaniac
   0x66: Quick Recovery
   0x67: Salesman
   0x68: Stonewall
   0x69: Thief
   0x6A: Weapon Handling
   0x6B: Vault City Training
   0x6C: Alcohol Raised Hit Points
   0x6D: Alcohol Raised Hit Points II
   0x6E: Alcohol Lowered Hit Points
   0x6F: Alcohol Lowered Hit Points II
   0x70: Autodoc Raised Hit Points
   0x71: Autodoc Raised Hit Points II
   0x72: Autodoc Lowered Hit Points
   0x73: Autodoc Lowered Hit Points II
   0x74: Expert Excrement Expeditor
   0x75: Weapon Enhanced Knockout†
   0x76: Jinxed
   </pre>

(\*) - The charisma loss from the assault implants is applied when you
get them, but is not part of the perk.

==List of traits==

.. raw:: html

   <pre>
   0x00: Fast Metabolism
   0x01: Bruiser
   0x02: Small Frame
   0x03: One Hander
   0x04: Finesse
   0x05: Kamikaze
   0x06: Heavy Handed
   0x07: Fast Shot
   0x08: Bloody Mess
   0x09: Jinxed
   0x0A: Good Natured
   0x0B: Chem Reliant
   0x0C: Chem Resistant
   0x0D: Sex Appeal (In Fallout 1 this was Night Person)
   0x0E: Skilled
   0x0F: Gifted
   -1: (none)
   </pre>

==TeamX document==
[http://sites.google.com/site/chulancheg/Fallout2\_SAVE\_DAT.htm Copy of
original document] (in russian) [[Category:Fallout and Fallout 2 file
formats]][[Category:Translated TeamX documents]] [[Category:Fallout 2
documentation]]
