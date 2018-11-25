============
Worldmap.txt
============

Worldmap.txt is located in master.dat/data/ and contains information
about random encounters and terrain on the world map. The locations of
cities are stored in [[CITY.TXT File Format \| city.txt]]. Impassible
areas (i.e. ocean) are defined by [[MSK File Format \| .msk files]].

The comments refer to several features that do not exist. These should
be ignored and are not included in the excerpts here.

==Data Section== The file begins with some properties of the worldmap
system.

[Data] ; Encounter frequency percentages: Forced=100% ; This shouldn't
change Frequent=38% Common=22% Uncommon=12% Rare=4% None=0% ; This
shouldn't change

; Terrain types: ;terrain\_types=Desert:2, Mountain:4, City:1, Ocean:1
terrain\_types=Desert:1, Mountain:2, City:1, Ocean:1

The percentages are presumably the chance of an encounter occurring each
time the game checks for one. The frequency of these checks is believed
to be based on processor speed, not actual travel time or distance.

It isn't clear what the numbers next to the terrain types are for.
Possibly the time it takes to cross a square of that terrain type. The
commented out line may have been values left over from Fallout 1.

In Fallout 2 it takes 25 hours to cross a square of desert, city, or
ocean, and 32.5 hours (30% more) to cross a square of mountains. In
Fallout 1 it took 10 hours for desert and ocean, 20 for mountains, and 6
for city terrain.

;These determine which maps are randomly chosen from for normal random
encounters [Random Maps: Desert] map\_00=Desert Encounter 1
map\_01=Desert Encounter 2 ...

[Random Maps: Mountain] map\_00=Cavern Encounter 0 map\_01=Mountain
Encounter 1 ...

[Random Maps: City] map\_00=City Encounter 1 map\_01=City Encounter 2
...

[Random Maps: Ocean] map\_00=Coast Encounter 1 map\_01=Coast Encounter 2
...

This section is fairly self-evident: a list of maps used for random
encounters in the different terrain types.

==Encounter Groups==

These are the groups the encounter tables use to determine what
creatures and/or items appear with that group. A single encounter can
contain more than one group.

Example: [Encounter: ARRO\_Hunting\_Party] type\_00=pid:16777418,
Item:280(wielded), Item:(0-10)41, Script:484 ; Male Hunter w/ Sharp
Spear (Leader) type\_01=ratio:60%, pid:16777418, Item:7(wielded),
Item:(0-10)41, Script:484 ; Male Hunter w/ Spear type\_02=ratio:40%,
pid:16777419, Item:7(wielded), Item:(0-10)41, Script:484 ; Female Hunter
w/ Spear type\_03=Dead, pid:16777296, Distance:7 ; Dead Silver Gecko
position=wedge, spacing:2

There can be at most 100 type\_## entries (numbered 00 to 99). The
syntax (adapted from the description in worldmap.txt):

+------------------------------------------------------------------------------+
| Type Subinfo:                                                                |
+==============================================================================+
| Ratio:#% determines frequency of occurrence                                  |
+------------------------------------------------------------------------------+
| Dead means that the critter starts out in a dead state                       |
+------------------------------------------------------------------------------+
| pid:<#> determines what this critter's pid is                                |
+------------------------------------------------------------------------------+
| Script:<#> overrides the default script with a new one                       |
+------------------------------------------------------------------------------+
| Item:<#> gives an item of a given pid #; may be appended with "(wielded)"    |
+------------------------------------------------------------------------------+
| Item:(min-max)<#> gives a random quantity of the item, in the range given    |
+------------------------------------------------------------------------------+
| If () -- conditionals, allows <,>,<=,>=,==,& as well as tests on Player(),   |
+------------------------------------------------------------------------------+
| Enctr(Num\_Critters), Time, Global(), Rand(Chance%)                          |
+------------------------------------------------------------------------------+
| Note - Enctr and Time are never used, may not be implemented                 |
+------------------------------------------------------------------------------+
| Distance:# -- Forces the distance for a particular critter (if possible)     |
+------------------------------------------------------------------------------+

