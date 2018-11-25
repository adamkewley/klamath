===
GCD
===

GCD files contain starting character profiles. For more details, see
"[[Objects\_in\_Fallout\_2\|Resource Handling]]", section "[[Characters
files format\|Premade characters]]".

In addition to the 'theme' characters combat.gcd, stealth.gcd and
diplomat.gcd, there are thee other files: demo.gcd, blank.gcd and
player.gcd.

Presumably, demo.gcd was used in the demo, and blank.gcd and player.gcd
were intended for creating our own characters (recall at the beginning
of the game choosing the button "create character"). However, none of
these files are used in the game.

'''GCD File Format'''

All offsets are listed in this table: {\| border="1" \|- ! Offset !!
Meaning !! Normal Range \|- ! align="center" colspan="3" \| Game flags
\|- \|0x0000 \|This part is not fully understood, but includes
indicating some of the status tags shown in the lower-left part of the
screen. It's known that 0x00000001 - "sneak", 0x00000008 - "level",
0x00000010 - "addict". The tags for "poisoned" and "radiated" are not
here. \| - \|- ! align="center" colspan="3" \| '''Primary stats''' \|-
\|0x0004 \|Strength (ST) \|0 to 10 \|- \|0x0008 \|Perception (PE) \|0 to
10 \|- \|0x000C \|Endurance (EN) \|0 to 10 \|- \|0x0010 \|Charisma (CH)
\|0 to 10 \|- \|0x0014 \|Intelligence(IN) \|0 to 10 \|- \|0x0018
\|Agility (AG) \|0 to 10 \|- \|0x001C \|Luck (LK) \|0 to 10 \|- !
align"center" colspan="3" \| '''Secondary stats''' \|- \|0x0020 \|Hit
Points * \|0 to 999 \|- \|0x0024 \|Action Points * \|0 to 999 \|-
\|0x0028 \|Armor Class * \|0 to 999 \|- \|0x002C \|Unarmed Damage (NOTE:
this is not used by the game! Melee Damage stat is used instead.) \| -
\|- \|0x0030 \|Melee Damage * \|0 to 999 \|- \|0x0034 \|Carry Weight *
\|0 to 999 \|- \|0x0038 \|Sequence * \|0 to 99 \|- \|0x003C \|Healing
Rate * \|0 to 99 \|- \|0x0040 \|Critical Chance * \|0 to 100 \|-
\|0x0044 \|Critical hit modifier * \|0 to 100 \|- \|0x0048 \|Damage
Threshold - normal \|0 to 999 \|- \|0x004C \|Damage Threshold - laser
\|0 to 999 \|- \|0x0050 \|Damage Threshold - fire \|0 to 999 \|-
\|0x0054 \|Damage Threshold - plasma \|0 to 999 \|- \|0x0058 \|Damage
Threshold - electrical \|0 to 999 \|- \|0x005C \|Damage Threshold - EMP
\|0 to 999 \|- \|0x0060 \|Damage Threshold - explosive \|0 to 999 \|-
\|0x0064 \|Damage Resistance - normal \|0 to 100 \|- \|0x0068 \|Damage
Resistance - laser \|0 to 100 \|- \|0x006C \|Damage Resistance - fire
\|0 to 100 \|- \|0x0070 \|Damage Resistance - plasma \|0 to 100 \|-
\|0x0074 \|Damage Resistance - electrical \|0 to 100 \|- \|0x0078
\|Damage Resistance - EMP \|0 to 100 \|- \|0x007C \|Damage Resistance -
explosive \|0 to 100 \|- \|0x0080 \|Radiation Resistance * \|0 to 100
\|- \|0x0084 \|Poison Resistance * \|0 to 100 \|- \|0x0088 \|Age \|0 to
35 \|- \|0x008C \|Gender \|1 - female, 0 - male. \|- ! align"center"
colspan="3" \| '''Bonuses to primary stats''' \|- \|0x0090 \|Bonus to
strength \|0 to 10 \|- \|0x0094 \|Bonus to perception \|0 to 10 \|-
\|0x0098 \|Bonus to endurance \|0 to 10 \|- \|0x009C \|Bonus to charisma
\|0 to 10 \|- \|0x00A0 \|Bonus to intelligence \|0 to 10 \|- \|0x00A4
\|Bonus to agility \|0 to 10 \|- \|0x00A8 \|Bonus to luck \|0 to 10 \|-
! align"center" colspan="3" \| '''Bonuses to secondary stats''' \|-
\|0x00AC \|Bonus HP \|0 to 999 \|- \|0x00B0 \|Bonus AP \|0 to 999 \|-
\|0x00B4 \|Bonus AC \|0 to 999 \|- \|0x00B8 \|Bonus to unarmed damage
(NOTE: this is not used by the game! Melee Damage stat is used instead.)
\| - \|- \|0x00BC \|Bonus to melee damage \|0 to 999 \|- \|0x00C0
\|Bonus to carry weight \|0 to 999 \|- \|0x00C4 \|Bonus to sequence \|0
to 99 \|- \|0x00C8 \|Bonus to healing rate \|0 to 99 \|- \|0x00CC
\|Bonus to critical chance \|0 to 100 \|- \|0x00D0 \|Bonus to critical
hit modifier \|0 to 100 \|- \|0x00D4 \|Bonus to DT (normal) \|0 to 999
\|- \|0x00D8 \|Bonus to DT (laser) \|0 to 999 \|- \|0x00DC \|Bonus to DT
(fire) \|0 to 999 \|- \|0x00E0 \|Bonus to DT (plasma) \|0 to 999 \|-
\|0x00E4 \|Bonus to DT (electrical) \|0 to 999 \|- \|0x00E8 \|Bonus to
DT (EMP) \|0 to 999 \|- \|0x00EC \|Bonus to DT (explosive) \|0 to 999
\|- \|0x00F0 \|Bonus to DR (normal) \|0 to 100 \|- \|0x00F4 \|Bonus to
DR (laser) \|0 to 100 \|- \|0x00F8 \|Bonus to DR (fire) \|0 to 100 \|-
\|0x00FC \|Bonus to DR (plasma) \|0 to 100 \|- \|0x0100 \|Bonus to DR
(electrical) \|0 to 100 \|- \|0x0104 \|Bonus to DR (EMP) \|0 to 100 \|-
\|0x0108 \|Bonus to DR (explosive) \|0 to 100 \|- \|0x010C \|Bonus to
radiation resistance \|0 to 100 \|- \|0x0110 \|Bonus to poison
resistance \|0 to 100 \|- \|0x0114 \|Bonus to age \|0 to 35 \|- \|0x0118
\|Bonus to gender (sex-change surgery is still available in the
post-nuclear world, it seems) \|Positive values - female, negative or
zero - male \|- ! align"center" colspan="3" \| '''Skills''' \|- \|0x011C
\|Small Guns \|0 to 300-(5+4*\ AG) \|- \|0x0120 \|Big Guns \|0 to
300-(2\ *AG) \|- \|0x0124 \|Energy Weapons \|0 to 300-(2*\ AG) \|-
\|0x0128 \|Unarmed \|0 to 300-(30+2\ *(ST+AG)) \|- \|0x012C \|Melee
Weapons \|0 to 300-(20+2*\ (ST+AG)) \|- \|0x0130 \|Throwing \|0 to
300-(4\ *AG) \|- \|0x0134 \|First Aid \|0 to 300-(2*\ (PE+IN)) \|-
\|0x0138 \|Doctor \|0 to 300-(5+PE+IN) \|- \|0x013C \|Sneak \|0 to
300-(5+3\ *AG) \|- \|0x0140 \|Lockpick \|0 to 300-(10+PE+AG) \|-
\|0x0144 \|Steal \|0 to 300-(3*\ AG) \|- \|0x0148 \|Traps \|0 to
300-(10+PE+AG) \|- \|0x014C \|Science \|0 to 300-(4\ *IN) \|- \|0x0150
\|Repair \|0 to 300-(3*\ IN) \|- \|0x0154 \|Speech \|0 to 300-(5\ *CH)
\|- \|0x0158 \|Barter \|0 to 300-(4*\ CH) \|- \|0x015C \|Gambling \|0 to
300-(5\ *LK) \|- \|0x0160 \|Outdoorsman \|0 to 300-(2*\ (EN+IN)) \|- !
align"center" colspan="3" \| '''Unknown''' \|- \|0x0164-0x016F
\|Unknown. These values are always zero. \| - \|- ! align"center"
colspan="3" \| '''Other''' \|- \|0x0170-0x018F \|Character name \|0 to
32 characters, padded with zeroes \|- \|0x0190 \|First tagged skill \|0
to 17 \|- \|0x0194 \|Second tagged skill \|0 to 17 \|- \|0x0198 \|Third
tagged skill \|0 to 17 \|- \|0x019C \|Fourth tagged skill (This stays
default value unless GCD was edited manually outside of the game.) \|0
to 17, default is FF FF FF FF \|- \|0x01A0 \|First trait \|0 to 15 \|-
\|0x01A4 \|Second trait \|0 to 15 \|- \|0x01A8 \|Character points (for
stats) \|0 to 70-(ST+PE+EN+CH+IN+AG+LK) \|} '''Notes:'''

