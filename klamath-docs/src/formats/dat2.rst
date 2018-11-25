====
DAT1
====

DAT files are '''''archived data''''' files. They contain the bulk of
the data for ''[http://www.fallout.wikia.com/wiki/Fallout Fallout]'' and
''[http://www.fallout.wikia.com/wiki/Fallout\_2 Fallout 2]'', including
all game artwork, critters, scripts, message/dialogue files, sounds and
speech audio files, and much more. The two DAT files used by the games
are master.dat and critter.dat, both of which are located in the root
game folder. ==DAT1 vs DAT2== There were two different DAT file formats
used for the Fallout games. Both Fallout 1 and Fallout 2 used different
formats but used the same file ending: \*.dat. To avoid
misunderstandings, we'll refer to DAT1 (for the Fallout 1 DAT format)
and DAT2 (for the Fallout 2 version) in this document. It's important
that DAT2 is not an improved DAT1 version, but more a completely
rewritten file type that doesn't have much in common with DAT1.

==DAT1== This specific DAT file type stores data in the
[http://en.wikipedia.org/wiki/Endianness big-endian] format.

===Structure===

.. raw:: html

   <pre>
   int32   4   DirectoryCount
   int32   4   (Unknown1). Usually 0x0A (0x5E for master.dat). Must not be less than 1 or Fallout will crash
                instantly with a memory read error. Possibly some kind of memory buffer size.
   int32   4   (Unknown2). Always 0.
   int32   4   (Unknown3). Could be some kind of checksum, but Fallout seems to work fine with any value.

   // Directory Name Block - for each directory (DirectoryCount times)
   // master.dat starts its listing with a root node, called '.'. This node contains COLOR.PAL and several font files; be careful not to skip it, as it may appear like two extraneous bytes
   byte    1   Length (number of charactes) in DirectoryName
   char    *   DirectoryName

   // Directory Content Block - for each directory (DirectoryCount times)
   int32   4   FileCount. Number of files in the directory.
   int32   4   (Unknown4). Similar to (Unknown1), the default value seems to be 0x0A and Fallout works with 
                   most positive non-zero values.
   int32   4   (Unknown5). Seems to always be 0x10.
   int32   4   (Unknown6). See (Unknown3).
       // File List Block - for each file in directory (FileCount times).
       byte    1   Name length (number of characters)
       char    *   Name
       int32   4   Attributes. 0x20 means plain-text, 0x40 - compressed with LZSS.
       int32   4   Offset. Position in the file (from the beginning of the DAT file), where the file contets start.
       int32   4   Size. Original (uncompressed) file size.
       int32   4   PackedSize. Size of the compressed file in dat. If file is not compressed, PackedSize is 0.

   // Data block
   byte    *   File data for all files (use Offset and [Packed]Size to find where a specific file starts and ends).
   </pre>

=== Fallout 1 LZSS uncompression algorithm ===

Originally written by Shadowbird
[http://www.nma-fallout.com/threads/fallout-dat-files.160366/ on NMA
forum].

-  This is a '''decompression algorithm for files compressed with
   Fallout's LZSS algorithm''', not a file extraction algorithm for
   getting them out of the DAT file! DAT unpackers already incorporate
   this.
-  It's pretty much a generic LZSS decompression algorithm, with a
   possible difference from other implementations in that it doesn't
   prevent overwriting dictionary values while they're being output (see
   the loop in @FLeven).

.. raw:: html

   <pre>
   DICT_SIZE = 4096; // Dictionary (a.k.a. sliding window / ring / buffer) size
   MIN_MATCH = 3;
   MAX_MATCH = 18;

   Int16 N = 0;                      // number of bytes to read
   Int16 DO = 0;                     // Dictionary offset - for reading
   Int16 DI = DICT_SIZE - MAX_MATCH; // Dictionary index - for writing
   Byte L = 0;                       // Match length
   Byte FL = 0;                      // Flags indicating the compression status of up to 8 next characters.

   @Start
   * If at the end of file, exit.
   * Read N from input. The absolute value of N is how many bytes of data to read (if N=0, exit).
   * Go to @N<0 or @N>0 accordingly.

   @N<0
   * Take the absolute value of N (or multiply N by -1), and write that many bytes directly from input to output (without
           putting anything in Dictionary).
   * Go to @Start.

   @N>0
   * Clear dictionary (fill with spaces — 0x20)
   * DI = DICT_SIZE - MAX_MATCH;
   * Go to @Flag.

   @Flag
   * Read FL from input.
   * If N bytes have been read from input, go to @Start, otherwise, go to @Next.

   @Next
   * If this is the 9th time here since last @Flag, go to @Flag.
   * Go to @FLeven or @FLodd as appropriate.

   @FLodd
   * Read 1 byte from input, write it to output and to Dictionary (at position DI).
   * If N bytes have been read from input, go to @Start.
   * DI = DI + 1, or DI = 0 (if past the end of Dictionary).
   * Goto @FlagNext.

   @FLeven
   * Read 1 byte from input to DO.
   * If N bytes have been read from input, go to @Start (in a correctly compressed file this should not ever happen).
   * Read L from input.
   * Prepend the high-nibble (first 4 bits) from L to DO (DO = DO | ((L & 0xF0) << 4)) and remove it from L (L = L & 0x0F).
   * (L + MIN_MATCH) times:
     * Read a byte from dictionary at offset DO (wrap to the start of dictionary if past the end), and write to the output.
     * Write the byte to the Dictionary also, at position DI.
     * DI = DI + 1, or DI = 0 (if past the end of Dictionary).
     * DO = DO + 1.
   * Go to @FlagNext.

   @FlagNext
   * Divide FL by 2, rounding down (FL = FL >> 1).
   </pre>

== DAT2 ([http://en.wikipedia.org/wiki/Endianness Little-endian]) == ===
DAT2 specs Document === You can see Fallout's DAT2 files as if they
where common ZIPs, they're used to store the files that will be used
later inside the game, you can store anything you want, compressed or
not, having the length you want from a common TXT to a immense MVE, and
if you want, you can include a nice but useless DLL file. At the bottom
of the DAT it's included all the information about each one of these
files and some information about the DAT itself.

=== The DAT2 Format === DAT2 files are divided in 3 parts, Data Block,
Directory Tree and Fixed DAT Information block. Data Blocks contains all
the files stored in the DAT, some of them needs to be GZipped, others
don't. The Directory Tree contains all the information about each file
stored in Data Block, as well as the offset where it's located, if it's
compressed or not, packed/unpacked sizes, etc. And finally the Fixed DAT
Information block that contains the size in bytes of both full DAT and
the Directory Tree. Here you can see a small scheme of how DAT's
structure: {\|cellspacing="0" cellpadding="5" border="1"
!style="background:#F0F0FF"\|Part !style="background:#F0F0FF"\|Location
!style="background:#F0F0FF"\|Description \|- \|DataBlock \|.............
............. X \|Files stored in the archive \|- \|FilesTotal \|X+1
\|Number of files in the archive \|- \|DirTree \|X+5 ............. Z
\|Tree information about the files stored in the archive \|- \|TreeSize
\|Z+1 \|Size of DirTree in bytes \|- \|DataSize \|Z+5 \|Full size of the
archive in bytes \|}

\*FilesTotal + DirTree corresponds to Directory Tree block \*\*TreeSize
+ DataSize corresponds to Fixed DAT Information block

=== The Data Block === The Data Block contains just plain files, their
technical information is located in the Directory Tree. Data Block
starts from the very beginning of a DAT file. They can be compressed or
not, (Fallout engine uses zlib stream data compression), if they're
compressed the signature 0x78DA appears at the begin of the file, if
not, there is no signature, the file starts without signature. The
0x78DA compression signature has an integer (2 bytes/WORD) nature.
0x78DA in ASCII is "xÚ" as char is 120 for 'x' and 218 for 'Ú'
Compressed files are "zlib stream data" (RFC-1950(zlib format),
RFC-1951(deflate format), RFC-1952(gzip format)). However, if you attach
this header 1F 8B 08 08 9F E8 B7 36 02 03 to the file, such file could
been easily decompressed with WinZip.

=== The Directory Tree === Directory Tree contains entries that
specifies about a file stored in the Data Block. These entries can be
varying depending on the FilenameSize of the file (Path + Filename).
Like you saw in the scheme located at the beginning of this document,
Directory Tree has been divided into 2 parts, FilesTotal and the
DirTree. FilesTotal contains how many files are stored in the DAT,
DirTree contains all the information about these files. FilesTotal is
declared as a DWORD (4 bytes/Long) type and is read in INTEL L-H format.
Format of DirTree entries DirTree has a private structure. The length of
this structure can vary depending on the length of the Filename (path +
filename). All the entries are DWord types unless it's specified. At the
end of this chapter you can find a scheme on the structure and the way
it's declared on C and Visual Basic programming languages. All the
directories and files are stored in DOS 8.3 format, that is 8 characters
for the file name and 3 characters for the file extension. All the
entries are sorted alphabetically in a descendent direction. Structure
scheme: all Dwords are read in INTEL L-H format.

{\|cellspacing="0" cellpadding="5" border="1"
!style="background:#F0F0FF"\|Name !style="background:#F0F0FF"\|Type
!style="background:#F0F0FF"\|Description \|- \|FilenameSize \|Dword
\|Length of the ASCII filename. \|- \|Filename \|String \|Path and name
of the file, For example,
"text:raw-latex:`\english`:raw-latex:`\game`:raw-latex:`\WORLDMP`.MSG".
The length of the Filename is FilenameSize. \|- \|Type \|Byte
\|Compression boolean: 1 = Compressed 0 = Decompressed \|- \|RealSize
\|Dword \|Size of the file without compression. \|- \|PackedSize \|Dword
\|Size of the compressed file. \|- \|Offset \|Dword \|Address/Location
of the file. \|}

*Dword stands for 4 bytes/long integers 0xNN NN NN NN *\ Word stands for
2 bytes integers 0xNN NN *Byte stands for 1 byte integer 0xNN *\ String
stands for common string bytes "ABCDEF123456!@#$%/][", etc.

'''Declaration of a DirEntry ''' \* C decorated structure: struct
DirEntry { DWORD FilenameSize; char Filename[FilenameSize]; BYTE Type;
DWORD RealSize; DWORD PackedSize; DWORD Offset; }; \* Visual Basic
decorated structure: Type DirEntryId FilenameSize As Long Filename As
String \* 255 End Type

Type DirEntry Type As Byte RealSize As Long PackedSize As Long Offset As
Long End Type

'''Entry Example '''

{\|cellspacing="0" cellpadding="5" border="1"
!style="background:#F0F0FF"\|Offset !style="background:#F0F0FF"\|Values
!style="background:#F0F0FF"\|Description \|- \|13CC46AD \|16 00 00 00
\|FilenameSize, that is, 0x16 \|- \|13CC46B1 \|61 72 74 5C 62 61 63 6B
67 72 6E 64 5C 41 64 6F 62 65 2E 66 72 6D \|ASCII Filename, in this
case: art:raw-latex:`\backgrnd`:raw-latex:`\Adobe`.frm \|- \|13CC46C7
\|01 \|Type: 0x1 = compressed \|- \|13CC46C8 \|6A 2F 01 00 \|RealSize
\|- \|13CC46CC \|94 85 00 00 \|PackedSize \|- \|13CC46D0 \|00 00 00 00
\|Offset, location of the file. 0x00 1st file \|}

\*This exact example can be found on the Team X DAT specs document.

'''How to find a DirTreeAddr (starting location of Directory Tree)'''

To find the beginning of Directory Tree you can use this calculation:
DirTreeAddr = DataSize - TreeSize - 4

== Credits == Original DAT1 format reverse engineered by Shadowbird
(gmail.com, account "shadowbird.lv").

Original DAT2 format reverse engineered by MatuX (matip@fibertel.com.ar)
unless it's specified.

== See also == \*[[Master.dat]][[Category:Fallout and Fallout 2 file
formats]]


# DAT Spec

Source: http://falloutmods.wikia.com/wiki/DAT_file_format

## `dat_file`

```text

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                               .                               |
   |                               .                               |
   |                             data                              |
   |    len = sum(entry.packed_size for entry in tree_entries)     |
   |                               .                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           num_files                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                               .                               |
   |                               .                               |
   |                          tree_entries                         |
   |                       len = tree_size - 4                     |
   |                               .                               |
   |                               .                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                   tree_size (this + tree_entries)             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           file_size                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```

- Top-level container for all archive data
- All multi-byte numbers are little-endian
- `data` and `tree_entries` are variable length.
- `tree_size` holds the size (in bytes) of both `tree_entries` **and** `tree_size`.
- The size of `data` can be calculated by adding the `packed_size` of each `tree_entry`
  in `tree_entries`


## `data`

- Contains the data for all files described in `tree_entries`
- The data of all files is concatenated together with no separators
- The offset and size of each file in `data` is described in the file's respective
  `tree_entry` in `tree_entries`
- A file's data MAY be compressed with zlib compression. Although a file's `tree_entry` contains an
  `is_compressed` flag, a file's compression should be checked by testing that the first two bytes
  of data are the zlib magic number (`0x78da`). If the file is smaller than two bytes, it is not
  compressed.


## `tree_entries`

- Contains metadata for each file in `dat_file`
- A continuous block of `tree_entry`s with no separators
- The starting offset for `tree_entries` can be calculated from `tree_size`
- With some Fallout2 DAT2 files, `tree_entries` can contain duplicate entries, these are ignored


## `tree_entry`

```text

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                         filename_len                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                               .                               |
   |                               .                               |
   |                            filename                           |
   |                   (ASCII, len = filename_len)                 |
   |                               .               ----------------|
   |                               .               | is_compressed |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       decompressed_size                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                          packed_size                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                            offset                             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```

- Contains metadata of a file in `dat_file`
- The data of the file is held in `dat_file`s `data` block, starting at
  `offset` and ending at `offset + packed_size`
- `is_compressed` can have a value of either `0x0` (uncompressed) or `0x1`
  (compressed). For robustness, this flag should be ignored and, instead,
  the first two bytes of the file data should be read for the zlib magic
  number (`0x78da`)
- Filenames are stored in DOS 8.3 format: 8 characters for the file name,
  followed by a period (`.`), followed by a 3 character long extension.