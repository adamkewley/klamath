===
CFG
===

The format of the configuration file fallout2.cfg

The paper describes the main configuration file format ''Fallout 2'',
which is located in the main directory of the game.

==Description format fallout2.cfg== {\| border="1" ! [sound] !! [system]
!! [debug] !! [preferences] \|- \| music\_path1 = path \|\| critter\_dat
path = \|\| mode = string \|\| brightness = N \|- \| music\_path2 = path
\|\| critter\_patches path = \|\| output\_map\_data\_info = N \|\|
combat\_difficulty = N \|- \| cache\_size = N \|\| master\_dat path =
\|\| show\_load\_info = N \|\| combat\_looks = N \|- \| initialize = N
\|\| master\_patches path = \|\| show\_script\_messages = N \|\|
combat\_messages = N \|- \| device = N \|\| art\_cache\_size = N \|\|
show\_tile\_num = N \|\| combat\_speed = N \|- \| dma = N \|\|
color\_cycling = N \|\| \|\| combat\_taunts = N \|- \| irq = N \|\|
cycle\_speed\_factor = N \|\| \|\| game\_difficulty = N \|- \| port = N
\|\| executable string = \|\| \|\| item\_highlight = N \|- \|
master\_volume = N \|\| free\_space = N \|\| \|\| mouse\_sensitivity = N
\|- \| music\_volume = N \|\| hashing = N \|\| \|\| player\_speedup = N
\|- \| sndfx\_volume = N \|\| interrupt\_walk = N \|\| \|\| running = N
\|- \| speech\_volume = N \|\| language = string \|\| \|\| subtitles = N
\|- \| N = music \|\| scroll\_lock = N \|\| \|\| target\_highlight = N
\|- \| sounds = N \|\| splash N = \|\| \|\| text\_base\_delay = N \|- \|
speech = N \|\| times\_run = N \|\| \|\| text\_line\_delay = N \|- \|
\|\| \|\| \|\| violence\_level = N \|}

==The main structure of the configuration file fallout2.cfg==
Fallout2.cfg is a plain text file in the root directory of ''Fallout 2''
and consists of several sections, separated by empty strings and
specifying the general configuration of the game. Immediately after
installation the file consists of two sections, [sound] and [system].
They keep track of sound and other game resource files. After starting
the game these sections are expanded, adding [debug] and [preferences],
which contain debugging output settings and preferences for the game,
respectively.

===[sound]=== Prior to the launch of the game has only two options:
music\_path1 = path music\_path2 = path In the music\_path1 sets the
path for the loading music and audio ACM files, which will be copied as
needed from the specified file path music\_path2. For example, the game
was set in minimal- version (about 2 MB) in the
C::raw-latex:`\Fallout`2, with a CD-ROM game is D:. Then the section
will look like this: [sound] music\_path1 =
C::raw-latex:`\Fallout`2:raw-latex:`\data`:raw-latex:`\sound`, music,
music\_path2 = D::raw-latex:`\data`:raw-latex:`\sound`, music,
cache\_size = N (448) Presumably, the buffer size for DirectX audio
output. initialize = N (1) Indicates whether it is necessary to
initialize the sound card when running the game. device = N (-1) dma = N
(-1) irq = N (-1) port = N (-1) Technical parameters sound card, or -1
if you use default DirectSound device. master\_volume = N (22281)
music\_volume = N (22281) sndfx\_volume = N (22281) speech\_volume = N
(22281) Loudness. Used values from 0 to 0x7FFF (32767). The larger the
value, the louder the sound. Variables used for controlling the general
level of volume, volume of music, sound effects and speech,
respectively. music = N (1) sounds = N (1) speech = N (1) Variables
determine whether any music in the game, and it sounds appropriate: 0,
if banned, and 1 if allowed. These installations have priority over
volume variables.

===[system]=== This section is information on the location and
patch-file DAT- files, as well as some useful information. critter\_dat
path = critter\_patches path = master\_dat path = master\_patches path =
These variables defined the paths to critter.dat, possible patch files
for critter.dat, then the path to master.dat, and to patch files for
master.dat. Continuing the previous example, this section will look like
this: [system] critter\_dat = D::raw-latex:`\critter`.dat
critter\_patches = C::raw-latex:`\Fallout`2:raw-latex:`\data `
master\_dat = D::raw-latex:`\master`.dat master\_patches =
C::raw-latex:`\Fallout`2:raw-latex:`\data ` ........ ........ (which, it
is interesting that ''Fallout 2'' patch v1.02d (patch000.dat), if
available, is not established on the path master\_patches but in the
main directory Fallout) art\_cache\_size = N (32) Variable seemed to
indicate the size of the cache art- files. color\_cycling = N (1)
cycle\_speed\_factor = N (1) Variable parameters for storing visual
effect "transfusion" sites around the contour line of attack. Variable
color\_cycling determine whether the spill contour altogether (1 = yes,
0 = no). If a spill, it sets the variable speed cycle\_speed\_factor
overflowing. The higher the value, the slower happening transfusions.
That is just not clear why this should have been placed in fallout2.cfg,
yes, and even in such an important section. executable = string (game)
Rather, there must set the type that gets this game, either, or even
something. Perhaps the debugger. This variable, combined with
Section[debug] makes tightly think ... free\_space = N (20480) Rather,
the amount of free disk space (in kilobytes) necessary for the game.
Default-20 mb. Changing it hasn't been tested. hashing = N (1) Unknown.
interrupt\_walk = N (1) Takes two meanings. If interrupt\_walk = 0, then
you can not set the place (which move) to suspend the movement of the
hero or force him to flee a second click. If interrupt\_walk = 1, it
becomes feasible. language = string (english) Specify language version
of the game. In fact, it is the name of the folder from which the text
of a game should be used. For example, if there indicates "russian", the
text will be taken from "text:raw-latex:`\russian`</code>". scroll\_lock
= N (1) The effect is not known (no effect on the ScrollLock key to the
clipboard and has not). splash N = (auto) Number of the splashscreen
which will appear the next time you start the game. Perhaps chosen at
random. There are 6 screens (see the description of the file format and
RIX converter rix2bmp). times\_run = N (5) Frankly, I do not know%)
Maybe, the number of launches ''Fallout'' succeeded before a final
proposal on the registration of the game.

