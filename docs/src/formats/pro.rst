===
PRO
===

'''Note:''' All file paths (folders) are relative to either master.dat
(or critter.dat) if packed, or to Fallout's/Fallout 2's DATA folder if
unpacked.

This document describes the format of Fallout and Fallout 2 .PRO
(Prototype) files, which are located in PROTO/.

Each item, critter, wall, tile, and piece of scenery in the game has its
own corresponding PRO file. The PRO file name is usually generated using
the object's ''[[#Common header\|ObjectID]]'', pre-padded by zeros so
that it's 8 digits. For example, ObjectID of 32 would become
00000032.PRO.

The .PRO files in saved games (in DATA/SAVEGAME/SLOT##/PROTO/) have,
like the [[SAV File Format\|.SAV]] files, been gzipped. Adding the
extension '.gz' will let most archive utilities open them. Once
extracted, they have the format described here.

==Format description==

All controlling values in an PRO file are stored in
[http://en.wikipedia.org/wiki/Endianness big-endian] format. The first
56 bytes are common for all PROs. Those bytes are followed by values
specific for each type (and subtype, for Items and Scenery).

===PRO types=== {\| class="wikitable" border="1" ! Type ID !! Object !!
Location in PROTO/ !! File size, in bytes \|- \| 0x00 \|\| Items \|\|
ITEMS/ \|\| 129, 125, 122, 81, 69, 65, or 61, depending on type \|- \|
0x01 \|\| Critters \|\| CRITTERS/ \|\| 416 in Fallout 2, 412 in Fallout
1 \|- \| 0x02 \|\| Scenery \|\| SCENERY \|\| 49 or 45, depending on type
\|- \| 0x03 \|\| Walls \|\| WALLS/ \|\| 36 \|- \| 0x04 \|\| Tiles \|\|
TILES/ \|\| 28 \|- \| 0x05 \|\| Misc \|\| MISC/ \|\| 28 \|}

==Common header==

{\| class="wikitable" border="1" style="width: 640px" !Offset !! Size !!
Description \|- \| 0x0000 \|\| 4 \|\| '''''ObjectType & ObjectID'''''
Format:It is currently unknown how the second byte is (or isn't) used.
It is possible, and even likely, that instead of reading the value as
two separate numbers (ignoring the second byte), all 4 bytes are
actually read as a single number, of which the first byte is then split
away. That would leave 3 bytes for ObjectID (instead of 2), allowing
ObjectIDs up to 2^24 (16777215 or 8388607) instead of just 2^16 (65535
or 32767).

.. raw:: html

   <pre>0xAABBCCCC
     | | |
     | | |
     | | +- ObjectID
     | +--- Possible part of object ID (see footnote)
     +----- ObjectType</pre>

''ObjectType'':

.. raw:: html

   <pre>00: items
   01: critters
   02: scenery
   03: walls
   04: tiles
   05: misc</pre>

\|- \| 0x0004 \|\| 4 \|\| '''''TextID''''' Normally ''ObjectID''
multiplied by 100. This is the line number of the object name &
description (''TextID''+1) in the appropriate message file (directory
text/english/game/, file pro\_item.msg, pro\_crit.msg, pro\_scen.msg,
pro\_wall.msg, pro\_tile.msg, or pro\_misc.msg). \|- \| 0x0008 \|\| 4
\|\| '''''FrmType & FrmID''''' Format:

.. raw:: html

   <pre>0xAABBCCCC
     | | |
     | | |
     | | +- FrmID
     | +--- Possible part of FrmID (see footnote)
     +----- FrmType</pre>

''FrmType'':

.. raw:: html

   <pre>00: items
   01: critters
   02: scenery
   03: walls
   04: tiles
   05: backgrnd
   06: intrface
   07: inven</pre>

It is used to determine in what directory to search for the
corresponding FRM list and file (critter FRM list and files are in
critter.dat).

''FrmID'': line number in the relevant .lst file (e.g.
art/items/items.lst for Items). \|- \| 0x000C \|\| 4 \|\| '''''Light
radius''''' Values: 0..8 (hexes) \|- \| 0x0010 \|\| 4 \|\| '''''Light
intensity''''' Values: 0..65536 (0x0000FFFF). According to Fallout 2
mapper, this value is interpreted as 0..100%. \|- \| 0x0014 \|\| 4 \|\|
'''''Flags''''' Even though the data is present in the file, the flags
are not used for Tiles — the value is always -1 (0xFFFFFFFF). TransNone,
TransWall, TransGlass, TransSteam and TransEnergy flags are mutually
exclusive, but TransNone is not default value, default is empty. If any
of Trans\* flags are set, than the object is not affected by
transparency egg There is information that there is another flag,
"0x00000020 - Lights (gives off light, see fields Light radius and Light
intensity)". Considered false until proven otherwise, since doesn't show
up anywhere in Fallout 2 mapper (instead, the ''Light Radius'', ''Light
Intensity'' and/or ''Wall Light Type Flags'' seem to be used to
determine wether the proto gives off light at all)

.. raw:: html

   <pre>0x00000008 - Flat (rendered first, just after tiles)
   0x00000010 - NoBlock (doesn't block the tile)
   0x00000800 - MultiHex 
   0x00001000 - No Highlight (doesn't highlight the border; used for containers) 
   0x00004000 - TransRed 
   0x00008000 - TransNone (opaque) 
   0x00010000 - TransWall 
   0x00020000 - TransGlass
   0x00040000 - TransSteam 
   0x00080000 - TransEnergy
   0x10000000 - WallTransEnd (changes transparency egg logic. Set for walls with 1144 - 1155 pid)
   0x20000000 - LightThru 
   0x80000000 - ShootThru</pre>

\|}

==Items==

Item types: {\| class="wikitable" border="1" ! Type ID !! Object !! File
size, in bytes \|- \| 0 \|\| Armor \|\| 129 \|- \| 1 \|\| Container \|\|
65 \|- \| 2 \|\| Drug \|\| 125 \|- \| 3 \|\| Weapon \|\| 122 \|- \| 4
\|\| Ammo \|\| 81 \|- \| 5 \|\| Misc Item \|\| 69 \|- \| 6 \|\| Key \|\|
61 \|}

===Common fields===

{\|class="wikitable" border="1" !Offset !Size !Description \|- \|0x0018
\|3 \|Flags Ext Item Flags: 0x080000 - Hidden Item

Action Flags: 0x000008 - Use (can be used) 0x000010 - Use On Smth (can
be used on anything) 0x000080 - PickUp

| Weapon Flags: 0x000001 - Big Gun 0x000002 - 2Hnd (weapon is
  two-handed) \|- \|0x001B \|1 \|Attack Modes: This value is only used
  for weapons. This 1-byte value can be divided into two 4-bit
  components, each of which represents an attack mode: 0 - None 1 -
  Punch 2 - Kick 3 - Swing 4 - Thrust
| 5 - Throw 6 - Fire Single 7 - Fire Burst 8 - Flame

So, each weapon has 2 main attack modes: the lower 4 bytes determine
attack mode 1, the higher ones determine attack mode 2. \|- \|0x001C \|4
\|Script ID (script) Format: 0x0Y00XXXX Y-type specifier (1-spatial,
2-items, 3 - scenery, 4-critters) XXXX-line in scripts.lst If the value
is 0xFFFFFFFF, there is no script \|- \|0x0020 \|4 \|ObjSubType (see
proto.msg, starting with the line 150) Values: 0 - Armor 1 - Container 2
- Drug 3 - Weapon 4 - Ammo 5 - Misc 6 - Key \|- \|0x0024 \|4
\|'''''[[#Repeated values\|MaterialID]]''''' \|- \|0x0028 \|4 \|Size
(volume in containers) \|- \|0x002C \|4 \|Weight \|- \|0x0030 \|4 \|Cost
\|- \|0x0034 \|4 \|Inv FID (FRM ID for the item in inventory) \|-
\|0x0038 \|1 \|Sound ID Sound related to the item: when picked up from
the ground, opening a container, etc. Values: 0x21 0x23 0x24 0x30 ...
0x5A \|}

===Armor===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0039 \|\| 4 \|\| AC \|- ! colspan="3" \| '''Damage Resistance''' \|-
\| 0x003D \|\| 4 \|\| DR Normal \|- \| 0x0041 \|\| 4 \|\| DR Laser \|-
\| 0x0045 \|\| 4 \|\| DR Fire \|- \| 0x0049 \|\| 4 \|\| DR Plasma \|- \|
0x004D \|\| 4 \|\| DR Electrical \|- \| 0x0051 \|\| 4 \|\| DR EMP \|- \|
0x0055 \|\| 4 \|\| DR Explosion \|- ! colspan="3" \| '''Damage
Threshold''' \|- \| 0x0059 \|\| 4 \|\| DT Normal \|- \| 0x005D \|\| 4
\|\| DT Laser \|- \| 0x0061 \|\| 4 \|\| DT Fire \|- \| 0x0065 \|\| 4
\|\| DT Plasma \|- \| 0x0069 \|\| 4 \|\| DT Electrical \|- \| 0x006D
\|\| 4 \|\| DT EMP \|- \| 0x0071 \|\| 4 \|\| DT Explosion \|- !
colspan="3" \| '''Other''' \|- \| 0x0075 \|\| 4 \|\| Perk (see perk.msg,
starting with the line 101) -1 means no perk. Note: the object may have
any perk, but not all will work. \|- \| 0x0079 \|\| 4 \|\| Male FID \|-
\| 0x007D \|\| 4 \|\| Female FID \|}

===Containers=== {\| class="wikitable" border="1" ! Offset !! Size !!
Description \|- \| 0x0039 \|\| 4 \|\| Max Size (how much it can contain)
\|- \| 0x003D \|\| 4 \|\| Open Flags: 0x00000001 - Cannot Pick Up
'''(implies Magic Hands Grnd!)'''

0x00000008 - Magic Hands Grnd (reach down to open/close) \|}

===Drugs===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0039 \|\| 4 \|\| Stat0 Determines what character's characteristic is
to be changed '''immediately''' after taking the drug. Values: (see
stat.msg, starting with the line 100) 0 - Strength 1 - Perception 2 -
Endurance

etc. -1 - no effect -2 - Amount1 (below) will contain a random number
between Amount0, and Amount1 (inclusive) \|- \| 0x003D \|\| 4 \|\| Stat1
See Stat0 \|- \| 0x0041 \|\| 4 \|\| Stat2 See Stat0 \|- ! colspan="3" \|
'''Instant effect''' \|- \| 0x0045 \|\| 4 \|\| Amount0 Modifier for
Stat0 \|- \| 0x0049 \|\| 4 \|\| Amount1 Modifier for Stat1 \|- \| 0x004D
\|\| 4 \|\| Amount2 Modifier for Stat2 \|- ! colspan="3" \| '''First
delayed effect''' \|- \| 0x0051 \|\| 4 \|\| Duration1 The time delay for
the first effect (in game minutes). \|- \| 0x0055 \|\| 4 \|\| Amount0
Modifier for Stat0 \|- \| 0x0059 \|\| 4 \|\| Amount1 Modifier for Stat1
\|- \| 0x005D \|\| 4 \|\| Amount2 Modifier for Stat2 \|- ! colspan="3"
\| '''Second delayed effect''' \|- \| 0x0061 \|\| 4 \|\| Duration2 The
time delay for the second effect. This should be more Duration1. \|- \|
0x0065 \|\| 4 \|\| Amount0 Modifier for Stat0 \|- \| 0x0069 \|\| 4 \|\|
Amount1 Modifier for Stat1 \|- \| 0x0065 \|\| 4 \|\| Amount2 Modifier
for Stat2 \|- ! colspan="3" \| '''Other''' \|- \| 0x0071 \|\| 4 \|\|
Addiction Rate The probability of getting addicted, in percent. \|- \|
0x0075 \|\| 4 \|\| Addiction Effect Number of the perk to be given when
the player is addicted. Values: (see perk.msg, starting with the line
101) -1 for no perk Note: any perk can be used, but not all will work.
\|- \| 0x0079 \|\| 4 \|\| Addiction Onset Delay before the addiction
effect is applied. \|}

===Weapons===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0039 \|\| 4 \|\| Anim Code Which frameset is used for characters
wielding that weapon. Values: 0x00 - None (A) 0x01 - Knife (D) 0x02 -
Club (E) 0x03 - Sledgehammer (F) 0x04 - Spear (G) 0x05 - Pistol (H) 0x06
- SMG (I) 0x07 - Rifle (J) 0x08 - Big Gun (K) 0x09 - Minigun (L) 0x0A -
Rocket Launcher (M) \|- \| 0x003D \|\| 4 \|\| Min Dmg \|- \| 0x0041 \|\|
4 \|\| Max Dmg \|- \| 0x0045 \|\| 4 \|\| Dmg Type (see proto.msg,
starting with the line 250) 0 - Normal 1 - Laser 2 - Fire 3 - Plasma 4 -
Electrical 5 - EMP 6 - Explosive \|- \| 0x0049 \|\| 4 \|\| Max Range 1
The maximum distance for Primary Attack \|- \| 0x004D \|\| 4 \|\| Max
Range 2 The maximum distance for Secondary Attack \|- \| 0x0051 \|\| 4
\|\| Proj PID Format: 0x0500YYYY

where YYYY is an index in misc.lst \|- \| 0x0055 \|\| 4 \|\| Min ST The
minimum strength required to use the weapon \|- \| 0x0059 \|\| 4 \|\| AP
Cost 1 The number of AP for Primary Attack \|- \| 0x005D \|\| 4 \|\| MP
Cost 2 The number of AP for Secondary Attack \|- \| 0x0061 \|\| 4 \|\|
Crit Fail Number of the list of critical failures possible for this
weapon. \|- \| 0x0065 \|\| 4 \|\| Perk Number of the perk associated
with this weapon.

Values: see perk.msg, starting with the line 101; -1 for no perk

Note: the weapons can be have any perk, but not all will work. \|- \|
0x0069 \|\| 4 \|\| Rounds The number of rounds fired in a burst attack.
\|- \| 0x006D \|\| 4 \|\| Caliber Values: see proto.msg, starting with
the line 300 \|- \| 0x0071 \|\| 4 \|\| Ammo PID Index in items.lst \|-
\| 0x0075 \|\| 4 \|\| Max Ammo (size of the magazine) \|- \| 0x0079 \|\|
1 \|\| Sound ID Sound effects for the weapon: shooting, reloading, etc.
Line number in master.dat/sound/sfx/sndlist.lst \|}

===Ammo===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0039 \|\| 4 \|\| Caliber (ammo type) Values: see proto.msg, starting
with the line 300 \|- \| 0x003D \|\| 4 \|\| Quantity The number of
rounds in a magazine \|- \| 0x0041 \|\| 4 \|\| AC modifier \|- \| 0x0045
\|\| 4 \|\| DR modifier \|- \| 0x0049 \|\| 4 \|\| Dam Mult The
multiplier part of the ammo damage mod. \|- \| 0x004D \|\| 4 \|\| Dam
Div The divisor part of the ammo damage mod. \|}

