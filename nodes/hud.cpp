#include "hud.h"
#include <algorithm>
using namespace std;
using namespace display_demo;

void hud::tick(){
    out = in_img.copy();
    int x_end = std::min(in_hud.width, in_img.width - param_offset_x);
    int y_end = std::min(in_hud.height, in_img.height - param_offset_y);
    for(int x = 0; x < x_end; x++){
        for(int y = 0; y < y_end; y++){
            if(in_hud.at_2(x, y)){
                out.at_2(x + param_offset_x, y+param_offset_y) = param_color;
            }
        }
    }
}


