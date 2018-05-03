/*
 INI.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __ini_h__

#include "hash.h"

#define INI_KEY_MAX_LEN                 128
#define INI_VALUE_MAX_LEN               4096
#define INI_SECTION_BUCKETS             100
#define INI_TUPLE_BUCKETS               50

enum ini_options
{
    INI_OPTION_NONE=                    (0<<1)
};

struct ini_tuple
{
    char *                              m_name;
    char *                              m_value;
};

struct ini_section
{
    char *                              m_name;
    struct hash                         m_hash;
};

struct ini
{
    struct hash                         m_hash;
};

#ifdef __cplusplus
extern "C" {
#endif

extern int
ini_assign(
    struct ini*const                    o_ini);

extern void
ini_discharge(
    struct ini*const                    io_ini);

extern char const *
ini_lookup(
    int*const                           o_found,
    struct ini *const                   i_ini,
    char const *                        i_section,
    char const *                        i_key);

extern int
ini_parse(
    struct ini*const                    io_ini,
    char const*                         i_path,
    enum ini_options const              i_options);

#ifdef __cplusplus
}
#endif

#define __ini_h__
#endif