===Misc===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0039 \|\| 4 \|\| Power PID Index in items.lst \|- \| 0x003D \|\| 4
\|\| Power Type Values: see proto.msg, starting with the line 300 \|- \|
0x0041 \|\| 4 \|\| Charges The maximum number of charges \|}

===Keys===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0039 \|\| 4 \|\| KeyCode always 0xFFFFFFFF \|}

==Critters==

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0018 \|\| 4 \|\| Flags Ext Action Flags: 0x00002000 0x00004000 \|- \|
0x001C \|\| 4 \|\| Script ID Format: 0x0Y00XXXX

Y-type specifier (1-spatial, 2-items, 3 - scenery, 4-critters)

XXXX-room in scripts.lst

If the value is 0xFFFFFFFF, there is no script \|- \| 0x0020 \|\| 4 \|\|
Head FID (for talking heads) (see ) \|- \| 0x0024 \|\| 4 \|\| AI Packet
Format: 0x00000XXX XXX-see ai.txt, parameter packet\_num Note: Some
numbers can be drawn from packages and aibdymsg.txt aigenmsg.txt.
Similarly ai.txt. \|- \| 0x0028 \|\| 4 \|\| Team Num \|- \| 0x002C \|\|
4 \|\| Flags Critter Flags: 0x00000002 - Barter (can trade with)
0x00000020 - Steal (cannot steal from) 0x00000040 - Drop (doesn't drop
items) 0x00000080 - Limbs (can not lose limbs) 0x00000100 - Ages (dead
body does not disappear) 0x00000200 - Heal (damage is not cured with
time) 0x00000400 - Invulnerable (cannot be hurt) 0x00000800 - Flatten
(leaves no dead body) 0x00001000 - Special (there is a special type of
death) 0x00002000 - Range (melee attack is possible at a distance)
0x00004000 - Knock (cannot be knocked down) \|- ! colspan="3" \| '''Base
values of primary stats''' \|- \| 0x0030 \|\| 4 \|\| Strength (1-10) \|-
\| 0x0034 \|\| 4 \|\| Perception (1-10) \|- \| 0x0038 \|\| 4 \|\|
Endurance (1-10) \|- \| 0x003C \|\| 4 \|\| Charisma (1-10) \|- \| 0x0040
\|\| 4 \|\| Intelligence (1-10) \|- \| 0x0044 \|\| 4 \|\| Agility (1-10)
\|- \| 0x0048 \|\| 4 \|\| Luck (1-10) \|- \| 0x004C \|\| 4 \|\| HP \|-
\| 0x0050 \|\| 4 \|\| AP \|- \| 0x0054 \|\| 4 \|\| AC \|- \| 0x0058 \|\|
4 \|\| Unarmed damage ('''UNUSED,''' use Melee damage instead) \|- \|
0x005C \|\| 4 \|\| Melee damage \|- \| 0x0060 \|\| 4 \|\| Carry weight
(0-999) \|- \| 0x0064 \|\| 4 \|\| Sequence \|- \| 0x0068 \|\| 4 \|\|
Healing rate \|- \| 0x006C \|\| 4 \|\| Critical chance \|- \| 0x0070
\|\| 4 \|\| Better criticals \|- ! colspan="3" \| '''Base values of DR
and DT''' \|- \| 0x0074 \|\| 4 \|\| DT Normal \|- \| 0x0078 \|\| 4 \|\|
DT Laser \|- \| 0x007C \|\| 4 \|\| DT Fire \|- \| 0x0080 \|\| 4 \|\| DT
Plasma \|- \| 0x0084 \|\| 4 \|\| DT Electrical \|- \| 0x0088 \|\| 4 \|\|
DT EMP \|- \| 0x008C \|\| 4 \|\| DT Explosive \|- \| 0x0090 \|\| 4 \|\|
DR Normal \|- \| 0x0094 \|\| 4 \|\| DR Laser \|- \| 0x0098 \|\| 4 \|\|
DR Fire \|- \| 0x009C \|\| 4 \|\| DR Plasma \|- \| 0x00A0 \|\| 4 \|\| DR
Electrical \|- \| 0x00A4 \|\| 4 \|\| DR EMP \|- \| 0x00A8 \|\| 4 \|\| DR
Explosive \|- \| 0x00AC \|\| 4 \|\| DR Radiation \|- \| 0x00B0 \|\| 4
\|\| DR Poison \|- ! colspan="3" \| '''Basic age and sex''' \|- \|
0x00B4 \|\| 4 \|\| Age (1-99) \|- \| 0x00B8 \|\| 4 \|\| Gender (0-male,
1-female) \|- ! colspan="3" \| '''Bonuses to primary stats''' \|- \|
0x00BC \|\| 4 \|\| Strength (1-10) \|- \| 0x00C0 \|\| 4 \|\| Perception
(1-10) \|- \| 0x00C4 \|\| 4 \|\| Endurance (1-10) \|- \| 0x00C8 \|\| 4
\|\| Charisma (1-10) \|- \| 0x00CC \|\| 4 \|\| Intelligence (1-10) \|-
\| 0x00D0 \|\| 4 \|\| Agility (1-10) \|- \| 0x00D4 \|\| 4 \|\| Luck
(1-10) \|- \| 0x00D8 \|\| 4 \|\| HP \|- \| 0x00DC \|\| 4 \|\| AP \|- \|
0x00E0 \|\| 4 \|\| AC \|- \| 0x00E4 \|\| 4 \|\| Unarmed damage
('''UNUSED,''' use Melee damage instead) \|- \| 0x00E8 \|\| 4 \|\| Melee
damage \|- \| 0x00EC \|\| 4 \|\| Carry weight (0-999) \|- \| 0x00F0 \|\|
4 \|\| Sequence \|- \| 0x00F4 \|\| 4 \|\| Healing rate \|- \| 0x00F8
\|\| 4 \|\| Critical chance \|- \| 0x00FC \|\| 4 \|\| Better criticals
\|- ! colspan="3" \| '''Bonuses to DT and DR''' \|- \| 0x0100 \|\| 4
\|\| DT Normal \|- \| 0x0104 \|\| 4 \|\| DT Laser \|- \| 0x0108 \|\| 4
\|\| DT Fire \|- \| 0x010C \|\| 4 \|\| DT Plasma \|- \| 0x0110 \|\| 4
\|\| DT Electrical \|- \| 0x0114 \|\| 4 \|\| DT EMP \|- \| 0x0118 \|\| 4
\|\| DT Explosive \|- \| 0x011C \|\| 4 \|\| DR Normal \|- \| 0x0120 \|\|
4 \|\| DR Laser \|- \| 0x0124 \|\| 4 \|\| DR Fire \|- \| 0x0128 \|\| 4
\|\| DR Plasma \|- \| 0x012C \|\| 4 \|\| DR Electrical \|- \| 0x0130
\|\| 4 \|\| DR EMP \|- \| 0x0134 \|\| 4 \|\| DR Explosive \|- \| 0x0138
\|\| 4 \|\| DR Radiation \|- \| 0x013C \|\| 4 \|\| DR Poison \|- !
colspan="3" \| '''Bonuses to age and sex''' \|- \| 0x0140 \|\| 4 \|\|
Age (1-99) \|- \| 0x0144 \|\| 4 \|\| Gender (0-male, 1-female) \|- !
colspan="3" \| '''Skills''' \|- \| 0x0148 \|\| 4 \|\| Small guns (0-300)
\|- \| 0x014C \|\| 4 \|\| Big guns (0-300) \|- \| 0x0150 \|\| 4 \|\|
Energy weapons (0-300) \|- \| 0x0154 \|\| 4 \|\| Unarmed (0-300) \|- \|
0x0158 \|\| 4 \|\| Melee (0-300) \|- \| 0x015C \|\| 4 \|\| Throwing
(0-300) \|- \| 0x0160 \|\| 4 \|\| First aid (0-300) \|- \| 0x0164 \|\| 4
\|\| Doctor (0-300) \|- \| 0x0168 \|\| 4 \|\| Sneak (0-300) \|- \|
0x016C \|\| 4 \|\| Lockpick (0-300) \|- \| 0x0170 \|\| 4 \|\| Steal
(0-300) \|- \| 0x0174 \|\| 4 \|\| Traps (0-300) \|- \| 0x0178 \|\| 4
\|\| Science (0-300) \|- \| 0x017C \|\| 4 \|\| Repair (0-300) \|- \|
0x0180 \|\| 4 \|\| Speech (0-300) \|- \| 0x0184 \|\| 4 \|\| Barter
(0-300) \|- \| 0x0188 \|\| 4 \|\| Gambling (0-300) \|- \| 0x018C \|\| 4
\|\| Outdoorsman (0-300) \|- ! colspan="3" \| '''Other''' \|- \| 0x0190
\|\| 4 \|\| Body Type (see proto.msg, starting with the line 400) 0x0 -
biped 0x1 - quadruped 0x2 - robotic \|- \| 0x0194 \|\| 4 \|\| Exp Val
Amount of experience gained for killing this critter \|- \| 0x0198 \|\|
4 \|\| Kill Type (see proto.msg, starting with the line 1450) 0x00 - Men
0x01 - Women 0x02 - Children 0x03 - Super Mutants 0x04 - Ghouls 0x05 -
Brahmin 0x06 - Radscorpions 0x07 - Rats 0x08 - Floaters 0x09 - Centaurs
0x0A - Robots 0x0B - Dogs 0x0C - Manti 0x0D - DeathClaws 0x0E - Plants
(only applies to Fallout 2) 0x0F - Geckos (only applies to Fallout 2)
0x10 - Aliens (only applies to Fallout 2) 0x11 - Giant Ants (only
applies to Fallout 2) 0x12 - Big Bad Boss (only applies to Fallout 2)
\|- \| 0x019C \|\| 4 \|\| Damage Type (see proto.msg, starting with the
line 250) 0x0 - Normal 0x1 - Laser 0x2 - Fire 0x3 - Plasma 0x4 -
Electrical 0x5 - EMP 0x6 - Explode \|}

