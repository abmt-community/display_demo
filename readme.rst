
Make fonts
==========
The script folder contains a modified version of the stm32 font generator (`stm32-font.py`) that creates a abmt compatible struct.
https://www.fontriver.com/ is a good place to find fonts.

Make onwn icons
===============
In the scripts folder is a script (`mk_icons.sh`) that may help you.
- convert -dither None -density 1200 -resize ${SIZE}x${SIZE} icon.png icon.mono
- xxd -i icon.mono >> icon.h

There are pre compiled iconoir icons in the script folder. 

Included fonts
==============
- http://viznut.fi/unscii/ [License Public Domain]


JSON Display Elements
======================
box
---
- x: <val | "val %">; default: 0
- y: <val | "val %">; default: 0
- w: <val | "val %">; default: 1 [width]
- h: <val | "val %">; [height]
- m: <val>; default 0; [marin]
- b: <val>; default 0; negative numbers add up to margin [border]
- p: <val>; default 0; [padding]
- r: <4|6|8>; default 0; [border radius]
- al: "<tl|tc|tr|cl|cc|cr|bl|bc|br>"; default tl; [align]
- disp: <true|false>; default: true; [display]
- <name>: {type: "<element>", "<param>": <value> ...}

text
----
- txt: <text>
- fnt: <val>: default "default"; "default" == first font in list
- scale: <val>; default 1;
- x: <val | "val %">; default: 0
- y: <val | "val %">; default: 0
- m: <val>; default 0; [marin]
- b: <val>; default 0; negative numbers add up to margin [border]
- p: <val>; default 0; [padding]
- r: <4|6|8>; default 0; [border radius]
- al: "<tl|tc|tr|cl|cc|cr|bl|bc|br>"; default tl; [align]
- disp: <true|false>; default: true; [display]


icon
----
- name: <img_name> | "" -> no image
- x: <val | "val %">; default: 0
- y: <val | "val %">; default: 0
- m: <val>; default 0; [marin]
- b: <val>; default 0; negative numbers add up to margin [border]
- p: <val>; default 0; [padding]
- r: <4|6|8>; default 0; [border radius]
- al: "<tl|tc|tr|cl|cc|cr|bl|bc|br>"; default tl; [align]
- disp: <true|false>; default: true; [display]

line
----
- x1: <val | "val %">; default: 0
- y1: <val | "val %">; default: 0
- x2: <val | "val %">; default: 0
- y2: <val | "val %">; default: 0
- w:  <val>; default 1;
- disp: <true|false>; default: true; [display]

hline
-----
- y: <val | "val %">; default: 0
- w: <val>; default 1;
- disp: <true|false>; default: true; [display]

vline
-----
- x: <val | "val %">; default: 0
- w: <val>; default: 1
- disp: <true|false>; default: true; [display]

Examples
========
``$ echo '{"t": {"type": "text", "txt": "Hello World", "scale": 3, "fnt": "mcr"}}' | nc -q0 -u epd_demo.local 3333``

.. code-block:: bash
 
 echo '{
  "status": {"type": "text", "txt": "Hello World", "scale": 1, "p": 3, "fnt": "mcr"},
  "bat": {"type": "icon", "name": "battery_charging", "al": "tr"},
  "l1": {"type": "hline", "y": 16},
  "main": { "type": "box", "y": "16%", "h": "84%", "w": "84%", "b": 0,
    "text": {"type": "text", "txt": "HELLO", "al": "cc", "scale": 3, "fnt": "mcr"}
  },
  "btns": { "type": "box", "w": "16%", "h": "84%", "al": "br", "b": 0, 
    "b1": { "type": "box", "w": "100%", "h": "33%", "al": "tl", "b": 1, "i": {"type": "icon", "name": "skip_next", "al": "cc"}},
    "b2": { "type": "box", "w": "100%", "h": "33%", "al": "cl", "b": 1, "y": -1, "i": {"type": "icon", "name": "play", "al": "cc"}},
    "b3": { "type": "box", "w": "100%", "h": "33%", "al": "bl", "b": 1, "y": -2,"i": {"type": "icon", "name": "skip_prev", "al": "cc"} }
  }
 }' | nc -q0 -u epd_demo.local 3333

