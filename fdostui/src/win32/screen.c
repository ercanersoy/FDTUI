/*
 SCREEN.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "screen.h"

/*! \cond */
#if defined(__DMC__) || defined(__BORLANDC__)
/* Console font information */
typedef struct _CONSOLE_FONT_INFO {
    DWORD   nFont;
    COORD   dwFontSize;
} CONSOLE_FONT_INFO;
typedef CONSOLE_FONT_INFO   *PCONSOLE_FONT_INFO;
#endif

BOOL WINAPI GetCurrentConsoleFont(HANDLE, BOOL, PCONSOLE_FONT_INFO);
/*! \endcond */

HANDLE                                  _screen_active= INVALID_HANDLE_VALUE;
HANDLE                                  _screen_stdin= INVALID_HANDLE_VALUE;
static DWORD                            _screen_stdin_restore_mode= 0;
static HANDLE                           _screen_stdout_restore= INVALID_HANDLE_VALUE;
static HANDLE                           _screen_page0= INVALID_HANDLE_VALUE;
static HANDLE                           _screen_page1= INVALID_HANDLE_VALUE;
static int                              _screen_pushed= 0;
unsigned int                            _video_cols= 80;
unsigned int                            _video_rows= 25;

int
screen_init()
{
    int                                 l_exit;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;

    l_exit= 0;

    do
    {

        _screen_stdout_restore= GetStdHandle(STD_OUTPUT_HANDLE);
    
        _screen_page0= CreateConsoleScreenBuffer(
            (GENERIC_READ | GENERIC_WRITE),
            (FILE_SHARE_READ | FILE_SHARE_WRITE),
            0,
            CONSOLE_TEXTMODE_BUFFER,
            0);

        if (INVALID_HANDLE_VALUE == _screen_page0)
        {
            l_exit= -1;
            break;
        }
    
        SetConsoleActiveScreenBuffer(_screen_page0);
        GetConsoleScreenBufferInfo(_screen_page0, &l_info);
    
        _screen_active= _screen_page0;
        _video_cols= l_info.dwSize.X;
        _video_rows= l_info.dwSize.Y;

        _screen_page1= CreateConsoleScreenBuffer(
            (GENERIC_READ | GENERIC_WRITE),
            (FILE_SHARE_READ | FILE_SHARE_WRITE),
            0,
            CONSOLE_TEXTMODE_BUFFER,
            0);

        if (INVALID_HANDLE_VALUE == _screen_page1)
        {
            l_exit= -1;
            break;
        }
    
        _screen_stdin= GetStdHandle(STD_INPUT_HANDLE);

        if (INVALID_HANDLE_VALUE == _screen_stdin)
        {
            l_exit= -1;
            break;
        }

        GetConsoleMode(_screen_stdin, &_screen_stdin_restore_mode);
        SetConsoleMode(_screen_stdin, (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT));

    }while(0);

    return l_exit;
}

void
screen_deinit()
{

    SetConsoleMode(_screen_stdin, _screen_stdin_restore_mode);
    SetConsoleActiveScreenBuffer(_screen_stdout_restore);
    CloseHandle(_screen_page0);
    CloseHandle(_screen_page1);

    return;
}

static void
screen_copy(
    HANDLE                              io_target,
    HANDLE                              i_source)
{
    CHAR_INFO*                          l_buffer;
    COORD                               l_buffer_coord;
    COORD                               l_buffer_size;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;
    SMALL_RECT                          l_region;
    SHORT                               l_row;
    size_t                              l_size;

    GetConsoleScreenBufferInfo(i_source, &l_info);

    l_size= l_info.dwSize.X * sizeof(CHAR_INFO);
    l_buffer= (CHAR_INFO*)malloc(l_size);

    l_buffer_size.X= l_info.dwSize.X;
    l_buffer_size.Y= 1;
    l_buffer_coord.X= 0;
    l_buffer_coord.Y= 0;

    for (l_row= 0; l_info.dwSize.Y > l_row; l_row++)
    {
        l_region.Top= l_row;
        l_region.Left= 0;
        l_region.Right= l_info.dwSize.X - 1;
        l_region.Bottom= l_row;
    
        ReadConsoleOutput(
            i_source,
            l_buffer,
            l_buffer_size,
            l_buffer_coord,
            &l_region);
    
        WriteConsoleOutput(
            io_target,
            l_buffer,
            l_buffer_size,
            l_buffer_coord,
            &l_region);
    }

    free(l_buffer);

    return;
}

