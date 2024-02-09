/**
 * Author: Hendrik van Arragon, 2023
 * SPDX-License-Identifier: MIT
 */
 
#include "display.h"

#include "iconoir_24.h"
#include "iconoir_96.h"
#include "asian_test_font.h"
#include "font_unscii_8.h"
#include "font_unscii_16.h"
#include "font_unscii_24.h"
#include "font_unscii_fantasy_8.h"
#include "font_unscii_mcr_8.h"
#include "font_unscii_thin_8.h"

using namespace std;
using namespace display_demo;

void display::draw_frame(){
    img.fill(false); // clear
    
    abmt::img_bw bat = {24, 24, icon_battery_empty_24};
    if(battery > 75){
        bat = {24, 24, icon_battery_full_24};
    } else if(battery > 50){
        bat = {24, 24, icon_battery_75_24};
    } else if(battery > 25){
        bat = {24, 24, icon_battery_50_24};
    }else if(battery > 10){
        bat = {24, 24, icon_battery_25_24};
    } 
    
    img.draw_icon(bat, width - 30, 1);
    
    img.write_text(status_text, 3, 6, true, 2, font_unscii_mcr_8_font);
    
    img.draw_line(0, th, width, th, true, line_size);
    
    img.draw_line(0, height - bh, width, height - bh, true, line_size);
    auto seg_size = width / 3;
    img.draw_line(seg_size, height - bh, seg_size, height, true, line_size);
    img.draw_line(seg_size*2, height - bh, seg_size*2, height, true, line_size);
    img.draw_icon({24, 24, icon_skip_prev_24}, seg_size/2 - 12 , height - bh + 3);
    img.draw_icon({24, 24, icon_skip_next_24}, seg_size/2 - 12 + seg_size * 2, height - bh + 3);
    
    if(display_play){
        img.draw_icon({24, 24, icon_play_24}, seg_size/2 - 12 + seg_size , height - bh + 3);
    }
    
}

void display::screen_text(std::string text){
    draw_frame();
    img.write_text(text, 5, th + 3);
}

void display::screen_icon(){
   abmt::img_bw icon =  {96, 96, icon_media_image_96};
    draw_frame();
    img.draw_icon(icon, width/2 - icon.width/2,  th + (height-th*2)/2 - icon.height/2);

}

void display::screen_menu(){
    draw_frame();
    int line_hight = 18;
    img.write_text("menu example", 5, th + 3);
    
    draw_box_text("Back",           5, th + line_hight * 1 + 5, selected_menu_element == BACK);
    draw_box_text("Set output:",    5, th + line_hight * 2 + 5, selected_menu_element == SET_OUT);
    draw_box_text("Set interval:",  5, th + line_hight * 3 + 5, selected_menu_element == SET_INT);
    draw_box_text("Factory reset",  5, th + line_hight * 4 + 5, selected_menu_element == RESET);

    draw_box_text(std::to_string(out),   128, th + line_hight * 2 + 5, selected_menu_element == INP_OUT);
    draw_box_text(std::to_string(interval), 128, th + line_hight * 3 + 5, selected_menu_element == INP_INT);
}

void display::draw_box_text(std::string txt, int x, int y, bool border){
    int padding = 4;
    abmt::font font = abmt::default_font;
    img.write_text(txt, x + padding, y  + padding/2);
    if(border){
        box(x - 1, y - 1, txt.length()*(font.width + font.space) + 2*padding, font.height + padding);
    }
}

void display::screen_test_font(){
    draw_frame();
    img.write_text("侘寂", 5, th + 25, true, 1, asian_test_font);
}

void display::box(int x, int y, int width, int height){
    abmt::img_bw corner_tl = {4, 4, border_round_4};
    abmt::img_bw corner_tr = corner_tl.rotate_90();
    abmt::img_bw corner_br = corner_tr.rotate_90();
    abmt::img_bw corner_bl = corner_br.rotate_90();
    
    auto cw = corner_tl.width;
    auto ch = corner_tl.height;
    
    img.draw_line(x + cw, y, x + width - cw, y, true, 1);
    img.draw_line(x, y + ch, x, y + height - ch, true, 1);
    img.draw_line(x + cw, y + height, x + width - cw, y + height, true, 1);
    img.draw_line(x + width, y + ch, x + width, y + height - ch, true, 1);
   
    img.draw_icon(corner_tl, x, y);
    img.draw_icon(corner_tr, x + width - cw + 1, y);
    img.draw_icon(corner_bl, x, y + height - ch + 1);
    img.draw_icon(corner_br, x + width - cw + 1, y + height - ch + 1);
}