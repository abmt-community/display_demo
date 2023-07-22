/**
 * Author: Hendrik van Arragon, 2023
 * SPDX-License-Identifier: MIT
 */
 
#ifndef DISPLAY_DEMO_DISPLAY_H
#define DISPLAY_DEMO_DISPLAY_H DISPLAY_DEMO_DISPLAY_H

#include <abmt/img.h>
#include "box.h"


namespace display_demo{


struct compile_display { }; //@node: auto

struct display{
    int width = 200;
    int height = 200;
    int th = 30; // top height
    int bh = 30; // bottom height
    int line_size = 1;
    int battery = 0;
    int out = 0;
    int interval = 0;
    
    bool display_play = true;
    
    abmt::img_bw img = {width,height};
    
    std::string status_text = "";
    
    enum {
        BACK, SET_OUT, SET_INT, RESET,
        INP_OUT, INP_INT
    } selected_menu_element;
    
    void draw_frame();
    void screen_text(std::string text);
    void screen_icon();
    void screen_menu();
    void screen_test_font();
    void draw_box_text(std::string txt, int x, int y, bool border = true);
    void box(int x, int y, int width, int height);
};

} // namespace display_demo

#endif // DISPLAY_DEMO_DISPLAY_H
