#!/bin/bash

find assets/critters/art -name "*.FRM" | xargs target/klamath frmshow assets/out/color.pal

