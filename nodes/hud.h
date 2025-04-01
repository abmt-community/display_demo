#ifndef DISPLAY_DEMO_HUD_H
#define DISPLAY_DEMO_HUD_H DISPLAY_DEMO_HUD_H

#include <abmt/img.h>

namespace display_demo{

//@node: auto
//@raster: auto
struct hud{
    abmt::img_bw in_hud;
    abmt::img_rgb in_img;
    abmt::img_rgb out;
    
    
    abmt::pixel_rgb param_color = {255,255,255};
    int param_offset_x = 0;
    int param_offset_y = 0;
    void tick();
};




} // namespace display_demo

#endif // DISPLAY_DEMO_HUD_H
