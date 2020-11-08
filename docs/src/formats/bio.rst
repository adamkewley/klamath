===
BIO
===

BIO files are **plain text** files. They contain biographies of
the 3 starting characters which can be chosen during the character
creation stage, and can be found in the ``\premade`` folder.

Format
======

There are no automatic linebreaks or scrolling on the
character selection screen, so extra attention should be paid to make
sure the length of each individual line does not exceed 26 characters
(they can be up to 30 lines in length, but it's ugly). Also, the number
of lines of content should not exceed 22 lines. The first line of text
should be "So-and-so's story", in all capitals. Stylistically, there
should be blank lines before and after the title.

.. code-block:: none

   (blank line)                      --
   "CHARACTER NAME'S" STORY:           |
   (another blank line)                |
   "                                   |
                                       |
                                       |
                                       |
                                       |
                                       |
                                       |
         Biography content             | --> 22 lines of content
                                       |
                                       |
                                       |
                                       |
                                       |
                                       |
                                       |
                                       |
                                       |
                                       |
                             "       --
   |                         |
    -------------------------
              |
              v
    26 characters maximum recommended

Example
-------

Taken from Narg's BIO file, combat.bio, exactly as it is
shown (*Fallout 2*):

.. code-block:: none

   NARG'S STORY:

   Narg's exceptional
   physique has made him
   one of the best hunters
   in the tribe. Narg's 
   first, and usually only,
   impulse is to crush 
   anything that he can't
   figure out. Narg has
   become quite adept at
   crushing, and slicing,
   and dicing. Narg would
   like to prove his
   worthiness to lead the
   tribe and he'll let
   nothing stand in his
   way.

Open with
---------

.BIO files can be opened with any simple text-editing
programs, like Notepad or WordPad.

TeamX (c) 2000 document http://www.teamx.ru/site_arc/docs/pc_format_en.rar