The sum of all ratios should be 100%; the game's behavior if they do not
is unknown. If no ratio is given, exactly one of that critter/item will
be generated. From text in fallout2.exe, it appears that items can be
marked "(worn)" to indicate that they are worn as armor. Either parens
() or curly braces {} can be used around wielded/worn.

Position determines what formation the group assumes when encountered,
and has one of the following values:

straight\_line - A straight line of creatures. double\_line - Two rows
of creatures. huddle - A cluster of creatures in a circle. cone - (I
believe) a triangular shape with the point away from the player. wedge -
(I believe) a triangular shape with the point facing the player.
surrounding - Surrounds the player.

This is followed by Spacing:<#>, which indicates how far the critters
are from each other. There can also be a Distance: field, which
indicates how far the critters are from the player.

The default is "position=Surrounding, Spacing:5,
Distance:Player(Perception)"

PIDs are described in the [[File\_Identifiers\_in\_Fallout \| File
Identifiers]] document. For critters, subtracting 16777216 gives the
critter's proto ID number.

==Encounter Tables== After the encounter groups come the encounter
tables. Each is a list of possible events, one of which is chosen
randomly. The worldmap.txt file contains 76 default encounter types, the
first six of which are leftover from Fallout 1 and are unused. An
example of the table format:

[Encounter Table 52] lookup\_name=SRNRRN\_M ; San Fran - NCR - Redding -
Reno Trade route - This is the name used to refer to this table in the
Tile sections. maps=Mountain Encounter 1, Mountain Encounter 2, Mountain
Encounter 4, Mountain Encounter 5, Cavern Encounter 0 - Which maps can
be used for encounters in this group. enc\_00=Chance:8%,Enc:(4-7)
KLA\_Trappers enc\_01=Chance:10%,Enc:(6-10) ARRO\_Spore\_Plants AMBUSH
Player enc\_02=Chance:9%,Enc:(3-8) ARRO\_Sm\_Scorpions AND (5-8)
ARRO\_Spore\_Plants AMBUSH Player enc\_03=Chance:10%,Enc:(5-8)
VPAT\_Patrol FIGHTING (4-6) VPAT\_Strong\_Slavers
enc\_04=Chance:3%,Enc:(3-6) KLA\_Trappers AND (4-5) KLAD\_Caravan
FIGHTING (8-12) KLA\_Bandits enc\_05=Chance:10%,Enc:(4-7)
Morton\_Brother, If(Global(386) > 0) And If(Global(386) < 6)
enc\_06=Chance:2%,Counter:1,Special,Map:Special Toxic
Encounter,Enc:Special1, If(Player(Level) > 5) And If(Global(607) < 1)

There can be up to 100 encounters (enc\_00 through enc\_99) in a table.
Each entry in the list starts with an index number and percentage
chance. The chances are probably relative, as they do not necessarily
add up to 100%. After Enc: come the encounter groups that will be found.
Each is prefixed with a range indicating how many critters from that
group should be generated. If no action is given, that group will be
neutral. If "AMBUSH Player" is specified, they will be hostile and
attack the player. If "FIGHTING" is used, the groups will attack each
other. Whether or not they will be hostile to the player is probably
determined by their scripts. Two groups connected by "AND" will be on
the same team and fight together. Finally, there may be a boolean
expression (usually based on global variables and player level) to
control whether or not this encounter can occur. "time\_of\_day" returns
the current time, rounded down to the nearest hour.

Special encounters have a slightly different format. The meaning of
"Counter:1,Special" is unknown; the number 1 is always used. Map:
specifies which special encounter map to use. Enc: is always "Special1",
a 'dummy' group with no critters in it.

The limits of the grouping mechanism are unknown. AND is only used to
join two groups, and always on the left side of FIGHTING. It is unknown
if any of the following would work: robbers AND highwaymen AND raiders
AMBUSH player geckos AND scorpions FIGHTING mole rats AND spore plants
enclave patrol FIGHTING hubologists FIGHTING master's army

