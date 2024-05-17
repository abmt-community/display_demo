#include "json_display.h"
#include <abmt/util/str_utils.h>
#include "iconoir_24.h"
#include "iconoir_16.h"
#include "font_unscii_8.h"
#include "font_unscii_16.h"
#include "font_unscii_24.h"
#include "font_unscii_fantasy_8.h"
#include "font_unscii_mcr_8.h"
#include "font_unscii_thin_8.h"

using namespace std;
using namespace display_demo;

void from_key(int16_t& var, std::string key, abmt::json& o, img_area b){
    if(o.key_exsists(key)){
        float val = o[key];
        if(o[key].is_string() && o[key].str().find("%") != string::npos){
            if(key[0] == 'x' || key[0] == 'w'){
                val = val*b.w/100 + 0.5;
            }else{
                val = val*b.h/100 + 0.5;
            }
        }
        var = val;
    }
}

void from_key_s(string& var, std::string key, abmt::json& o){
    if(o.key_exsists(key)){
        var = o[key];
    }
}

struct object{
    int16_t x = 0;
    int16_t y = 0;
    int16_t h = 0;
    int16_t m = 0;
    int16_t b = 0;
    int16_t p = 0;
    int16_t r = 0;
    int16_t w = 1;
    int16_t x1 = 0;
    int16_t x2 = 0;
    int16_t y1 = 0;
    int16_t y2 = 0;
    int16_t scale = 1;
    bool disp = true;
    std::string al = "tl";
    std::string txt = "";
    std::string fnt = "default";
    std::string name;
    std::string type;
    
    int16_t b_width = 0;
    
    img_area parrent;
    
    object(abmt::json o, img_area bx){
        parrent = bx;
        from_key(x,"x", o, bx);
        from_key(y,"y", o, bx);
        from_key(w,"w", o, bx);
        from_key(h,"h", o, bx);
        from_key(m,"m", o, bx);
        from_key(b,"b", o, bx);
        from_key(p,"p", o, bx);
        from_key(r,"r", o, bx);
        from_key(w,"w", o, bx);
        from_key(x1,"x1", o, bx);
        from_key(x2,"x2", o, bx);
        from_key(y1,"y1", o, bx);
        from_key(y2,"y2", o, bx);
        from_key(scale,"scale", o, bx);
        
        from_key_s(al,"al", o);
        from_key_s(txt,"txt", o);
        from_key_s(fnt,"fnt", o);
        from_key_s(name,"name", o);
        from_key_s(type,"type", o);
        
        if(o.key_exsists("disp")){
            disp = o["disp"];
        }
        
        b_width = m + b + p;
        
        if(b < 0){
            b = 0;
            m += -b; // adds negative border width to margin
        }
    }

    void align(int16_t w, int16_t h){
        if(al == "tr" || al == "cr" || al == "br"){
            x += parrent.w - w;
        }
        if(al == "tc" || al == "cc" || al == "bc"){
            x += (parrent.w - w)/2;
        }
        if(al == "cl" || al == "cc" || al == "cr"){
            y += (parrent.h - h)/2;
        }
        if(al == "bl" || al == "bc" || al == "br"){
            y += parrent.h - h;
        }
    }
};

