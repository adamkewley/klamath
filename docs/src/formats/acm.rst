---
ACM
---

ACM is a compressed audio file format used by Interplay computer
games. ACM files contain audio information stored in a proprietary
format. Video games such as Fallout, Baulder's Gate and BioWare
commonly use this file format.

An ACM file consists of a ``header`` followed by a sequence of
``stream`` elements.


~~~~~~~~~~~~~
Binary Format
~~~~~~~~~~~~~

An ACM file consists of header data followed by a sequence of stream
elements.

~~~~~~
Header
~~~~~~

.. code:: text

    endianness = big

     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |       0x97      |      0x28     |      0x03     |     0x01    |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                          num_samples                          |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |           num_channels          |          frequency          |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   pack_flgs_1   |  pack_flgs_2  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Notes:

- A sample is 16 bits long (in stream)
- ``num_channels``: is 1 for mono, 2 for stereo. However, the field is never really used.
- ``frequency``: bitrate: is ``22050`` for all known ACMs
- ``pack_flgs_1`` and ``pack_flgs_2`` hold flags at particulr is a
  bitset with the


~~~~~~
Header
~~~~~~

This table describes the meaning of header's fields:

+-------------+-----------------------+-----------------------------------------------+
| # of bits   | Value                 | Description                                   |
+=============+=======================+===============================================+
| 24          | 0x97 0x28 0x03        | ACM-signature1                                |
+-------------+-----------------------+-----------------------------------------------+
| 8           | 0x01                  | ACM-signature2                                |
+-------------+-----------------------+-----------------------------------------------+
| 32          | count of samples      | a sample is 16 bit                            |
+-------------+-----------------------+-----------------------------------------------+
| 16          | # of channels         | 1-mono, 2-stereo (actually not used)          |
+-------------+-----------------------+-----------------------------------------------+
| 16          | frequency (bitrate)   | 22050 in all known ACMs (actually not used)   |
+-------------+-----------------------+-----------------------------------------------+
| 4           | packAttrs             | (see below)                                   |
+-------------+-----------------------+-----------------------------------------------+
| 12          | packAttrs2            |                                               |
+-------------+-----------------------+-----------------------------------------------+

Explanation:

'''I.''' First 4 bytes of every ACM-file contain 0x01032897, this is
kind of signature and its exact meaning is opaque.

Taking in account that Sig1 and Sig2 are read separately, one can
conclude that Sig1 is an actual ACM-signature, while Sig2 is, for
example, a version of ACM.

'''II.''' Number of channels and Bitrate are specified in ACM-file, but
they are not used in the games I know (Fallout1, Fallout2 and
Planescape). All the known at the moment files are 22050 Hz. The # of
channels are 1 or 2, but actual count of channels depends on actual
usage of a resulting sound samples.

'''III.''' The next two values specify the behaviour of unpacking
algorithm. The packAttrs denotes the level of recursion in packed-data
processing. If this value is zero, no actual data-unpacking is
performed. The packAttrs2 is number of sub-blocks in a Packed-Block (see
section 2).

Actions:

'''I.''' Based on header values the next variables are defined (the
names correspond to the source code):

a) someSize = 1 << packAttrs (size of one sub-block)

b) someSize2 = someSize \* packAttrs2 (size of one PackedBlock)

c) decBuff\_size = 3\*someSize/2 - 2 (size of "memory"-buffer), this
   values the sum of following geometric progression: 2 + 4 + 8 +...+ (
   1 << (packAttrs-1) ) plus ( 1 << (packAttrs-1) ) once again.

'''II.''' The following arrays are created (their names differs from
source):

a) PackedBlock = DWORD [someSize2] (corresponds to someBuff). It is more
   convenient to think of this array as of variable-length array DWORD
   [packAttrs2][someSize] or DWORD [cnt\_of\_blocks][block\_len]. This
   array is used to process packed data.

b) if packAttrs is not zero, then the following array is created and
   initialized with zero: MemoryBuffer = DWORD [decBuff\_size]
   (corresponds to decompBuff). This array can be thought of as the
   following structure:

   { WORD [someSize/2][2], DWORD [someSize/4][2], DWORD
   [someSize/16][2], ... DWORD [1][2] }

Each item is used at a corresponding recursion level. This buffer is
used as a seam, as a glue between two consecutive PackedBlocks during
data-unpacking.

c) AmplitudeBuffer = WORD [0x10000], and Buff\_Middle =
   &amplitudeBuffer[0x8000], the latter is used as an array of WORDs
   with indices from -0x8000 to 0x7FFF. The buffer is filled with
   amplitude values used in subsequent unpacking.

==2. ACM-Stream==