extern void
screen_push(
    int const                           i_copy)
{
    CONSOLE_CURSOR_INFO                 l_cursor;
    CONSOLE_FONT_INFO                   l_font_page0;
    CONSOLE_FONT_INFO                   l_font_page1;
    HANDLE                              l_scract;
    HANDLE                              l_scroff;
    HANDLE                              l_scrnew;

    do
    {

        if (_screen_pushed)
        {
            _screen_pushed++;
            break;
        }

        GetCurrentConsoleFont(_screen_page0, 0, &l_font_page0);
        GetCurrentConsoleFont(_screen_page1, 0, &l_font_page1);

        if (l_font_page0.nFont != l_font_page1.nFont ||
            l_font_page0.dwFontSize.X != l_font_page1.dwFontSize.X ||
            l_font_page0.dwFontSize.Y != l_font_page1.dwFontSize.Y)
        {
            l_scrnew= CreateConsoleScreenBuffer(
                (GENERIC_READ | GENERIC_WRITE),
                (FILE_SHARE_READ | FILE_SHARE_WRITE),
                0,
                CONSOLE_TEXTMODE_BUFFER,
                0);

            if (INVALID_HANDLE_VALUE != l_scrnew)
            {
                if (_screen_active == _screen_page0)
                {
                    CloseHandle(_screen_page1);
                    _screen_page1= l_scrnew;
                }
                else
                {
                    CloseHandle(_screen_page0);
                    _screen_page0= l_scrnew;
                }
                GetConsoleCursorInfo(_screen_active, &l_cursor);
                SetConsoleCursorInfo(l_scrnew, &l_cursor);
            }
        }

        if (_screen_active == _screen_page0)
        {
            l_scract= _screen_page0;
            l_scroff= _screen_page1;
        }
        else
        {
            l_scract= _screen_page1;
            l_scroff= _screen_page0;
        }

        _screen_active= l_scroff;
        _screen_pushed= 1;

        if (i_copy)
        {
            screen_copy(l_scroff, l_scract);
        }

    }while(0);

    return;
}

extern void
screen_pop()
{
    CONSOLE_CURSOR_INFO                 l_cursor;
    HANDLE                              l_screen;

    do
    {

        if (0 == _screen_pushed)
        {
            break;
        }

        _screen_pushed--;
        
        if (_screen_pushed)
        {
            break;
        }

        if (_screen_active == _screen_page0)
        {
            l_screen= _screen_page1;
        }
        else
        {
            l_screen= _screen_page0;
        }

        GetConsoleCursorInfo(l_screen, &l_cursor);
        SetConsoleCursorInfo(_screen_active, &l_cursor);
        SetConsoleActiveScreenBuffer(_screen_active);

    }while(0);

    return;
}

static int
screen_setlines(
    unsigned int const                  i_rows)
{
    COORD                               l_coord;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;
    SMALL_RECT                          l_rect;
    HANDLE                              l_scroff;

    l_coord.X= 80;
    l_coord.Y= i_rows;
    SetConsoleScreenBufferSize(_screen_active, l_coord);

    l_rect.Left= 0;
    l_rect.Top= 0;
    l_rect.Right= 79;
    l_rect.Bottom= i_rows - 1;
    SetConsoleWindowInfo(_screen_active, TRUE, &l_rect);

    GetConsoleScreenBufferInfo(_screen_active, &l_info);
    
    _video_cols= l_info.dwSize.X;
    _video_rows= l_info.dwSize.Y;

    if (_screen_active == _screen_page0)
    {
        l_scroff= _screen_page1;
    }
    else
    {
        l_scroff= _screen_page0;
    }

    l_coord.X= l_info.dwSize.X;
    l_coord.Y= l_info.dwSize.Y;
    SetConsoleScreenBufferSize(l_scroff, l_coord);

    l_rect.Left= 0;
    l_rect.Top= 0;
    l_rect.Right= l_info.dwSize.X - 1;
    l_rect.Bottom= l_info.dwSize.Y - 1;
    SetConsoleWindowInfo(l_scroff, TRUE, &l_rect);

    return 0;
}

