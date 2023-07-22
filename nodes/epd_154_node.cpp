#include "epd_154_node.h"

using namespace std;
using namespace display_demo;

void epd154::init(){
    epd.spi = in_spi;
	epd.reset_pin = in_reset;
	epd.dc_pin = in_dc;
	epd.cs_pin = in_cs;
	epd.busy_pin = in_busy;
    
    epd.LDirInit();
    epd.Clear();
    epd.SetupPartial();
}

void epd154::tick(){
    if(in_busy->get() == false){
        epd.SetFrameMemoryPartial((const unsigned char*)in_img.data.data, 0,0, 200,200, param_swap_endian, param_invert_px);
        epd.DisplayPartFrame();        
    }
}