1. All offsets point to "big endian" DWORDs (4-byte numbers, lowest
   value = 4th byte).

2. Some values can be set higher than the maximum (SPECIAL, for
   example), but starting a game with them won't work. (not sure if this
   means an error/crash, or not having further effect beyond the max.)

3. Not all the maximum values have been verified; some are based on
   (fairly reasonable) assumptions.

4. When setting maximum values, remember that traits can affect stats
   and skills, and primary stats affect skills and secondary stats.

5. Those marked "presumably" are based on comparison with the SAVE.DAT
   file format (and also from studying PRO files).

6. '''Asterisks '''(\*) mark secondary stats which are calculated based
   on primary stats and are not loaded from the GCD. I.e. they are here
   only nominally, and are not actually used.

7. If you tagged a fourth skill, the number of additional tag skill
   points becomes 99 (the default is 3, as you may recall). However,
   it's not possible to tag all the skills, because the restriction to
   three tagged skills remains.

If you play on a Mac, you can use the grammar on
[http://www.synalysis.net/formats.xml synalysis.net] to edit .gcd files
easily.

[http://www.teamx.ru/rus/bd/gcd\_format.shtml Original document] at
teamx.ru (in russian) [[Category:Fallout and Fallout 2 file
formats]][[Category:Translated TeamX documents]]
