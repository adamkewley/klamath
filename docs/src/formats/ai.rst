==
AI
==

The file consists of a description of combat parameters for the player
and all NPC classes in the game. Descriptions are separated from one
another with an empty line. Each description consists of the (unique)
name of the class and settings. Below is a list of parameters in
alphabetical order, which is how they are always written in the file.
"string" means that the value should be text; "N" indicates a numeric
value; and "mixed" can be either.

There seems to be a mismatch between some of the string values used and
what the engine actually looks for. Potentially incorrect values are
indicated with a (\*).

'''[Class Name]'''

This option must be specified first. For example, [Enclave Patrol],
[Fighting Mantis] or [Arroyo Villager]. The player is the class named
Player AI. Every party member must have the following classes: [PARTY
NAME AGGRESSIVE] [PARTY NAME BERSERK] [PARTY NAME COWARD] [PARTY NAME
CUSTOM] [PARTY NAME DEFENSIVE] Here NAME is the name of a member of the
team, such as: DOC, LENNY, etc. GORIS

'''aggression = N'''

The chance of aggressive behavior by the NPC in combat. Measured as a
percentage. Valid values are: 0-100.

'''area\_attack\_mode = string'''

Conditions for burst-fire. Existing values, the internal numeric
representation, and the text used for setting custom behavior in-game:
no\_pref\* -1 "Not Applicable." always 0 "Always!" sometimes 1
"Sometimes, don't worry about hitting me" be\_sure 2 "Be sure you won't
hit me" be\_careful 3 "Be careful not to hit me" be\_absolutely\_sure 4
"Be absolutely sure you won't hit me" 'no\_pref' is used by Cyberdog and
Goris, and has a line in custom.msg, but is not checked for by the
engine.

'''attack\_end = N '''

'''attack\_start = N'''

Combat floats said when attacking. At the beginning of the attack, a
random number between the two values is chosen. Then the appropriate
line number from master.dat/text/english/game/combatai.msg is displayed.

'''attack\_who = string'''

Who the NPC should attack. Existing values: whomever\_attacking\_me 0
"Whomever is attacking me" strongest 1 "The strongest" weakest 2 "The
weakest" whomever 3 "Whomever you want" closest 4 "Whomever is closest"

'''best\_weapon = string'''

Preferred weapon types. This option does not exist for all NPCs.
Available values: no\_pref 0 "None" melee 1 "Melee" melee\_over\_ranged
2 "Melee then ranged" ranged\_over\_melee 3 "Ranged then melee" ranged 4
"Ranged" unarmed 5 "Unarmed" unarmed\_over\_thrown ? (Spore Plant)
random ? never\* ? (Deathclaw)

'''body\_type = string'''

Defines the block of messages that critters say in response to being
hit. The blocks are from master.dat/data/AIBDYMSG.TXT, which in turn
refers to lines in combatai.msg . Existing values (examples of critters
that use them in parens): None Primitive Wimpy Person (Whimpy, Coward
...) Punk (SF) Berserk Person (Kamakazi. ..) Tough Person (Bounty
Hunter, Caravan Driver ...) Boxer Normal Person Robot Junkie (Crazy
Addict) Elron (Habologisty) Ghoul Kaga Raider (Khan) Raider (Non-Khan)
Gangster (Mobsters) Shi Super Mutant (Master Army) Goris Miron Marcus
Lenny Cassidy Robo Brain (Hum / Cyb) Sulik

'''called\_freq = N'''

Sets the probability of using an aimed attack, which is calculated as
1/N. Cannot be zero.

'''chance = N'''

The likelihood of any message. Measured as a percentage. Valid values
are: 0-100.

'''chem\_primary\_desire = N'''

Preference for drug use. PIDs of items, or -1 if there is no preference.
If you want to specify multiple types of drugs, they are listed
separated by commas (e.g., 284,81,103). Strangely, most critters use 81
(Iguana-on-a-stick, whole).

'''chem\_use = string'''

NPC drug use in combat. Existing values: clean 0 "I'm clean"
stims\_when\_hurt\_little 1 "Stimpacks when hurt a bit"
stims\_when\_hurt\_lots 2 "Stimpacks when hurt a lot" sometimes 3 "Any
drug some of the time" anytime 4 "Any drug any time" always ?

'''color = N'''

Perhaps the font color used for combat floats. It is 58 for all critter
types.

'''disposition = mixed'''

Determines NPC behavior during a battle. Existing values: -1\* none
custom coward defensive aggressive berserk

'''distance = string'''

