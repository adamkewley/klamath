===
MSG
===

MSG files are '''''indexed text''''' files. They contain in-game
messages, character and item dialogues, and are located in the
text:raw-latex:`\english`:raw-latex:`\game` and
text:raw-latex:`\english`:raw-latex:`\dialog` folders, respectively.

==Structure== Each indexed text line has the format: "{}{}{}"

:\* The first set of curly braces "{}" indicate the line index (number)
in decimal format :\* The second set of braces (optionally) indicates
the name of an [[ACM File Format\|ACM sound file]] :\* The third set of
braces contain the text to be displayed (dialogue box or observation
window)

.. raw:: html

   <pre>
   Dialogue Line Number
          |
          |
     +----+    ACM sound file name
     |                 |
     |                 |
     |            +----+                 Displayed message (dialogue box or observation window)
     |            |                                |
     |            |                                |
     |            |                           +----+
     |            |                           |
     v            v                           v

   {100}  {deadbrahmin001}   {You see a horribly mutilated, two-headed cow. Hamburgers, anyone?}
   </pre>

===Notes=== :\* The length of the lines between the braces cannot exceed
1024 characters :\* The length of a single word cannot exceed 53
characters (max width of the in-game dialog window), the rest is not
displayed :\* Linebreaks inside the braces are allowed '''*''' :*
Linebreaks are '''ignored''' by game engine. That is, if you do

{Hi

there}

in msg, it will be displayed as "Hithere" in-game. :\* At the end of the
file there is a blank line (with no characters in it) :\* The game only
looks for text within braces, which allows leaving comments in between
the indexed lines :\* Comments are usually preceded by a "#", or a pound
symbol, unlike Fallout's scripting language, ''SSL, ''which uses two
slashes "//" '''\*\*''' :\* Sound files are ''generally'' only used in
dialog with 'talking heads' :\* Index is supposed to be unique. If it's
not unique, only the latest string with matching index is used by the
game.

'''\*''' However, a trailing portion of the string ''must'' contain at
least one non-space character before the last brace. Otherwise, the game
will crash. '''\*\*''' The Fallout engine itself treats everything
outside of the braces as comments (ignores them). However, comment
mark-ups could be required by some of the external modding tools.

===Example=== Taken from AHHAKUN.MSG -
[http://fallout.wikia.com/wiki/Hakunin Hakunin's] message / dialogue
file (''Fallout 2'')

.. raw:: html

   <pre>
   {100}{}{Before you stands Hakunin, the village shaman. He appraises you with his crazy eyes from somewhere 
   in the world only he inhabits.....}
   {101}{}{You see Hakunin lying on the ground. He's badly wounded.}
   {102}{}{You see Hakunin. He's very near death.}
   {103}{}{Hakunin is badly burned. He smells like roast chicken, looks like burnt meatloaf, and you don't know 
   how he's managed to stay alive this long.}

   #Commentary: the following line of text plays a sound (the sound file is hak001.acm)

   {104}{hak001}{Greetings, Chosen. Why does the earth before me warm to the touch of your passing?}
   {105}{}{Uh?}
   </pre>

===Open with=== '''.MSG files can be opened with any simple text-editing
programs, like Notepad or WordPad. Additionally, the Fallout Script
Editor has a function that allows you to open and edit message files as
well.'''

''Note'': Editing in a more feature-rich text editor such as Microsoft
Word should not be a problem in theory, but should be treated with care
unless you opt to save the file in Plain Text Format after you're done
making changes. Programs such as Word can use characters that Fallout
could (potentially) not parse properly. For example, Word automatically
changing three periods to one 'rich text' character (an elipses).

Some files that ship with Fallout 1 do have these characters and seem to
work properly, but it's something to keep in mind when editing.
Assumedly the same applies to Fallout 2 {citation needed}

==TeamX document== [http://www.teamx.ru/rus/bd/msg\_format.shtml
Original document] at teamx.ru (in russian) [[Category:Fallout and
Fallout 2 file formats]][[Category:Translated TeamX documents]]
[[Category:Fallout and Fallout 2 file formats]][[Category:Translated
TeamX documents]]
