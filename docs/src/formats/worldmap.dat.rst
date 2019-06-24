============
WORLDMAP.DAT
============

Description file format WORLDMAP.DAT

==Introduction==

At the moment, this is the most controversial issue. There are several
versions, but unequivocally say that this is indeed the case impossible.
But you can only select one. Over time, this text can be modified
slightly (to confirm or supplement to this release).

This file is a unique cache. As proof of that, you can say that every
time preserving Fallout drawn to the file. Maybe checks data. This is
also clear from the logical point of view. Better to work with the file
size at 9 KB than to read every file of 400 MB. Moreover, this file is
modified to change the basic work on the files to the map. So, he
constantly displays exact figures on the world map. All these signs are
very similar to the cache. However, the data stored in it, and standing
within the game will not be affected. Perhaps the developers have
created this file to speed up work on the weaknesses of computers.
Indeed, during the game, we very often come at a map of the world.
Another confirmation of this release. Alignment of data between
worldmap.dat and save.dat. In the latter has all the data from
worldmap.dat. Accordingly, they display the data on the world map. It is
possible that this data is recorded originally from worldmap.dat. Also
during the game Fallout from the assistance of the data follows the
integrity of the game. Indeed file worldmap.dat not overwritten during
work with a map of peace. And if major changes to files (city.txt,
worldmap.txt), it will be noticeable, compared with the file created
when running the game.

==Structure==

All data are for the original version of the game with a patch 1.02d.
The inclusion of this file in the game engine can be found (Fargus):

{\| border="1" ! Offset !! Meaning \|- \| 0xFD470 \|\| wb .....
worldmap.dat \|}

Accordingly, you can change the file name. Tak same can be turned off
worldmap.dat creating files when starting the game. To do so, simply
change the attribute wb (open for entries as a binary file) on rb (open
reading). Now worldmap.dat not file will be created when you start the
game. The consequences of such a step is not known, but as a result of
the testing problems do not arise.

As for the structure itself worldmap.dat. The file has three tables.
They are different data. And first and third tables in turn are divided
into subgroups. The columns are:

At the beginning of the file are data that does not take into account
the game engine. This line of city.txt with the following text (to be
exact, only coordinates are present): ; world\_pos = 173122; Absolute
position? ; SAVED From the look of it, the data were from the initial
stage of development of the game, when Arroyo has not yet been conceived
as a village is our favorite.

As soon as these data are values:

{\| border="1" ! Offset !! Type !! Value !! Description \|- \| 0x0020
\|\| int \|\| 13880 \|\| ?? \|- \| 0x0020 \|\| int \|\| 31 \|\| The
number of cities (from city.txt) \|}

Since then go running 3 tables (to the end of the file)

===Table 1=== Figures in the table are divided into groups. One group is
the characteristics of a city. Total number of the appropriate column.
city.txt cities from the example of one group:

{\| border="1" !! Offset !! Type !! Value !! Description \|- \| 0x0030
\|\| int \|\| B8 \|\| Coordinates city \|- \| 0x0030 \|\| int \|\| 85
\|\| Coordinates city \|- \| 0x0030 \|\| int \|\| 01 \|\| On
start\_state = (00-Off) \|- \| 0x0030 \|\| int \|\| 00 \|\| The name of
the city, with the button. On the world map in the top right (Live, No)
\|- \| 0x0040 \|\| int \|\| 05 \|\| Entrance number \|- \| (until the
end of group) \|\| int \|\| 01 \|\| On entrance\_X = (00-Off) \|}

===Table 2=== From beginning to the end of the table are data on the
"fog" card. "Fog" obscures visibility accessible area on the map.

{\| border="1" ! Offset !! Type !! Value !! Description \|- \| 0x0690
\|\| int \|\| 00 \|\| 00 is not visible, 01-part, perhaps 02-open \|}

===Table 3=== Responsible for the Special Encounter's file worldmap.txt.
The data are divided into groups. As Special Encounter can meet certain
taylah, respectively, whereas the data are divided into [Encounter Table
X].

{\| border="1" ! Offset !! Type !! Value !! Description \|- \| 0x013A0
\|\| int \|\| 175 \|\| col. Special Encounter's \|}

For example, one group:

{\| border="1" ! Offset \|\| Type \|\| Value \|\| Description \|- \|
0x013B0 \|\| int \|\| 06 \|\| [Encounter Table X] \|- \| 0x013B0 \|\|
int \|\| 19 \|\| enc\_X (where X is the number of order in the group)
\|- \| 0x013B0 \|\| int \|\| 01 \|\| met-no \|- \| 0x013B0 \|\| int \|\|
06 \|\| [Encounter Table X] \|- \| 0x013S0 \|\| int \|\| 1A \|\| enc\_X
(where X is the number of order in the group) \|- \| 0x013B0 \|\| int
\|\| 01 \|\| met-no \|} [[Category:Fallout and Fallout 2 file formats]]
