===
INT
===

Format files in INT- Fallout

The paper describes the format of INT Files for Fallout and Fallout2.
Information about this was obtained by analyzing source compiler scripts
for Fallout2. Baseline compiler texts were obtained from the source
compiler scripts for the game Klingon Academy. Compilers for the two
games overlap by about 80%.

==Background==

INT files are compiled scripts, designed for animating the world of
Fallout. They set the behavior of all objects on the maps, and are used
to create dialogues with the characters in the game.

==File Format==

INT files consists of the following parts:

*Script initialization code *\ A table of procedure descriptions *A list
of the names of procedures and variables *\ A list of text strings
*Script object initialization code *\ The body of procedures

Note: All numbers are stored in big-endian format (the most significant
byte first).

===Script Initialization Code===

Script initialization code is for the transfer of control to script
object initialization code and providing the correct procedure return
addresses(?).

This part of an INT file should have a fixed size of 42 bytes, because
that is the offset at which it looks for the table of procedures.

Table 2.1 - script initialization code {\|border="1" cellpadding="5"
cellspacing="0" !style="background:#F0F0FF"\|Offset
!style="background:#F0F0FF"\|Size !style="background:#F0F0FF"
colspan="2"\|Value !style="background:#F0F0FF"\|Description \|- \|0x0000
\|2 bytes \|0x8002 \|O\_CRITICAL\_START \|Start of 'Critical' code
section. (see 'script object initialization code') \|- \|0x0002 \|2
bytes \|0xC001 \|O\_INTOP \|rowspan="2"\|Return address of 'start'
procedure \|- \|0x0004 \|4 bytes \|0x00000012 \|INT (0x00000012) \|-
\|0x0008 \|2 bytes \|0x800D \|O\_D\_TO\_A \|Entry return address of the
'start' procedure in the stack addresses \|- \|0x000A \|2 bytes \|0xC001
\|O\_INTOP \|rowspan="2"\|Starting address of script object
initialization code \|- \|0x000C \|4 bytes \|... \| \|- \|0x0010 \|2
bytes \|0x8004 \|O\_JMP \|Jump to script object initialization code \|-
\|0x0012 \|2 bytes \|0x8010 \|O\_EXIT\_PROG \|rowspan="12"\|The specific
function of each opcode is not entirely clear. Apparently they support
the correct completion of initialization scripts. \|- \|0x0014 \|2 bytes
\|0x801A \|O\_POP \|- \|0x0016 \|2 bytes \|0x8020
\|O\_POP\_FLAGS\_RETURN \|- \|0x0018 \|2 bytes \|0x801A \|O\_POP \|-
\|0x001A \|2 bytes \|0x8021 \|O\_POP\_FLAGS\_EXIT \|- \|0x001C \|2 bytes
\|0x801A \|O\_POP \|- \|0x001E \|2 bytes \|0x8022
\|O\_POP\_FLAGS\_RETURN\_EXTERN \|- \|0x0020 \|2 bytes \|0x801A \|O\_POP
\|- \|0x0022 \|2 bytes \|0x8023 \|O\_POP\_FLAGS\_EXIT\_EXTERN \|-
\|0x0024 \|2 bytes \|0x8024 \|O\_POP\_FLAGS\_RETURN\_VAL\_EXTERN \|-
\|0x0026 \|2 bytes \|0x8025 \|O\_POP\_FLAGS\_RETURN\_VAL\_EXIT \|-
\|0x0028 \|2 bytes \|0x8026 \|O\_POP\_FLAGS\_RETURN\_VAL\_EXIT\_EXTERN
\|}

===Table of procedures===

The table of procedures contains information about all the available
procedures in an INT file.

It should be noted that the first procedure in INT files is a procedure
named '..............' (14 periods). This procedure does not have a
body, and is designed to make indices of procedures start at 1, not 0.

Table 2.2-Table of procedures {\|border="1" cellpadding="5"
cellspacing="0" !style="background:#F0F0FF"\|Offset
!style="background:#F0F0FF"\|Size !style="background:#F0F0FF"
colspan="2"\|Description \|- \|0x0000 \|4 bytes \| \|The number of
procedures in the table \|- \|0x0004 \|4 bytes \|rowspan="6"\|Entry0
\|Name procedure (offset in the list of names). \|- \|0x0008 \|4 bytes
\|Set flags according to the type of procedure. P\_TIMED = 0x01 - Timed
(delayed?) procedure P\_CONDITIONAL = 0x02 - Conditional procedure
P\_IMPORT = 0x04 - Imported procedure P\_EXPORT = 0x08 - Exported
procedure P\_CRITICAL = 0x10 - Critical procedure

'''Note:'''

Creating timed and conditional procedures is not entirely understood. It
is also not clear if they are used at all in Fallout.

Because of a bug in the original BIS compiler cannot compile conditional
procedures. \|- \|0x000C \|4 bytes \|The delay in milliseconds for timed
procedures \|- \|0x0010 \|4 bytes \|The position in the file code
expressing conditions for conditional procedures (the offset from the
beginning of the file). \|- \|0x0014 \|4 bytes \|The position in the
file of the function body (offset from the beginning of the file). \|-
\|0x0018 \|4 bytes \|The number of arguments of the procedure \|- \|...
\|... \|... \|... \|- \|colspan="4"\|Note: The offset in the table given
from the beginning of the table of procedures, not the start of the
file. \|}

