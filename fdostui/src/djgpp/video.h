/*
  VIDEO.H
 
  Adapted from the work of Jerome Shidel INFO.PAS.
 
  Copyright 1990-2015, Jerome Shidel.
    
  This project and related files are subject to the terms of the Mozilla 
  Public License,  v. 2.0. If a copy of the MPL was not distributed with 
  this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef __video_h__

enum video_card
{
    VIDEO_CARD_VESA                     = 9,
    VIDEO_CARD_VGA                      = 8,
    VIDEO_CARD_MCGA                     = 7,
    VIDEO_CARD_EGA                      = 6,
    VIDEO_CARD_CGA                      = 5,
    VIDEO_CARD_INCOLOR                  = 4,
    VIDEO_CARD_HGCPLUS                  = 3,
    VIDEO_CARD_HGC                      = 2,
    VIDEO_CARD_MDA                      = 1,
    VIDEO_CARD_NONE                     = 0
};

#ifdef __cplusplus
extern "C" {
#endif

extern enum video_card
video_detect();

#ifdef __cplusplus
}
#endif

#define __video_h__
#endif
