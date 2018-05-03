/*
 HASH.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#ifndef __hash_h__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bucket
{
    char *                                  m_key;
    void *                                  m_value;
    struct bucket *                         m_next;
};

typedef void 
(*hash_remove_cb)(
    void *                                  io_value);

struct hash
{
    struct bucket **                        m_bucket;
    size_t                                  m_slots;
    hash_remove_cb                          m_remove;
};

extern int
hash_assign(
    struct hash*const                       o_hash,
    size_t const                            i_buckets,
    hash_remove_cb                          i_remove);

extern void
hash_discharge(
    struct hash*const                       io_hash);

extern int
hash_insert(
    struct hash*const                       io_hash,
    char const *                            i_key,
    void *                                  i_value);

extern void *
hash_lookup(
    int *const                              o_found,
    struct hash*const                       io_hash,
    char const *                            i_key);

extern int
hash_remove(
    struct hash*const                       io_hash,
    char const *                            i_key);

#ifdef __cplusplus
}
#endif

#define __hash_h__
#endif
