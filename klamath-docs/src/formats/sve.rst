===
SVE
===

SVE files are '''''indexed text''''' files. They contain subtitles for
Interplay videos, and can be found in the
"text%language%:raw-latex:`\cuts`</code>" folder.

==Format== The format of these files is simple - each line contains a
number (the frame number in the video), immediately followed by a colon,
then followed by the actual subtitle text.

.. raw:: html

   <pre>
   Start Frame Number
        |
        |
    +----   Colon (indicates where text starts)
    |            |
    |            |
    |       +----+     Subtitle Text
    |       |               |
    |       |               |
    |       |          +----+
    |       |          |
    v       v          v

   "20"     :     "War. War never changes."
   </pre>

On reaching the specified frame while playing the video, the
corresponding text will be displayed. Each following subtitle replaces
the previous one until the end of the video.

The name of the SVE file is the same as the corresponding [[MVE File
Format\|MVE video file]] in the "art:raw-latex:`\cuts`</code>" folder.

===Example=== Taken from credits.sve (''Fallout 2''):

.. raw:: html

   <pre>
   20:War. War never changes.
   70:The end of the world occurred pretty much as we predicted
   125:Too many humans, not enough space or resources to go around.
   190:The details are trivial and pointless, the reasons, as always, purely human ones.
   285:The Earth was nearly wiped cleaned of life.
   </pre>

===Open with=== .SVE files can be opened with any simple text editing
program, like Notepad or WordPad.

==TeamX Document== [http://www.teamx.ru/rus/bd/sve\_format.shtml
Original document] at teamx.ru (in Russian) [[Category:Fallout and
Fallout 2 file formats]][[Category:Translated TeamX documents]]