void json_display::draw_bdr(img_area b, uint8_t size, uint8_t radius){
    uint16_t cw = 0;
    uint16_t ch = 0;
    uint8_t bo = (size-1)/2 + (size-1)%2; // border offset (img.line is centered)
    
    bool draw_corner_icon = false;
    abmt::img_bw corner_tl;
    abmt::img_bw corner_tr;
    abmt::img_bw corner_br;
    abmt::img_bw corner_bl;
    
    if(radius == 4 && size > 0){
        draw_corner_icon = true;
        if(size > 1){
            corner_tl = {4, 4, border_round_4_full};
        }else{
            corner_tl = {4, 4, border_round_4};
        }
    }
    if(radius == 6 && size > 0){
        draw_corner_icon = true;
        if(size > 1){
            corner_tl = {6, 6, border_round_6_full};
        }else{
            corner_tl = {6, 6, border_round_6};
        }
    }
    if(radius == 8 && size > 0){
        draw_corner_icon = true;
        if(size > 1){
            corner_tl = {8, 8, border_round_8_full};
        }else{
            corner_tl = {8, 8, border_round_8};
        }
    }
    
    if(draw_corner_icon){
        corner_tr = corner_tl.rotate_90();
        corner_br = corner_tr.rotate_90();
        corner_bl = corner_br.rotate_90();
        cw = corner_tl.width;
        ch = corner_tl.height;
    }
    
    out.draw_line(b.x + cw,  b.y + bo, b.x + b.w - cw -1, b.y + bo, true, size); // tl -> tr
    out.draw_line(b.x + bo, b.y + b.h - ch -1, b.x + bo, b.y + ch, true, size); // bl -> tl 
    out.draw_line(b.x + b.w - cw -1, b.y + b.h - bo -1, b.x + cw, b.y + b.h - bo -1, true, size); // br -> bl
    out.draw_line(b.x + b.w - bo -1, b.y + ch, b.x + b.w - bo -1, b.y + b.h - ch -1, true, size); // tr -> br
    
    if(draw_corner_icon){
        out.draw_icon(corner_tl, b.x, b.y);
        out.draw_icon(corner_tr, b.x + b.w - cw , b.y);
        out.draw_icon(corner_bl, b.x, b.y + b.h - ch);
        out.draw_icon(corner_br, b.x + b.w - cw , b.y + b.h - ch);
    }
}

void  json_display::display_array(abmt::json& o, img_area b){
    for(auto itr: o){
        auto& obj = itr.value;
        if(obj.is_object() && obj.key_exsists("type")){
            auto t = obj["type"];
            if(t == "text"){
                display_text(obj, b);
            }else if(t == "line"){
                display_line(obj, b);
            }else if(t == "hline"){
                display_hline(obj, b);
            }else if(t == "vline"){
                display_vline(obj, b);
            }else if(t == "icon"){
                display_icon(obj, b);
            }else if(t == "box"){
                display_box(obj, b);
            }
        }
    }
}

void json_display::display_box(abmt::json& j, img_area bx){
    object o(j, bx);
    if(o.disp == false){
        return;
    }
    o.align(o.w, o.h);
    draw_bdr({bx.x + o.x + o.m, bx.y + o.y + o.m, o.w - o.m*2, o.h - o.m*2}, o.b, o.r);

    // box content
    img_area content_box = bx;
    content_box.x += o.b_width + o.x;
    content_box.y += o.b_width + o.y;
    content_box.w  = o.w - o.b_width*2;
    content_box.h  = o.h - o.b_width*2;
    display_array(j, content_box);
}

void json_display::display_icon(abmt::json& j, img_area bx){
    object o(j, bx);
    if(o.disp == false || o.name == ""){
        return;
    }
    bool found = false;
    icon i;
    if( o.name != "" ){
        for(auto itr: in_icons){
            if(itr.first == o.name){
                i = itr.second;
                found = true;
                break;
            }
        }
    }
    
    if( found ){
        o.align(i.w*o.scale + o.b_width*2, i.h*o.scale + o.b_width*2);
        draw_bdr({ bx.x + o.x + o.m, 
                          bx.y + o.y + o.m, 
                          i.w*o.scale + (o.b + o.p)*2, 
                          i.h*o.scale + (o.b + o.p)*2
                 }, o.b, o.r);
        abmt::img_bw icon_to_draw(i.w, i.h, i.ptr);
        out.draw_icon(icon_to_draw,  bx.x + o.x + o.b_width, bx.y + o.y + o.b_width, true, o.scale);
    }else{
        o.align(abmt::default_font.width + o.b_width*2, abmt::default_font.height + o.b_width*2);
        out.write_text("X", bx.x + o.x, bx.y + o.y, true, 2);
    }
}