int
screen_set25lines()
{
    return screen_setlines(25);
}

int
screen_set50lines()
{
    return screen_setlines(50);
}

extern void
screen_changed_size()
{
    COORD                               l_coord;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;
    SMALL_RECT                          l_rect;
    HANDLE                              l_scroff;

    GetConsoleScreenBufferInfo(_screen_active, &l_info);
    
    _video_cols= l_info.dwSize.X;
    _video_rows= l_info.dwSize.Y;

    if (_screen_active == _screen_page0)
    {
        l_scroff= _screen_page1;
    }
    else
    {
        l_scroff= _screen_page0;
    }

    l_coord.X= l_info.dwSize.X;
    l_coord.Y= l_info.dwSize.Y;
    SetConsoleScreenBufferSize(l_scroff, l_coord);

    l_rect.Left= 0;
    l_rect.Top= 0;
    l_rect.Right= l_info.dwSize.X - 1;
    l_rect.Bottom= l_info.dwSize.Y - 1;
    SetConsoleWindowInfo(l_scroff, TRUE, &l_rect);

    return;
}

extern void 
screen_fill(
    screen_char_t const                 i_char,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    DWORD                               l_bytes;
    unsigned char                       l_char;
    COORD                               l_coord;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;
    DWORD                               l_written;

    GetConsoleScreenBufferInfo(_screen_active, &l_info);

    l_bytes= (l_info.dwSize.X * l_info.dwSize.Y);
    l_char= (i_char) ? i_char : 0x20;
    l_coord.X= 0;
    l_coord.Y= 0;

    FillConsoleOutputCharacter(
        _screen_active,
        l_char,
        l_bytes,
        l_coord,
        &l_written);

    FillConsoleOutputAttribute(
        _screen_active,
        (i_fcolor|i_bcolor),
        l_bytes,
        l_coord,
        &l_written);

    return;
}

extern void 
screen_fill_area(
    int const                           i_col,
    int const                           i_row,
    unsigned int const                  i_columns,
    unsigned int const                  i_rows,
    screen_char_t const                 i_char,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    int                                 l_col;
    unsigned int                        l_columns;
    unsigned int                        l_count;
    COORD                               l_coord;
    unsigned char                       l_char;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;
    int                                 l_row;
    unsigned int                        l_rows;
    int                                 l_scratch;
    DWORD                               l_written;

    l_col= i_col;
    l_columns= i_columns;
    l_char= (i_char) ? i_char : 0x20;
    GetConsoleScreenBufferInfo(_screen_active, &l_info);
    l_row= i_row;
    l_rows= i_rows;

    do
    {

        if (0 == i_rows)
        {
            break;
        }

        if (0 > i_col)
        {
            l_scratch= l_columns - (0 - i_col);
            if (0 >= l_scratch)
            {
                break;
            }
            l_columns= l_scratch;
            l_col= 0;
        }

        if ((SHORT)(l_col + l_columns) > l_info.dwSize.X)
        {
            l_columns= l_info.dwSize.X - l_col;
        }

        if (0 == l_columns)
        {
            break;
        }

        if (0 > l_row)
        {
            l_scratch= l_rows - (0 - l_row);
            if (0 >= l_scratch)
            {
                break;
            }
            l_rows= l_scratch;
            l_row= 0;
        }

        l_coord.X= l_col;
        l_coord.Y= l_row;
        l_count= 0;

        do
        {

            if (l_rows <= l_count)
            {
                break;
            }

            if (l_info.dwSize.Y <= l_coord.Y)
            {
                break;
            }

            FillConsoleOutputCharacter(
                _screen_active,
                l_char,
                l_columns,
                l_coord,
                &l_written);
        
            FillConsoleOutputAttribute(
                _screen_active,
                (i_fcolor|i_bcolor),
                l_columns,
                l_coord,
                &l_written);

            l_coord.Y++;
            l_count++;

        }while(1);


    }while(0);

    return;
}

