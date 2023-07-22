
Make fonts
==========
The script folder contains a modified version of the stm32 font generator (`stm32-font.py`) that creates a abmt compatible struct.
https://www.fontriver.com/ is a good place to find fonts.

Make onwn icons
===============
In the scripts folder is a script (`mk_icons.sh`) that may help you.
- convert -dither None -density 1200 -resize ${SIZE}x${SIZE} $i $i.mono
- xxd -i $i >> tmp_header
