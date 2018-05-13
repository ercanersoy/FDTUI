/*
 FDTUICFG.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __fdtuicfg_h__

#ifdef __cplusplus
extern "C" {
#endif

extern void
fdtuicfg_load();

extern void
fdtuicfg_load_file(
    char const*                         i_path);

#ifdef __cplusplus
}
#endif

#define __fdtuicfg_h__
#endif
