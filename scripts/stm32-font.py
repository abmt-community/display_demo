#!/usr/bin/python3
# Modified version to create abmt fonts
# source: https://github.com/zst-embedded/STM32-LCD_Font_Generator
# SPDX-License-Identifier: AGPL-3.0

# other symbols:
# https://android.googlesource.com/platform/frameworks/base/+/56a2301/data/fonts
# ./stm32-font.py -f /usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf -s 30 -c ch
# 
# In some cases you may adjust the margin in line 77


from PIL import Image, ImageFont, ImageDraw
import argparse
import math
import re
import time
import textwrap
import regex

# Greyscale threshold from 0 - 255
THRESHOLD = 128
# Font Character Set
#CHAR_SET = ' !"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~'
CHAR_SET = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,+-:;!"#$%&\'()*/<=>?@[\]^_`{|}~'

def get_charset_perceived():
    # https://stackoverflow.com/questions/6805311/playing-around-with-devanagari-characters
    return regex.findall(r'\X', CHAR_SET)


def get_max_width(font):
    widths = []
    for ch in get_charset_perceived():
        w, h = font.getsize(ch)
        widths.append(w)
    return max(widths)


def bin_to_c_hex_array(bin_text, bytes_per_line, lsb_padding=0, msb_padding=0):
    # create comment with preview of line
    comment = bin_text.replace('0', ' ').replace('1', '#')

    # pad the top or bottom remaining bits with 0's
    bin_text = ("0" * msb_padding) + bin_text + ("0" * lsb_padding)
    # ensure the length matches the number of bytes
    assert len(bin_text) == (bytes_per_line * 8)

    # split up into 8 digits each of bytes
    bin_list = re.findall('.{8}', bin_text)
    # convert to hex representation
    bin_list = map(lambda a: "0x{:02X}".format(int(a, 2)), bin_list)
    array = ', '.join(bin_list)

    return f'{array}, /* |{comment}| */\r\n'


def generate_font_data(font, x_size, y_size):
    data = ''

    # find bytes per line needed to fit the font width
    bytes_per_line = math.ceil(x_size / 8)
    empty_bit_padding = (bytes_per_line * 8 - x_size)

    for i, ch in enumerate(get_charset_perceived()):
        # the starting array index of the current char
        array_offset = i * (bytes_per_line * y_size)
        assert data.count('0x') == array_offset

        # comment separator for each char
        data += '\r\n'
        data += f"// @{array_offset} '{ch}' ({font_width} pixels wide)\r\n"

        # Calculate size and margins for centered text
        w, h = font.getsize(ch)
        x_margin = (x_size - w) // 2
        y_margin = (y_size - h) // 2
        margin = (x_margin, y_margin)
        im_size = (x_size, y_size)

        # create image and write the char
        im = Image.new("RGB", im_size)
        drawer = ImageDraw.Draw(im)
        drawer.text(margin, ch, font=font)
        del drawer

        # for each row, convert to hex representation
        for y in range(y_size):
            # get list of row pixels
            x_coordinates = range(x_size)
            pixels = map(lambda x: im.getpixel((x, y))[0], x_coordinates)
            # convert to bin text
            bin_text = map(lambda val: '1' if val > THRESHOLD else '0', pixels)
            bin_text = ''.join(bin_text)
            # convert to c-style hex array
            data += bin_to_c_hex_array(bin_text, bytes_per_line,
                                       lsb_padding=empty_bit_padding)
    return data


def output_files(font, font_width, font_height, font_data, font_name, char_set):
    generated_time = time.strftime("%Y-%m-%d %H:%M:%S")

    # create filename, remove invalid chars
    filename = f'font_{font_name}_{font_height}'
    filename = ''.join(c if (c.isalnum() or c=='_') else '' for c in filename)

    # C file template
    output = f"""/**
 * This file provides '{font_name}' [{font_height}px] text font
 * for STM32xx-EVAL's LCD driver.
 *
 * Generated with STM32-LCD_Font_Generator on {generated_time}
 */
#pragma once

#include <abmt/img.h>

// {font_data.count('0x')} bytes
const uint8_t {filename}_data [] = {{{font_data}}};

const abmt::font {filename}_font = {{
    {font_width},  // width 
    {font_height}, // height
    {font_width}/9,  // space 
    {font_height}/8, // linespace
    {filename}_data,
    "{char_set}"    
}};

//@node:auto
struct {filename}{{
    abmt::font out_font = {filename}_font;
}};

"""
    # Output font C header file
    with open(f'{filename}.h', 'w') as f:
        f.write(output)

    # Output preview of font
    size = font.getsize(CHAR_SET)
    im = Image.new("RGB", size)
    drawer = ImageDraw.Draw(im)
    drawer.text((0, 0), CHAR_SET, font=font)
    im.save(f'{filename}.png')


if __name__ == '__main__':
    # Command-line arguments
    parser = argparse.ArgumentParser(
        description='Generate text font for STM32xx-EVAL\'s LCD driver')

    parser.add_argument('-f', '--font',
                        type=str,
                        help='Font type [filename]',
                        required=True)
    parser.add_argument('-s', '--size',
                        type=int,
                        help='Font size in pixels [int]',
                        default=16,
                        required=False)
    parser.add_argument('-n', '--name',
                        type=str,
                        help='Custom font name [str]',
                        required=False)
    parser.add_argument('-c', '--charset',
                        type=str,
                        help='Custom charset from file [filename]',
                        required=False)
    args = parser.parse_args()

    if args.charset:
        with open(args.charset) as f:
            CHAR_SET = f.read().splitlines()[0]

    # create font type
    font_type = args.font
    font_height = args.size

    myfont = ImageFont.truetype(font_type, size=font_height)
    #myfont = ImageFont.load(font_type)
    font_width = get_max_width(myfont)

    if args.name:
        font_name = args.name
    else:
        font_name = myfont.font.family

    # generate the C file data
    font_data = generate_font_data(myfont, font_width, font_height)
    font_data = textwrap.indent(font_data, ' ' * 4)

    # output everything
    output_files(font=myfont,
                 font_width=font_width,
                 font_height=font_height,
                 font_data=font_data,
                 font_name=font_name,
                 char_set=CHAR_SET.replace('\\','\\\\').replace('"','\\"')
                 )
