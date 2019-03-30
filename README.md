# klamath

Utilities for working with Fallout 1/2 assets


# Usage

```
# view help
klmth
klmth --help

# extract a DAT file into current dir
klmth dat2extract <DAT2_FILE>

# print AAF glyphs as plaintext
klmth aafprint [AAF_FILE]...

# print FRM header as plaintext
klmth frmheader [FRM_FILE]...

# show FRMs in a window
klmth frmshow [FRM_FILE]...

# print PAL RGB values as plaintext
klmth palprint [PAL_FILE]
```


# Building

```
# install to ~/.local/bin
make install
```


This project contains small utlities for parsing + working with
Fallout 1/2's assets. Each parser/tool is designed to be mostly
standalone, rather than being integrated into an actual game
engine. 

Open-source Fallout 1/2 platform.

**prototype stage**: Currently developing IO libraries for Fallout 1/2 assets, based on reference implementation
                     from [falltergeist](https://github.com/falltergeist/falltergeist/)