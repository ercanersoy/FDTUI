/*
 STRINGS.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __strings_h__

#ifdef __cplusplus
extern "C" {
#endif

#define TEXT_SIZE_CANCEL                16
#define TEXT_SIZE_DRIVES                16
#define TEXT_SIZE_ENTER_VALUE           32
#define TEXT_SIZE_FILES                 16
#define TEXT_SIZE_FILTER                16
#define TEXT_SIZE_HIDDEN                16
#define TEXT_SIZE_NAME                  16
#define TEXT_SIZE_OK                    16
#define TEXT_SIZE_OPEN_FILE             24
#define TEXT_SIZE_READ_ONLY             24
#define TEXT_SIZE_REFRESH               16
#define TEXT_SIZE_SYSTEM                16
#define TEXT_SIZE_UNABLE_TO_CHANGE_DIRECTORY 64


extern unsigned char _text_cancel[TEXT_SIZE_CANCEL];
extern unsigned char _text_drives[TEXT_SIZE_DRIVES];
extern unsigned char _text_enter_value[TEXT_SIZE_ENTER_VALUE];
extern unsigned char _text_files[TEXT_SIZE_FILES];
extern unsigned char _text_filter[TEXT_SIZE_FILTER];
extern unsigned char _text_hidden[TEXT_SIZE_HIDDEN];
extern unsigned char _text_name[TEXT_SIZE_NAME];
extern unsigned char _text_ok[TEXT_SIZE_OK];
extern unsigned char _text_open_file[TEXT_SIZE_OPEN_FILE];
extern unsigned char _text_read_only[TEXT_SIZE_READ_ONLY];
extern unsigned char _text_refresh[TEXT_SIZE_REFRESH];
extern unsigned char _text_system[TEXT_SIZE_SYSTEM];
extern unsigned char _text_unable_to_change_directory[TEXT_SIZE_UNABLE_TO_CHANGE_DIRECTORY];

#ifdef __cplusplus
}
#endif

#define __strings_h__
#endif