The distance between the NPC and its enemy, managing battle tactics.
Existing values: stay\_close 0 "Stay close to me" charge 1 "Charge!"
snipe 2 "Snipe the enemy" on\_your\_own 3 "On your own" stay 4 "Stay
where you are" random\* ? (listed only for Mobsters and Tough Khan)

'''font = N'''

Perhaps, the font used for combat floats. Value in the US version of the
game is 101. In other versions 102, 103, and 104 are used.

'''general\_type = string'''

Defines the block of combat taunts that the critter periodically shouts
during a battle (attack, retreat, miss). The blocks are in
master.dat/data/AIGENMSG.TXT, and refer to lines in combatai.msg .
Existing values: Badger Berserk Person Bounty Hunter Boxer Cassidy Chip
Crazed Robot Cyberdog Dragon (The) Elron Guards Gangster Ghoul Goris
Guard (Normal) Guard (Tough) Junkie Kaga (1) Lenny Lo Pan Marcus Myron
None Normal Person OZ - 7 OZ - 9 Primitive Raider Raider Captain Raider
Mercs Rat God Robot Ryan Shi Guards Sulik Super Mutant (Master Army) The
Brain Tough Person Wimpy Person

'''hit\_eyes\_end = N '''

'''hit\_eyes\_start = N'''

'''hit\_groin\_end = N '''

'''hit\_groin\_start = N'''

'''hit\_head\_end = N '''

'''hit\_head\_start = N'''

'''hit\_left\_arm\_end = N '''

'''hit\_left\_arm\_start = N'''

'''hit\_left\_leg\_end = N '''

'''hit\_left\_leg\_start = N'''

'''hit\_right\_arm\_end = N '''

'''hit\_right\_arm\_start = N'''

'''hit\_right\_leg\_end = N '''

'''hit\_right\_leg\_start = N'''

'''hit\_torso\_end = N '''

'''hit\_torso\_start = N'''

NPC messages when hit in the eyes, groin, head, left arm, left leg,
right arm, right leg and torso, respectively. For details, see the
description of attack\_end.

'''hurt\_too\_much = string'''

Indicates the condition in which the NPC starts running away. Multiple
conditions are separated by commas. blind 40 crippled 3c crippled\_arms
30 crippled\_legs 0c 'crippled' is equivalent to 'crippled\_arms,
crippled\_legs'

'''max\_dist = N'''

The maximum distance from the player where the NPC is hostile. Measured
in hexes.

'''min\_hp = N'''

The minimum number of hit points, at which the NPC starts running away.

'''min\_to\_hit = N'''

The NPC will attack only if it has at least this high a chance
(percentage) of hitting its target.

'''miss\_end = N '''

'''miss\_start = N'''

NPC combat taunts when missing a target. For details, see the
description of ''attack\_end''.

'''move\_end = N '''

'''move\_start = N'''

Combat taunts used when moving to attack (i.e., when not yet in range of
the enemy, but already hostile) For details, see description of
''attack\_end''.

'''outline\_color = N'''

Maybe the color used for target highlighting. For all NPC classes, it is
55.

'''packet\_num = N'''

Number of the AI package. Never change it! If the new AI package you
indicate packet\_num, which already exists, then when you click on the
AI button in the Mapper (in critter properties), it closes the window.
When you create a new AI package, the value of packet\_num must be
consecutive. In AI.TXT there are 187 values for packet\_num, from 0 to
186 inclusive. If you want to add a new package, it should have
packet\_num 187, the next added package 188, and so on. If packet\_num
is not followed consecutively, for example, 188 immediately after 186,
the mapper will close.

'''run\_away\_mode = string'''

Condition when the NPC will run away. Existing values (second column is
minimum amount of damage taken, as a percentage of max HP): coward 0% 0
"Abject coward" finger\_hurts 25% 1 "Your finger hurts" bleeding 40% 2
"You're bleeding a bit" not\_feeling\_good 60% 3 "Not feeling good"
tourniquet 75% 4 "You need a tourniquet" never 100% 5 "Never!" none 0% 6
"None"

'''run\_end = N '''

'''run\_start = N'''

Combat floats used when running away. For details, see the description
of attack\_end.

'''secondary\_freq = N'''

Similar to ''called\_freq''. Sets the probability of using a secondary
(i.e., burst) attack mode, which is calculated as 1/N. Cannot be zero.
Only used when ''area\_attack\_mode'' is 'sometimes' or 'no\_pref'.

[[http://www.teamx.ru/rus/bd/ai\_txt\_format.shtml\| Original document]]
at TeamX.ru (in Russian) [[Category:Fallout and Fallout 2 file
formats]][[Category:Translated TeamX documents]]