==Scenery==

Types of scenery: {\| class="wikitable" border="1" ! Scenery ID !! Type
!! .pro file size, in bytes \|- \| 0 \|\| Door \|\| 49 \|- \| 1 \|\|
Stairs \|\| 49 \|- \| 2 \|\| Elevator \|\| 49 \|- \| 3 \|\| Ladder
Bottom \|\| 45 \|- \| 4 \|\| Ladder Top \|\| 45 \|- \| 5 \|\| Generic
Scenery \|\| 45 \|}

===Common Fields===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0018 \|\| 2 \|\| '''''[[#Repeated values\|Wall Light Type Flags]]'''''
\|- \| 0x001A \|\| 2 \|\| '''''[[#Repeated values\|Action Flags]]'''''
\|- \| 0x001C \|\| 4 \|\| '''''[[#Repeated values\|ScriptType &
ScriptID]]''''' \|- \| 0x0020 \|\| 4 \|\| '''''ScenerySubType''''' See
text/game/proto.msg, starting with the line 200. Values:

.. raw:: html

   <pre>0: Door 
   1: Stairs 
   2: Elevator 
   3: Ladder Bottom 
   4: Ladder Top 
   5: Generic Scenery</pre>

\|- \| 0x0024 \|\| 4 \|\| '''''[[#Repeated values\|MaterialID]]''''' \|-
\| 0x0028 \|\| 1 \|\| '''''SoundID''''' Values (according to the list in
Fallout 2 mapper):