extern void 
screen_putc(
    int const                           i_col,
    int const                           i_row,
    screen_char_t const                 i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    int                                 l_col;
    unsigned int                        l_columns;
    COORD                               l_coord;
    int                                 l_count;
    unsigned char                       l_char;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;
    DWORD                               l_written;

    l_col= i_col;
    l_columns= i_repeat_count;
    l_char= (i_char) ? i_char : 0x20;
    GetConsoleScreenBufferInfo(_screen_active, &l_info);

    do
    {

        if (0 == l_columns)
        {
            break;
        }

        if (0 > i_row)
        {
            break;
        }

        if (l_info.dwSize.Y <= i_row)
        {
            break;
        }

        if (l_info.dwSize.X <= i_col)
        {
            break;
        }

        if (0 > l_col)
        {
            l_count= l_columns - (0 - l_col);
            if (0 >= l_count)
            {
                break;
            }
            l_columns= l_count;
            l_col= 0;
        }
        
        if (l_info.dwSize.X <= (SHORT)(l_col + l_columns))
        {
            l_columns= l_info.dwSize.X - l_col;
        }

        l_coord.X= l_col;
        l_coord.Y= i_row;

        FillConsoleOutputCharacter(
            _screen_active,
            l_char,
            l_columns,
            l_coord,
            &l_written);
    
        FillConsoleOutputAttribute(
            _screen_active,
            (i_fcolor|i_bcolor),
            l_columns,
            l_coord,
            &l_written);

    }while(0);

    return;
}

extern void 
screen_putvc(
    int const                           i_col,
    int const                           i_row,
    screen_char_t const                 i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    COORD                               l_coord;
    int                                 l_count;
    unsigned char                       l_char;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;
    int                                 l_row;
    DWORD                               l_written;

    l_row= i_row;
    l_count= i_repeat_count;
    l_char= (i_char) ? i_char : 0x20;
    GetConsoleScreenBufferInfo(_screen_active, &l_info);

    do
    {

        if (0 == l_count)
        {
            break;
        }

        if (0 > l_row)
        {
            l_count= i_repeat_count - (0 - i_row);
            if (0 >= l_count)
            {
                break;
            }
            l_row= 0;
        }

        if (l_info.dwSize.X <= i_col)
        {
            break;
        }

        if (0 > i_col)
        {
            break;
        }

        l_coord.X= i_col;
        l_coord.Y= l_row;

        do
        {

            if (l_info.dwSize.Y <= l_coord.Y)
            {
                break;
            }

            FillConsoleOutputCharacter(
                _screen_active,
                l_char,
                1,
                l_coord,
                &l_written);
        
            FillConsoleOutputAttribute(
                _screen_active,
                (i_fcolor|i_bcolor),
                1,
                l_coord,
                &l_written);

            l_coord.Y++;
            l_count--;

            if (0 >= l_count)
            {
                break;
            }

        }while(1);

    }while(0);

    return;
}

extern void 
screen_puts(
    int const                           i_col,
    int const                           i_row,
    unsigned char const*                i_string,
    unsigned int const                  i_length,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    int                                 l_col;
    COORD                               l_coord;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;
    unsigned int                        l_length;
    unsigned char const*                l_ptr;
    DWORD                               l_written;

    l_col= i_col;
    l_length= i_length;
    GetConsoleScreenBufferInfo(_screen_active, &l_info);
    l_ptr= i_string;

    do
    {

        if (0 == i_length)
        {
            break;
        }

        if (0 > i_row)
        {
            break;
        }

        if (l_info.dwSize.Y <= i_row)
        {
            break;
        }

        if (l_info.dwSize.X <= i_col)
        {
            break;
        }

        if (0 > l_col)
        {
            do
            {
                l_ptr++;
                l_length--;
                l_col++;

                if (0 == l_length)
                {
                    break;
                }

                if (0 == l_col)
                {
                    break;
                }

            }while(1);
        }

        if (0 == l_length)
        {
            break;
        }

        if (l_info.dwSize.X <= (SHORT)(l_col+l_length))
        {
            l_length= l_info.dwSize.X - l_col;
        }

        l_coord.X= l_col;
        l_coord.Y= i_row;

        WriteConsoleOutputCharacterA(
            _screen_active,
            (LPCSTR)l_ptr,
            l_length,
            l_coord,
            &l_written);

        FillConsoleOutputAttribute(
            _screen_active,
            (i_fcolor|i_bcolor),
            l_length,
            l_coord,
            &l_written);

    }while(0);

    return;
}