This is a bit-stream. It is organized as a collection of bit-blocks,
each of which can be unpacked into someSize2 samples, i.e. into one
PackedBlock. Let's call such a bit-block just a BitBlock. Then an
ACM-Stream is just a sequence of BitBlocks (with various length).

Each BitBlock contains information about amplitudes used in
corresponding PackedBlock and the description of how to manage these
amplitudes in order to produce sound samples.

'''I.''' The first 20 bits of BitBlock can be considered as its header:

::

      bits | name
     ------+------
        4  | pwr
       16  | val

Using these values the AmplitudeBuffer is filled as following:

::

      count = 1 << pwr
      Buff_Middle [+0] = 0
      Buff_Middle [+1] = val
      Buff_Middle [+2] = 2*val
      ...
      Buff_Middle [+(count-1)] = (count-1)*val
      Buff_Middle [-1] = -val
      Buff_Middle [-2] = -2*val
      ...
      Buff_Middle [-count] = -count*val

'''II.''' PackedBlock is filled with values from AmplitudeBuffer. This
is done with help of special filling subroutines (or Fillers).

There are 14 different Fillers (their description will be given later):

::

      Zero, Ret0, Linear, k13, k12, t15, k24,
      k23, t27, k35, k34, k45, k44 and t37.

Each (well, almost each) Filler is intended to fill only one column of
PackedBlock, which is considered as DWORD[packAttrs2][someSize] (n-th
column in this case is an array { PB[0][n], PB[1][n], ... , PB[pa2-1][n]
}).

For each column of PackedBlock one of Fillers is invoked. To select the
Filler, 5 bits are read from BitBlock, and this 5-bit value is used as
an index in the following array:

::

      Filler[32] = {Zero, Ret0, Ret0, Linear, Linear, Linear, Linear, Linear,
         Linear, Linear, Linear, Linear, Linear, Linear, Linear, Linear,
         Linear, k13, k12, t15, k24, k23, t27, k35,
         k34, Ret0, k45, k44, Ret0, t37, Ret0, Ret0};

Two parameters are passed to a Filler: its index in the array and the
number of a column it is applied to.

Thus the filling of PackedBlock with amplitudes can be outlined with the
following C-pseudocode:

::

      for (int i=0; i<someSize; i++) {
         int Ind = get_bits_from_BitBlock (5);
         (Fillers [i]) (Ind, i);
      }

'''III.''' Description of Fillers.

1)  Zero. Fills the column with zero. Does not use any bits from
    BitBlock.

2)  Ret0. Breaks the filling of current PackedBlock, discards its
    contents and starts a new BitBlock. In rather large ACM-files I've
    tested I have not found this kind of Filler.

3)  Linear. 'Ind' parameter is the number of bits from BitBlocks which
    are used as an index of a value in AmplitudeBuffer. In pseudocode:

    Linear (int Ind, int column\_n) { for (int i=0; i<packAttrs2; i++) {
    int val = get\_bits\_from\_BitBlock (Ind); PackedBlock
    [i][column\_n] = Buff\_Middle [val]; } }

4)  k13. Uses variable count of bits (up to 3) from BitBlock to fill the
    column:

    bit-sequence \| action (in order of \| appearance) \|
    --------------+------------- 0 \| PB[i][n] = 0; PB[++i][n] = 0 1, 0
    \| PB[i][n] = 0 1, 1, 0 \| PB[i][n] = Buff\_Middle [-1] 1, 1, 1 \|
    PB[i][n] = Buff\_Middle [+1]

5)  k12. Up to 2 bits: bit-seq. \| value ----------+----------- 0 \| 0
    1, 0 \| Buff\_Middle [-1] 1, 1 \| Buff\_Middle [+1]

6)  k24. Up to 4 bits: bit-seq. \| value(s) ----------+----------- 0 \|
    0, 0 1,0 \| 0 1,1,0,0 \| Buff\_Middle [-2] 1,1,1,0 \| Buff\_Middle
    [-1] 1,1,0,1 \| Buff\_Middle [+1] 1,1,1,1 \| Buff\_Middle [+2]

7)  k23. Up to 3 bits: bit-seq. \| value ----------+----------- 0 \| 0
    1,0,0 \| Buff\_Middle [-2] 1,1,0 \| Buff\_Middle [-1] 1,0,1 \|
    Buff\_Middle [+1] 1,1,1 \| Buff\_Middle [+2]

8)  k35. Up to 5 bits: bit-seq. \| value(s) /----> 2bits \| B\_M index
    --------------+----------- \| -------+----------- 0 \| 0, 0 \| 0,0
    \| -3 1,0 \| 0 \| 1,0 \| -2 1,1,0,0 \| Buff\_Middle [-1] \| 0,1 \|
    +2 1,1,0,1 \| Buff\_Middle [+1] \| 1,1 \| +3 1,1,1, 2bits \| (\*)
    -------------/