.. raw:: html

   <pre>
    0x21 
    0x23 
    0x24 
    0x30..0x5A</pre>

\|}

===Door===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0029 \|\| 4 \|\| '''''WalkThru Flag''''' Values:

.. raw:: html

   <pre>0x0000000F: yes</pre>

\|- \| 0x002D \|\| 4 \|\| ''(unknown)'' 0xCCCCCCCC (mostly) or
0xFFFFFFFF (sometimes) \|}

=== StairsThese values are not normally saved in the .pro file (space
filled with 0xFF), as they only make sense when actually put on a map.
===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0029 \|\| 4 \|\| '''''[[#Repeated values\|DestTile & DestElev]]'''''
\|- \| 0x002D \|\| 4 \|\| DestMap Value is a map number in
data:raw-latex:`\maps`.txt (hard-coded list in Fallout?), -1 goes to the
worldmap. \|}

=== Elevator ===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0029 \|\| 4 \|\| ElevType Values from 0x00 to 0x17 \|- \| 0x002D \|\|
4 \|\| ElevLevel Current level of the elevator (not to be confused with
the level of map!). This parameter specifies which floor the arrow
initially points at. \|}

=== Ladder Bottom ===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0029 \|\| 4 \|\| '''''[[#Repeated values\|DestTile & DestElev]]'''''
\|}