===List of procedure and variable names===

This part of the INT file contains the names of script variables and
procedures. It does not include the names of local variables declared
within procedures.

Some information in this section may be redundant, since referring to
global variables is done by index, not by name.

Table 2.3. List of names of procedures and variables {\|border="1"
cellpadding="5" cellspacing="0" !style="background:#F0F0FF"\|Offset
!style="background:#F0F0FF"\|Size !style="background:#F0F0FF"\|Value
!style="background:#F0F0FF"\|Description \|- \|0x0000 \|4 bytes
\|totalSize \|Size of the list of names, except the ending, in bytes.
\|- \|0x0004 \|2 bytes \|length0 \|The length of name 0, including the
null (zero) terminator, in bytes.

'''Note:'''

Length 'object name' MUST be EVEN. Otherwise, the INT file becomes
unexecutable. \|- \|0x0006 \|length0 bytes \| \|'Name 0'

A string of ASCII characters ending with a null (zero) byte. Another
null will be added if necessary to make the total length of the string
even. \|- \|... \|... \|... \|... \|- \|totalSize \|4 bytes \|0xFFFFFFFF
\|Indicates the end of the list. \|- \|colspan="4"\|Note: The offset in
the table are given from the beginning of the list of names, not the
start of the file.

The addresses of names/strings are specified using offsets from the
beginning of the list and point to the first character of the string,
not its length. \|}

===List of strings===

This part of the INT file contains all the string constants used in the
script. The format is the same as the list of procedure and variables.
It is possible for this section to be empty (i.e. no strings are used).

===Script object initialization code===

This part of the script is to initialize global script variables, export
and initialize variables, export procedures, and moving to the 'start'
procedure (or returning to the script initialization code if there isn't
one).

| Table 2.5. Script object initialization code {\|border="1"
  cellpadding="5" cellspacing="0" !style="background:#F0F0FF"\|Offset
  !style="background:#F0F0FF"\|Size !style="background:#F0F0FF"
  colspan="2"\|Value !style="background:#F0F0FF"
  colspan="2"\|Description \|- \|0x0000 \|2 bytes \|0x802C
  \|O\_SET\_GLOBAL \| \|The specific meaning of this opcode is not
  entirely clear. Possibly for switching to the global script stack. \|-
  \|0x0002 \|2 bytes \|0x9001 0xA001 0xC001 \|O\_STRINGOP O\_FLOATOP
  O\_INTOP \|Variable Type \|rowspan="2"\|Global variable 0 \|- \|0x0004
  \|4 bytes \|value0 \| \|Value of variable \|- \|... \|... \|... \|...
  \|... \|... \|- \| \|2 bytes \|0x9001 \|O\_STRINGOP
  \|rowspan="2"\|Variable name \|rowspan="3"\|Exports variable named
  name0 \|- \| \|4 bytes \|name0 \| \|- \| \|2 bytes \|0x8016
  \|O\_EXPORT\_VAR \|Exports variable \|- \|... \|... \|... \|... \|...
  \|... \|- \| \|2 bytes \|0x9001 0xA001 0xC001 \|O\_STRINGOP O\_FLOATOP
  O\_INTOP \|Type of assigned value \|rowspan="5"\|Assigns the value
  value0 to the external variable name0 \|- \| \|4 bytes \|value0 \|
  \|Value \|- \| \|2 bytes \|0x9001 \|O\_STRINGOP
  \|rowspan="2"\|Variable name \|- \| \|4 bytes \|name0
| \| \|- \| \|2 bytes \|0x8015 \|O\_STORE\_EXTERNAL \|Assign to external
  variable \|- \|... \|... \|... \|... \|... \|... \|- \| \|2 bytes
  \|0xC001 \|O\_INTOP \|rowspan="2"\|The number of arguments for the
  procedure \|rowspan="5"\|Export the procedure 'name\_p' \|- \| \|4
  bytes \|numArgs \|
| \|- \| \|2 bytes \|0x9001 \|O\_STRINGOP \|rowspan="2"\|Procedure name
  \|- \| \|4 bytes \|name\_p \| \|- \| \|2 bytes \|0x8017
  \|O\_EXPORT\_PROC \|Export procedure \|- \|... \|... \|... \|... \|...
  \|... \|- \| \|2 bytes \|0xC001 \|O\_INTOP \|rowspan="2"\|Variable
  name \|rowspan="3"\|The number of arguments for the 'start' procedure.
  \|- \| \|4 bytes \|0 \|INT (0) \|- \| \|2 bytes \|0x8003
  \|O\_CRITICAL\_DONE \|End of 'Critical' code section (see 'script
  initialization code'). \|- \| \|2 bytes \|0xC001 \|O\_INTOP
  \|rowspan="2"\|Address transition \|rowspan="3"\|The transition to the
  procedure 'start', and or return to the script initialization code if
  there isn't one \|- \| \|4 bytes \|address off the 'start' procedure

or

| 0x00000012
| \| \|- \| \|2 bytes \|0x8004 \|O\_JMP \|The jump to the 'start'
  procedure or return to script initialization code \|}

===Body of procedure code===

This part of the INT file contains a sequence of opcodes implementing
the procedures of the script.

Discussion of the meaning of opcodes and their arguments is beyond the
scope of this paper.

Original file taken from [http://www.teamx.ru TeamX] documentation

[[Category:Fallout and Fallout 2 file formats]]
