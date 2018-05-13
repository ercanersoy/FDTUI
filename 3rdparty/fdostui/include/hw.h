/*
 HW.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#ifndef __hw_h__

#if defined(__DOS__) || defined(__NT__) || defined(__DJGPP__)

#ifdef __cplusplus
extern "C" {
#endif

#define HW_DRIVE_MAX                    26

extern unsigned char                    _hw_drives[HW_DRIVE_MAX];

extern void
hw_detect_drives();

#ifdef __cplusplus
}
#endif

#endif /* #if defined(__DOS__) || defined(__NT__) */

#define __hw_h__
#endif
