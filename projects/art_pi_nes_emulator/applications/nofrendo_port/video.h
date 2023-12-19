#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <vid_drv.h>
#include <nes.h>

#define  DEFAULT_WIDTH           256
#define  DEFAULT_HEIGHT          NES_VISIBLE_HEIGHT
#define  DEFAULT_PALETTE_SIZE    256

viddriver_t * video_get_driver();

#endif