extern void 
screen_set_color(
    int const                           i_col,
    int const                           i_row,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    int                                 l_col;
    unsigned int                        l_columns;
    COORD                               l_coord;
    int                                 l_count;
    CONSOLE_SCREEN_BUFFER_INFO          l_info;
    DWORD                               l_written;

    l_col= i_col;
    l_columns= i_repeat_count;
    GetConsoleScreenBufferInfo(_screen_active, &l_info);

    do
    {

        if (0 == l_columns)
        {
            break;
        }

        if (0 > i_row)
        {
            break;
        }

        if (l_info.dwSize.Y <= i_row)
        {
            break;
        }

        if (l_info.dwSize.X <= i_col)
        {
            break;
        }

        if (0 > i_col)
        {
            l_count= i_repeat_count - (0 - i_col);
            if (0 >= l_count)
            {
                break;
            }
            l_columns= l_count;
            l_col= 0;
        }

        l_coord.X= l_col;
        l_coord.Y= i_row;

        FillConsoleOutputAttribute(
            _screen_active,
            (i_fcolor|i_bcolor),
            l_columns,
            l_coord,
            &l_written);


    }while(0);

    return;
}

extern screen_block_t
screen_getc(
    int const                           i_col,
    int const                           i_row)
{
    CHAR_INFO                           l_info;
    COORD                               l_bufsize;
    COORD                               l_bufcoord;
    SMALL_RECT                          l_region;

    l_bufsize.X= 1;
    l_bufsize.Y= 1;
    l_bufcoord.X= 0;
    l_bufcoord.Y= 0;
    l_region.Left= i_col;
    l_region.Top= i_row;
    l_region.Right= i_col;
    l_region.Bottom= i_row;

    ReadConsoleOutput(
        _screen_active,
        &l_info,
        l_bufsize,
        l_bufcoord,
        &l_region);

    return l_info;
}

extern screen_block_t __FAR*
screen_read(
    screen_block_t __FAR*               o_block,
    unsigned int const                  i_columns,
    int const                           i_col,
    int const                           i_row)
{
    COORD                               l_bufsize;
    COORD                               l_bufcoord;
    SMALL_RECT                          l_region;

    l_bufsize.X= i_columns;
    l_bufsize.Y= 1;
    l_bufcoord.X= 0;
    l_bufcoord.Y= 0;
    l_region.Left= i_col;
    l_region.Top= i_row;
    l_region.Right= i_col+i_columns;
    l_region.Bottom= i_row;

    ReadConsoleOutput(
        _screen_active,
        o_block,
        l_bufsize,
        l_bufcoord,
        &l_region);

    return &o_block[i_columns];
}

extern screen_block_t const __FAR*
screen_write(
    int const                           i_col,
    int const                           i_row,
    screen_block_t const __FAR*         i_block,
    unsigned int const                  i_columns)
{
    COORD                               l_bufsize;
    COORD                               l_bufcoord;
    SMALL_RECT                          l_region;

    l_bufsize.X= i_columns;
    l_bufsize.Y= 1;
    l_bufcoord.X= 0;
    l_bufcoord.Y= 0;
    l_region.Left= i_col;
    l_region.Top= i_row;
    l_region.Right= i_col+i_columns;
    l_region.Bottom= i_row;

    WriteConsoleOutput(
        _screen_active,
        i_block,
        l_bufsize,
        l_bufcoord,
        &l_region);

    return &i_block[i_columns];
}
