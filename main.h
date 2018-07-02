/*
 *  main.h - Main header file
 *  Written by Ercan Ersoy.
 */

#include <direct.h>
#include <dos.h>
#include <fdostui.hpp>
#include <mouse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "config.h"
#include "lang\en.h"
#include "lang\fr.h"
#include "lang\tr.h"

// Common Variables

// Current directory string pointer variable
char *current_directory = (char *)malloc(MAX_PATH_LENGTH + 1);
// Show hidden file variable
bool show_hidden_file = false;
// Clipboard item name string pointer variable
char *clipboard_item_name = (char *)calloc(256, 1);
// Clipboard item path string pointer variable
char *clipboard_item_path = (char *)calloc(MAX_PATH_LENGTH, 1);
// Clipboard status variable
unsigned char clipboard_status = 0;  // 0 for nothing, 1 for copying, 2 for cutting.
// History path names string array variable
char history_path_names[HISTORY_LENGTH][MAX_PATH_LENGTH + 1];
// History index variable
int history_index = 0;

// Bar

// Window of bar pointer variable
window *bar = new window(0, 0, _video_cols, 3);
// Menubar of bar pointer variable
menubar *bar_menus = new menubar(0, 0, _video_cols, 1);

// File Manager

// Window of file manager pointer variable
window *file_manager = new window(5, 6, 68, 18);
// Menubar of file manager pointer variable
menubar *file_manager_menus = new menubar(0, 0, _video_cols, 1);
// Current directory label of file manager pointer variable
label *current_directory_label = new label(1, 2, 65, 1);
// Current drivers listbox of file manager pointer variable
listbox *drivers = new listbox(0, 3, 66, 5);
// Current directories and files of file manager pointer variable
listbox *directories_and_files = new listbox(0, 8, 66, 8);

// Function prototypes

// Exit window manager function
void exit_window_manager(menuitem *, void *);
// Command run function
void command_run(char const *);
// Copy file function
void copy_file(char *, char *);
// Directory view function
void directory_view(menuitem *, void *);
// Upper directory control function
void upper_directory_control(void);
// Change current directory function
void change_current_directory(char *, unsigned char);
// Change current drive function
void change_current_drive(listbox const *, void *);
// Copy directory function
void copy_directory(char *, char *);
// Remove item function
char remove_item(char *);
// History back function
void history_back(struct menuitem *, void *);
// History foward function
void history_foward(menuitem *, void *);
// Change to upper directory function
void change_to_upper_directory(menuitem *, void *);
// Show file manager function
void show_file_manager(menuitem *, void *);
// Show run function
void show_run(menuitem *, void *);
// Run application EDIT function
void run_application_edit(menuitem *, void *);
// Run application EDLIN function
void run_application_edlin(menuitem *, void *);
// Quit function
void quit(menuitem *, void *);
// Reboot function
void reboot(menuitem *, void *);
// Poweroff function
void poweroff(menuitem *, void *);
// Item open function
void item_open(menuitem *, void *);
// New directory function
void new_directory(menuitem *, void *);
// Exit file manager function
void file_manager_exit(menuitem *, void *);
// Cut function
void cut_item(menuitem *, void *);
// Copy function
void copy_item(menuitem *, void *);
// Paste Function
void paste_item(menuitem *, void *);
// Delete Function
void delete_item(menuitem *, void *);
// Show hidden files function
void show_hidden_files(menuitem *, void *);

// Bar Menus

// Internal applications menu of bar
struct menuitem internal_applications_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_FILE_MANAGER), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, show_file_manager, 0},  // File manager
   {reinterpret_cast<unsigned char const*>(STRING_RUN), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, show_run, 0},  // Run
   {0}
};

// DOS applications menu of bar
struct menuitem dos_applications_menu[] =
{
   {reinterpret_cast<unsigned char const*>("EDIT"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, run_application_edit, 0},  // Edit
   {reinterpret_cast<unsigned char const*>("EDLIN"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, run_application_edlin, 0},  // Edlin
   {0}
};

// Exit menu of bar
struct menuitem exit_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_QUIT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, quit, 0},  // Quit
   {reinterpret_cast<unsigned char const*>(STRING_REBOOT_COMPUTER), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, reboot, 0},  // Reboot Computer
   {reinterpret_cast<unsigned char const*>(STRING_POWEROFF_COMPUTER), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, poweroff, 0},  // Power Off Computer
   {0}
};

// Menubar of bar
struct menuitembar menus_of_bar[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_INTERNAL_APPLICATIONS), 0, SCAN_NONE, internal_applications_menu},  // Internal Applications
   {reinterpret_cast<unsigned char const*>(STRING_DOS_APPLICATIONS), 0, SCAN_NONE, dos_applications_menu},  // Dos Applications
   {reinterpret_cast<unsigned char const*>(STRING_EXIT), 0, SCAN_NONE, exit_menu},  // Exit
   {0}
};

// File Manager Menus

// Fİle menu of file manager
struct menuitem file_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_OPEN), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, item_open, 0},  // Open
   {reinterpret_cast<unsigned char const*>(STRING_NEW_DIRECTORY), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, new_directory, 0},  // New directory
   {reinterpret_cast<unsigned char const*>(STRING_EXIT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, file_manager_exit, 0},  // Exit
   {0}
};

// Edit menu of file manager
struct menuitem edit_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_CUT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, cut_item, 0},  // Cut
   {reinterpret_cast<unsigned char const*>(STRING_COPY), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, copy_item, 0},  // Copy
   {reinterpret_cast<unsigned char const*>(STRING_PASTE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, paste_item, 0},  // Paste
   {reinterpret_cast<unsigned char const*>(STRING_DELETE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, delete_item, 0},  // Delete
   {0}
};

// View menu of file manager
struct menuitem view_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_REFRESH), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, directory_view, 0},  // Refresh
   {reinterpret_cast<unsigned char const*>(STRING_SHOW_HIDDEN_FILES), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX, show_hidden_files, 0},  // Show hidden files
   {0}
};

// Go menu of file manager
struct menuitem go_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_BACK), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_DISABLED, history_back, 0},  // Back
   {reinterpret_cast<unsigned char const*>(STRING_FOWARD), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_DISABLED, history_foward, 0},  // Foward
   {reinterpret_cast<unsigned char const*>(STRING_UP), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_DISABLED, change_to_upper_directory, 0},  // Up
   {0}
};

// Menubar of file manager
struct menuitembar menus_of_file_manager[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_FILE), 0, SCAN_NONE, file_menu},  // File
   {reinterpret_cast<unsigned char const*>(STRING_EDIT), 0, SCAN_NONE, edit_menu},  // Edit
   {reinterpret_cast<unsigned char const*>(STRING_VIEW), 0, SCAN_NONE, view_menu},  // View
   {reinterpret_cast<unsigned char const*>(STRING_GO), 0, SCAN_NONE, go_menu},  // Go
   {0}
};
