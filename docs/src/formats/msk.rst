===
MSK
===

MSK files are masks for the worldmap which mark the territory that is
impassible for the player. Going into the ocean, for example, is
prevented by these masks.

These files are in the :raw-latex:`\data `folder and are named
WRLDMPxx.MSK, where xx is the mask number. Each mask corresponds to one
tile on the world map.

The files a bit mask, where each bit corresponds to one pixel on the
world map. 1 means that the terrain is passible, 0 is impassible.

It could be said that this is just a black and white fixed-size image,
one bit per pixel, without a title.

In these files what's important is not so much the format as the way it
is presented. To work with them there are some tools in the "Files"
section at teamx.ru: [http://www.teamx.ru/files/utils/MSKTools.rar MSK
Tools]] and [[http://www.teamx.ru/files/utils/msk2bmp.rar msk2bmp]].

[http://www.teamx.ru/rus/bd/msk\_format.shtml Original document] at
teamx.ru (in russian)

[[Category:Fallout and Fallout 2 file formats]][[Category:Translated
TeamX documents]]