From text in fallout2.exe, it appears that "days\_played" can be used
similarly to "time\_of\_day". The keyword "or" also seems to be
available, though it is not clear whether it can be applied to the
encounter groups or boolean expression.

==Tiles==

The Fallout 2 world map consists of 20 tiles. Each each of these tiles
consists of 42 squares, arranged 7 wide by 6 high (7x6). This makes for
a total of 840 individual squares on the Fallout 2 map. This section
contains terrain and encounter information for them.

[Tile Data] num\_horizontal\_tiles=4

Tiles are laid out left to right, top to bottom, so the number of
vertical tiles does not need to be specified. What happens if the total
number of tiles is not an even multiple of this value is unknown.

[Tile 0] art\_idx=339 encounter\_difficulty=0 walk\_mask\_name=wrldmp00
;
x\_offset\_yoffset=terrain,morning\_chance,afternoon\_chance,night\_chance,type
0\_0=Ocean,Fill\_W,None,None,None,Fish\_O
0\_1=Ocean,Fill\_W,None,None,None,Fish\_O
0\_2=Ocean,Fill\_W,None,None,None,Fish\_O
0\_3=Ocean,Fill\_W,None,None,None,Fish\_O
0\_4=Ocean,Fill\_W,None,None,None,Fish\_O
0\_5=Ocean,Fill\_W,None,None,None,Fish\_O
1\_0=Ocean,Fill\_W,Uncommon,Uncommon,Uncommon,Fish\_O
1\_1=Ocean,Fill\_W,Uncommon,Uncommon,Uncommon,Fish\_O
1\_2=Ocean,Fill\_W,Rare,Rare,Rare,Fish\_O
1\_3=Ocean,Fill\_W,Rare,Rare,Rare,Fish\_O
1\_4=Ocean,Fill\_W,Uncommon,Uncommon,Uncommon,Fish\_O
1\_5=Ocean,Fill\_W,Uncommon,Uncommon,Uncommon,Fish\_O
2\_0=Mountain,No\_Fill,Uncommon,Uncommon,Uncommon,Arro\_M
2\_1=Ocean,No\_Fill,Uncommon,Uncommon,Uncommon,Arro\_O
2\_2=Mountain,No\_Fill,Uncommon,Uncommon,Uncommon,Arro\_M
2\_3=Mountain,No\_Fill,Rare,Rare,Rare,Arro\_M
2\_4=Desert,No\_Fill,Uncommon,Uncommon,Uncommon,Arro\_D
2\_5=Mountain,No\_Fill,Uncommon,Uncommon,Uncommon,Arro\_M
3\_0=Mountain,No\_Fill,Uncommon,Uncommon,Uncommon,Arro\_M ...

\*art\_idx indicates which image (/art/intrface/wrldmp**.frm) is to be
displayed for this tile. *encounter\_difficulty is a modifier to the
outdoorsman skill check for the player to be able to avoid a random
encounter. Normally negative values are used, indicating a penalty.
*\ walk\_mask\_name specifies which mask file (/data/wrldmp**.msk) to
use to keep the player from walking across the ocean. If no file is
given, the entire tile is passable.

The offset of each square is measured from the upper-left corner.
Fill\_W indicates that when this square is explored, the square(s) to
the left of it become visible as well. Although similar strings exist in
fallout2.exe for all eight cardinal directions (n, s, e, w, nw, ne, sw,
se), none of the others appear to work. No\_Fill is the normal behavior,
making only the adjacent squares half-visible. The frequencies for each
of the time periods (morning, afternoon, night) are always the same, so
it isn't clear if making them different has actually been implemented.
The final type field chooses which encounter table to use for this
square.

Originally based on the document in [[TeamX]]'s
[http://www.teamx.ru/eng/files/docs/index.shtml file
section][[Category:Fallout and Fallout 2 file formats]]