void json_display::display_text(abmt::json& j, img_area bx){
    object o(j, bx);
    if(o.disp == false){
        return;
    }
    const abmt::font* font = &abmt::default_font;
    if(o.fnt != "default"){
        for(auto itr: in_fonts){
            if(itr.first == o.fnt){
                font = itr.second;
                break;
            }
        }
    }else if(in_fonts.size() > 0){
        font = in_fonts.begin()->second;
    }
    
    string text = o.txt;
    vector<string> lines;
    size_t end_of_line = 0;
    while((end_of_line = text.find('\n')) != string::npos){
        lines.push_back(text.substr(0, end_of_line));
        text.erase(0, end_of_line + 1);
    }
    lines.push_back(text); // rest
    
    int h = font->height*lines.size() + (lines.size() -1) * font->line_space;
    int w = 0;
    for(auto l:lines){
        uint16_t line_width = l.size()*(font->width + font->space);
        if(line_width > w){
            w = line_width;
        }
    }

    w = w * o.scale;
    h = h * o.scale;

    o.align(w + o.b_width*2, h + o.b_width*2);
    
    draw_bdr({ bx.x + o.x + o.m, 
                      bx.y + o.y + o.m, 
                      w + (o.b + o.p)*2, 
                      h + (o.b + o.p)*2
    }, o.b, o.r);
    
    out.write_text(o.txt, bx.x + o.x + o.b_width, bx.y + o.y + o.b_width, true, o.scale, *font);
}

void json_display::display_line(abmt::json& j, img_area bx){
    object o(j, bx);
    if(o.disp == false){
        return;
    }
    out.draw_line(bx.x + o.x1, bx.y + o.y1, bx.x + o.x2, bx.y + o.y2, true, o.w);
}

void json_display::display_hline(abmt::json& j, img_area bx){
    object o(j, bx);
    if(o.disp == false){
        return;
    }
    out.draw_line(bx.x, bx.y + o.y, bx.x + bx.w -1, bx.y + o.y, true, o.w);
}

void json_display::display_vline(abmt::json& j, img_area bx){
    object o(j, bx);
    out.draw_line(bx.x + o.x, bx.y, bx.x + o.x, bx.y + bx.h -1, true, o.w);
}

void json_display::tick(){
    if(in.is_object() == false){
        return;
    }
    if(in == last_displayd){
        out_updated = false;
        return;
    }
    out_updated = true;
    last_displayd = in;
    out = abmt::img_bw(param_width, param_height);  // create new image / shared pointer
    out.fill(false);
    display_array(in,root_box);
}

void json_display::init(){
    out = abmt::img_bw(param_width, param_height);
    root_box.x = 0;
    root_box.y = 0;
    root_box.w = param_width;
    root_box.h = param_height;
}

std::map<std::string, const abmt::font*> display_demo::default_fonts = {
    { "default", &abmt::default_font },
    { "un8", &font_unscii_8_font },
    { "un16", &font_unscii_16_font },
    { "un24", &font_unscii_24_font },
    { "fantasy", &font_unscii_fantasy_8_font },
    { "mcr", &font_unscii_mcr_8_font },
    { "thin", &font_unscii_thin_8_font },
};

std::map<std::string, const abmt::font*> display_demo::font_un8 = {
    { "un8", &font_unscii_8_font },
};
std::map<std::string, const abmt::font*> display_demo::font_un16 = {
    { "un16", &font_unscii_16_font },
};
std::map<std::string, const abmt::font*> display_demo::font_un24 ={
    { "un14", &font_unscii_24_font },
};
std::map<std::string, const abmt::font*> display_demo::font_mcr = {
    { "mcr", &font_unscii_mcr_8_font },
};
std::map<std::string, const abmt::font*> display_demo::font_thin = {
    { "thin", &font_unscii_thin_8_font },
};
std::map<std::string, const abmt::font*> display_demo::font_fantasy = {
    { "fantasy", &font_unscii_fantasy_8_font },
};

