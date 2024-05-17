#ifndef DISPLAY_DEMO_JSON_DISPLAY_H
#define DISPLAY_DEMO_JSON_DISPLAY_H DISPLAY_DEMO_JSON_DISPLAY_H

#include <cstdint>
#include <string>
#include <map>
#include <abmt/json.h>
#include <abmt/img.h>
#include "box.h"

namespace display_demo{

struct img_area{
    int x;
    int y;
    int w;
    int h;
};

struct icon{
    int w;
    int h;
    const unsigned char* ptr;
};


extern std::map<std::string, const abmt::font*> default_fonts;
extern std::map<std::string, const abmt::font*> font_un8;
extern std::map<std::string, const abmt::font*> font_un16;
extern std::map<std::string, const abmt::font*> font_un24;
extern std::map<std::string, const abmt::font*> font_mcr;
extern std::map<std::string, const abmt::font*> font_thin;
extern std::map<std::string, const abmt::font*> font_fantasy;

extern std::map<std::string, icon> default_icons; // 16px
extern std::map<std::string, icon> default_icons_24;


//@node: auto
struct json_display{
    abmt::json in = { {"t", {{"type", "text"}, {"txt", "test"}} } };
    abmt::img_bw out;
    bool out_updated = true;
    std::map<std::string, const abmt::font*> in_fonts = display_demo::default_fonts;
    std::map<std::string, icon> in_icons = display_demo::default_icons;
    
    abmt::json last_displayd;
    
    int param_width  = 320;
    int param_height = 240;
    img_area root_box;
    
    void draw_bdr(img_area b, uint8_t size, uint8_t radius);
    void display_array(abmt::json& o, img_area b);
    void display_box(abmt::json& o, img_area b);
    void display_icon(abmt::json& o, img_area b);
    void display_text(abmt::json& o, img_area b);
    void display_line(abmt::json& o, img_area b);
    void display_hline(abmt::json& o, img_area b);
    void display_vline(abmt::json& o, img_area b);
    
    void tick();
    void init();
    
};


} // namespace display_demo

#endif // DISPLAY_DEMO_JSON_DISPLAY_H