9)  k34. Up to 4 bits: bit-seq. \| value /----> 2bits \| B\_M index
    ------------+----------- \| -------+----------- 0 \| 0 \| 0,0 \| -3
    1,0,0 \| Buff\_Middle [-1] \| 1,0 \| -2 1,0,1 \| Buff\_Middle [-1]
    \| 0,1 \| +2 1,1, 2bits \| (\*) -------------/ 1,1 \| +3

10) k45. Up to 5 bits: bit-seq. \| value -------------+----------- 0 \|
    0, 0 1,0 \| 0 1,1, 3bits \| 3bits->B\_M index: 000-> -4, 100-> -3,
    010-> -2, 110-> -1 001-> +1, 101-> +2, 011-> +3, 111-> +4

11) k45. Up to 4 bits: bit-seq. \| value -----------+----------- 0 \| 0
    1, 3bits \| 3bits->index: 000-> -4, 100-> -3, 010-> -2, 110-> -1
    001-> +1, 101-> +2, 011-> +3, 111-> +4

12) t15. Takes 5 bits from BitBlock. This value is considered as a
    base-3 number with 3 digits. Each digit is used as an index in
    Buff\_Middle. So 3 consecutive items of a column are filled.

13) t27. Reads 7 bits and treats obtained value as base-5 number with 3
    digits. Each digit is used as an index in Buff\_Middle, in this way
    3 consecutive items of a column are filled.

14) t37. Takes 7 bits and treats their value as base-11 number with 2
    digits. Each digit is used as an index in Buff\_Middle, in this way
    two consecutive items of a column are filled.

'''IV.''' Processing of data in PackedBlock.

If packAttrs is zero, we do not have do to anything else, we've got the
sound samples in PackedBlock. Otherwise we need to apply unpacking
algorithm to values in PackedBlock to gain sound samples. This is done
in functions unpackValues, sub\_4d3fcc and sub\_4d420c (see source).

All I can tell about the unpacking is that it is recursive. During the
process the PackedBlock is treated as DWORD
[cnt\_of\_blocks][block\_len]. At the beginning of the processing

cnt\_of\_blocks = packAttrs2\*2 block\_len = someSize/2.

From some recursion level to the next level cnt\_of\_blocks is
multiplied by 2 and block\_len is divided by 2, so the value
cnt\_of\_blocks\*block\_len is a constant.

Also I can describe the properties of results from this data processing.
NOTE: to study these properties, an algorithm was slightly altered: in
unpackValues function the for-loop with increment of items of
PackedBlock was commented out. This increment just adds a constant value
to all the items of resulting array, so it is not a core part of
algorithm.

Let's call F(PB) the result of applying of functions mentioned above to
a PackedBlock PB. And denote by PB{[i]=v} a PackedBlock where all the
items are equal zero, except for the i-th one, which is assigned a value
of v. That is PB{[1]=5} symbolizes the array {0,5,0,...,0}.

The following properties of F can be observed:

1) Compact support. F (PB{[i]=v}) is zero everywhere, except for items
   from i to i+2\*someSize-1.
2) Periodicity with period someSize. F (PB{[i]=v}) and F
   (PB{i+someSize}=v) differ only in the way, that the latter one is
   shifted by someSize.
3) Linearity. F (PB{[i]=a, [j]=b}) = a\ *F (PB{[i]=1}) + b*\ F
   (PB{[j]=1}).
4) Almost orthogonality (a strange one). Let's denote f\_i = F
   (PB{[i]=1}), where i=0..someSize-1. w\_i = F (PB{ [i]=1,
   [i+someSize]=1, [i+2\*someSize]=1, ... } ), shifted to the left by
   (2\*someSize) items (that is we simply discard first items). And
   group their indices in the following way: g0 = {0} g1 = {1} g2 = {2,
   3} g3 = {4, 5, 6, 7} ... g\_packAttrs = { ..., (someSize-1) } Then
   any two vectors w\_i and f\_j are orthogonal (in sense of inner
   product) if their indices belong to different groups.

Moreover, w\_i and f\_j belonging to the same group are orthogonal if i
and j are of different parity (I am not sure of this term; what I mean
to say is that one of them is even while the another is odd). May be
w\_i can be treated as test-functions for presence of f\_j in a signal.

By Abel, Found in [[TeamX]]'s
[http://www.teamx.ru/eng/files/docs/index.shtml offline
docs][[Category:Fallout and Fallout 2 file formats]]