===[debug]=== The section consists of variables, specifying debugging
treatment plants for the game. (yet to know how it is included%) mode =
string (environment) Unknown. Perhaps the type of debugging.
output\_map\_data\_info = N (0) Presumably, allowing extraction of map
information. show\_load\_info = N (1) Same, but in relation to loading
games. show\_script\_messages = N (1) Probably, taking debugging
information from scripts show\_tile\_num = N (1) Perhaps taking the tile
numbers of floors and walls.

===[preferences]=== This section is fully duplicated options menu
(Preferences Menu) in the game, so I will simply list the variables
here, and the defaults. brightness N = (1.000000) combat\_difficulty = N
(1) combat\_looks = N (0) combat\_messages = N (1) combat\_speed = N (0)
combat\_taunts = N (1) game\_difficulty = N (1) item\_highlight = N (1)
language\_filter = N (0) mouse\_sensitivity N = (1.000000)
player\_speedup = N (0) running = N (0) subtitles = N (0)
target\_highlight = N (2) text\_base\_delay N = (3.500000)
text\_line\_delay N = (1.000000) violence\_level = N (3)
[[Category:Fallout and Fallout 2 file formats]]




-----
Video
-----




CFG files are '''''indexed text-configuration''''' files. They are used for placing fade-in and fade-out effects for videos, and can be found in "<code>master.dat\art\cuts</code>".

==Structure==
The names of CFG files correspond to the name of the MVE file video in which the effects are placed. In the file are text entries like this:
<pre>
;
; comments are indicated by semi-colons
;

[info]                    ; declares basic info about the effects in the video (word "info" in brackets)
total_effects=2           ; declare the total number of fade effects
effect_frames=0,250       ; initializes the start frames for the fade effects, separated by comma(s)

;     <------------ random semi-colons
;

[0]                       ; effect number 1 (distinguished by start frame number, in brackets)
fade_type=in              ; only two possible values for this: in or out
fade_color=0,0,0          ; red - green - blue values for fade-in or fade-out. Can range from 0 - 63, separated by commas
fade_steps=30             ; The number of frames the fade will take place on. Normal movie Frames Per Second (FPS) is 15

[250]                     ; effect number 2 (distinguished by start frame number, in brackets)
fade_type=out             ; only two possible values for this: in or out
fade_color=0,0,0          ; red - green - blue values for fade-in or fade-out. Can range from 0 - 63, separated by commas
fade_steps=30             ; The number of frames the fade will take place on. Normal movie Frames Per Second (FPS) is 15
</pre>

===Notes===
:* So far, ''all''  fade effects have been to black (RGB values 0,0,0), and the number of steps is always 30. It might not be possible to change those values (not verified).

===Example===
Taken from <code>afailed.cfg</code> - the effect file for <code>afailed.mve</code>, [http://fallout.wikia.com/wiki/Hakunin Hakunin's] last spiritual contact with the [http://fallout.wikia.com/wiki/Chosen_One Chosen One].
<pre>
;
; movie.cfg
;

[info]
total_effects=2                 ; Total number of fading effects.
effect_frames=0,130           ; The frame number each effect will start on.

;
;

[0]                             ; Frame number this effect starts on.
fade_type=in                    ; Fade in or out?
fade_color=0,0,0                ; r,g,b value to fade from/to (range is 0-63).
fade_steps=30                   ; number of steps (frames) this fade takes.

[130]                            ; Frame number this effect starts on.
fade_type=out                   ; Fade in or out?
fade_color=0,0,0               ; r,g,b value to fade from/to (range is 0-63).
fade_steps=30                   ; number of steps (frames) this fade takes.
</pre>

===Open with===
.CFG files can be opened with any simple text-editing programs, like Notepad or WordPad.

==TeamX document==
[http://www.teamx.ru/rus/bd/cfg_format.shtml Original document] at teamx.ru (in Russian)
[[Category:Fallout and Fallout 2 file formats]]
[[Category:Translated TeamX documents]]
