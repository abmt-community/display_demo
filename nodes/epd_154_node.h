#ifndef DISPLAY_DEMO_EPD_154_NODE_H
#define DISPLAY_DEMO_EPD_154_NODE_H DISPLAY_DEMO_EPD_154_NODE_H


#include <abmt/time.h>
#include <abmt/io/spi.h>
#include <abmt/io/pin.h>
#include <abmt/img.h>

#include "epd1in54_V2.h"

//@compile: epd1in54_V2.cpp

namespace display_demo{


//@node:auto
struct epd154{
    
    abmt::img_bw in_img;
    abmt::io::spi_ptr in_spi;
    abmt::io::opin_ptr in_cs;
    abmt::io::opin_ptr in_dc;
    abmt::io::opin_ptr in_reset;
    abmt::io::ipin_ptr in_busy;
    
    bool param_swap_endian = true;
    bool param_invert_px = false;

    
    Epd epd;
    void init();
    void tick();
};


} // namespace display_demo

#endif // DISPLAY_DEMO_EPD_154_NODE_H
