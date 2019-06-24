========
CITY.TXT
========

CITY.TXT contains data about locations on the world map. This includes
both fixed locations, such as cities, and random encounters that are not
saved.

The file is a text format and is divided into tables describing
locations. Each table contains a list of variables which have values of
various types – text, numerical, combined. The order of variables in the
list is usually the same, but not always, so the order may not matter.
As with several other configuration files, a semicolon comments out the
rest of the line it's on.

'''1. Description of location tables'''

Each table starts with a header in the format:

[Area 01]

This line marks the beginning of the table, and also gives it a unique
ID number. There can be up to 100 areas, numbered 00 through 99.

Immediately after the header is a list of variables.

'''2. List of Variables'''

Each variable has the format var\_name=value ; no spaces are used except
in text strings.

-  ''area\_name'' - name of the location described in the table.
   Necessary parameter.

-  ''world\_pos'' - position of this city on the world map. Format is
   x\_pos,y\_pos ; calculating the value is a little tricky. Take the
   coordinates of the upper-left corner of the a square bounding box
   around the city circle, and add 23,22 to it. This will be at the
   center of a large circle, the lower-right corner of a medium circle,
   and some distance away from a small circle. Necessary parameter.

-  ''start\_state'' - whether or not this city is visible at the
   beginning of the game. Values are 'On' and 'Off'. Necessary
   parameter, possibly (at least, it's always used).

-  ''size'' - the size of the circle to draw on the world map. Values
   are 'Large', 'Medium', and 'Small'. Necessary parameter unless
   lock\_state is set to 'On'.

-  ''lock\_state'' - whether or not to save the location of this city
   and mark it on the map. If set to 'On', the location is not saved. If
   set to 'Off', the location is saved. Not necessary; the default is
   'Off' if not present.

-  ''townmap\_art\_idx'' - the image to use when selecting an entrance
   to the city. Value is a line number from intrface.lst . Necessary
   parameter; -1 indicates no image is used.

-  ''townmap\_label\_art'' - the image to use in the list of known
   cities on the world map screen. Value is a line number from
   intrface.lst . Necessary parameter; -1 indicates no image is used.

-  ''entrance\_n'' - an entrance to the city. There can be up to ten
   (numbered 0-9), and there must be no gaps in the sequence. Format is
   ''start\_state,x\_pos,y\_pos,map\_index,elevation,tile\_num,orientation''
   . Start\_state is whether or not this entrance is initially
   available; values are 'On' and 'Off'. X\_pos and y\_pos indicate the
   position of the entrance triangle on the city map screen, measured in
   pixels from the upper-left corner. Map\_index matches the value of
   the lookup\_name variable in the record for the map to go to in
   maps.txt . Elevation, tile\_num, and orientation indicate where the
   player appears on the map.

[[Category:Fallout and Fallout 2 file formats]]