=== Ladder Top ===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0029 \|\| 4 \|\| '''''[[#Repeated values\|DestTile & DestElev]]'''''
\|}

===Generic Scenery===

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0029 \|\| 4 \|\| ''(Unkown)'' 0xCCCCCCCC (mostly) or 0xFFFFFFFF
(sometimes) \|}

==Walls==

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0018 \|\| 2 \|\| '''''[[#Repeated values\|Wall Light Type Flags]]'''''
\|- \| 0x001A \|\| 2 \|\| '''''[[#Repeated values\|Action Flags]]'''''
\|- \| 0x001C \|\| 4 \|\| '''''[[#Repeated values\|ScriptType &
ScriptID]]''''' \|- \| 0x0020 \|\| 4 \|\| '''''[[#Repeated
values\|MaterialID]]''''' \|}

==Tiles==

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0018 \|\| 4 \|\| '''''[[#Repeated valeus\|MaterialID]]''''' \|}

==Misc==

{\| class="wikitable" border="1" ! Offset !! Size !! Description \|- \|
0x0018 \|\| 4 \|\| Unknown, presumably unused \|}

==Repeated values== These are values that appear in more than one PRO
type/subtype. {\| class="wikitable" border="1" !Length!!Description \|-
\| 2 \|\| '''''Wall Light Type Flags'''''

.. raw:: html

   <pre>0x0000 - North / South 
   0x0800 - East / West 
   0x1000 - North Corner 
   0x2000 - South Corner 
   0x4000 - East Corner 
   0x8000 - West Corner</pre>

\|- \| 2 \|\| '''''Action Flags'''''

.. raw:: html

   <pre>0x0001 - Kneel down when using
   0x0008 - Use (can be used) 
   0x0010 - Use On Smth (can be used on anything) 
   0x0020 - Look
   0x0040 - Talk 
   0x0080 - PickUp</pre>

\|- \| 4 \|\| '''''DestTile & DestElev''''' Format:

.. raw:: html

   <pre>0xABCCDDDD
     | | |
     | | |
     | | +- DestTile
     | +--- Possible part of DestElev (see footnote)
     +----- DestElev</pre>

''DestElev'': A can be anythingSetting DestElev in Fallout 2 mapper
autmatically sets A to 0xC., B values:

.. raw:: html

   <pre>0x0: zero 
   0x2: first 
   0x4: second</pre>

''DestTile'' values: 0 to 40000. \|- \| 4 \|\| '''''ScriptType &
ScriptID''''' -1 (0xFFFFFFFF) for none.

Format:

.. raw:: html

   <pre>0xAABBCCCC
     | | |
     | | |
     | | +- ScriptID
     | +--- Possible part of ScriptID (see footnote)
     +----- ScriptType
   </pre>

''ScriptType'':Older information suggests "1-spatial, 2-items,
3-scenery, 4-critters", but since 3 is actualy "Holodisk", at least in
items, it is not trusted.

.. raw:: html

   <pre>03: Holodisk</pre>

''ScriptID'': Number of the line in scripts/scripts.lst that has the
filename of the required script. \|- \| 4 \|\| '''''MaterialID''''' See
proto.msg, starting with the line 100. Values: 0: Glass 1: Metal 2:
Plastic 3: Wood 4: Dirt 5: Stone 6: Cement 7: Leather \|}

==Footnotes & references==

==Credits for the original (pre-wiki) article==

Originally written by Borg Locutus

Re-written and extended by Serge, R&D Group Leader, TeamX

Additional info by Tarantul.

Some info borrowed from FIME item editor manual by Garry

Revised by DIMS.

Translated by Dr. W95

Most content comes from the [http://teamx.ru/node/333 TeamX
document][[Category:Fallout and Fallout 2 file formats]]