std::map<std::string, icon> display_demo::default_icons = {
    { "check", {16,16,icon_check_16} },
    { "download", {16,16,icon_download_16} },
    { "battery_empty", {16,16,icon_battery_empty_16} },
    { "battery_25", {16,16,icon_battery_25_16} },
    { "battery_50", {16,16,icon_battery_50_16} },
    { "battery_75", {16,16,icon_battery_75_16} },
    { "battery_full", {16,16,icon_battery_full_16} },
    { "battery_charging", {16,16,icon_battery_charging_16} },
    { "undo", {16,16,icon_undo_16} },
    { "redo", {16,16,icon_redo_16} },
    { "trash", {16,16,icon_trash_16} },
    { "restart", {16,16,icon_restart_16} },
    { "wifi", {16,16,icon_wifi_16} },
    { "wifi_off", {16,16,icon_wifi_off_16} },
    { "code", {16,16,icon_code_16} },
    { "light_bulb", {16,16,icon_light_bulb_16} },
    { "light_bulb_on", {16,16,icon_light_bulb_on_16} },
    { "light_bulb_off", {16,16,icon_light_bulb_off_16} },
    { "emoji", {16,16,icon_emoji_16} },
    { "emoji_blink_right", {16,16,icon_emoji_blink_right_16} },
    { "emoji_sad", {16,16,icon_emoji_sad_16} },
    { "emoji_quite", {16,16,icon_emoji_quite_16} },
    { "emoji_surprise_alt", {16,16,icon_emoji_surprise_alt_16} },
    { "heart", {16,16,icon_heart_16} },
    { "play", {16,16,icon_play_16} },
    { "pause", {16,16,icon_pause_16} },
    { "skip_next", {16,16,icon_skip_next_16} },
    { "skip_prev", {16,16,icon_skip_prev_16} },
    { "repeat", {16,16,icon_repeat_16} },
    { "clock", {16,16,icon_clock_16} },
    { "alarm", {16,16,icon_alarm_16} },
    { "half_moon", {16,16,icon_half_moon_16} },
    { "dashboard_dots", {16,16,icon_dashboard_dots_16} },
    { "half_moon", {16,16,icon_half_moon_16} },
    { "temperature_high", {16,16,icon_temperature_high_16} },
};

std::map<std::string, icon> display_demo::default_icons_24 = {
    { "check", {24,24,icon_check_24} },
    { "download", {24,24,icon_download_24} },
    { "battery_empty", {24,24,icon_battery_empty_24} },
    { "battery_25", {24,24,icon_battery_25_24} },
    { "battery_50", {24,24,icon_battery_50_24} },
    { "battery_75", {24,24,icon_battery_75_24} },
    { "battery_full", {24,24,icon_battery_full_24} },
    { "battery_charging", {24,24,icon_battery_charging_24} },
    { "undo", {24,24,icon_undo_24} },
    { "redo", {24,24,icon_redo_24} },
    { "trash", {24,24,icon_trash_24} },
    { "restart", {24,24,icon_restart_24} },
    { "wifi", {24,24,icon_wifi_24} },
    { "wifi_off", {24,24,icon_wifi_off_24} },
    { "code", {24,24,icon_code_24} },
    { "light_bulb", {24,24,icon_light_bulb_24} },
    { "light_bulb_on", {24,24,icon_light_bulb_on_24} },
    { "light_bulb_off", {24,24,icon_light_bulb_off_24} },
    { "emoji", {24,24,icon_emoji_24} },
    { "emoji_blink_right", {24,24,icon_emoji_blink_right_24} },
    { "emoji_sad", {24,24,icon_emoji_sad_24} },
    { "emoji_quite", {24,24,icon_emoji_quite_24} },
    { "emoji_surprise_alt", {24,24,icon_emoji_surprise_alt_24} },
    { "heart", {24,24,icon_heart_24} },
    { "play", {24,24,icon_play_24} },
    { "pause", {24,24,icon_pause_24} },
    { "skip_next", {24,24,icon_skip_next_24} },
    { "skip_prev", {24,24,icon_skip_prev_24} },
    { "repeat", {24,24,icon_repeat_24} },
    { "clock", {24,24,icon_clock_24} },
    { "alarm", {24,24,icon_alarm_24} },
    { "half_moon", {24,24,icon_half_moon_24} },
    { "dashboard_dots", {24,24,icon_dashboard_dots_24} },
    { "half_moon", {24,24,icon_half_moon_24} },
    { "temperature_high", {24,24,icon_temperature_high_24} },
};