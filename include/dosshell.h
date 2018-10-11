/*
 *  dosshell.h - Main header file
 *  Written by Ercan Ersoy.
 */

#include <direct.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fdostui.hpp>
#include <mouse.h>
#include <kitten.h>
#include <config.h>


// Common Variables

// Current directory string pointer variable
static char *current_directory;
// Selected directory or file
static int selected_directory_or_file;
// Selected drive
static int selected_drive;
// Show archive item variable
static bool show_archive_item;
// Show hidden item variable
static bool show_hidden_item;
// Show read only item variable
static bool show_read_only_item;
// Show system item variable
static bool show_system_item;
// Clipboard item name string pointer variable
static char *clipboard_item_name;
// Clipboard item path string pointer variable
static char *clipboard_item_path;
// Clipboard status variable
static unsigned char clipboard_status;  // 0 for nothing, 1 for copying, 2 for cutting.
// History path names string array variable
static char history_path_names[HISTORY_LENGTH][MAX_PATH_LENGTH + 1];
// History index variable
static int history_index;
// Item attributes variable
static unsigned char *item_attributes;
// Open Kitten library
static int kitten_status;
// Disable ESC key variable from FDOSTUI
extern bool _wm_esc_flag;

// Bar

// Window of bar pointer variable
static window *bar;
// Menubar of bar pointer variable
static menubar *bar_menus;


// File Manager

// Window of file manager pointer variable
static window *file_manager;
// Menubar of file manager pointer variable
static menubar *file_manager_menus;
// Current directory label of file manager pointer variable
static label *current_directory_label;
// Current drivers listbox of file manager pointer variable
static listbox *drivers;
// Current directories and files of file manager pointer variable
static listbox *directories_and_files;
// "Item Attribute:" message label
static label *item_attributes_label;
// Item attribute label
static label *item_attributes_view;


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
void upper_directory_control_for_go_menu_item(void);
// Get item name function
char *get_item_name(char *);
// Item attribute viewing function
void item_attributes_viewing(int);
// Select directory or file function
void select_directory_or_file(listbox const *, void *);
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
// Quit function
void quit(menuitem *, void *);
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
// Paste function
void paste_item(menuitem *, void *);
// Rename item function
void rename_item(menuitem *, void *);
// Change archive attribute function
void change_archive_attribute(menuitem *, void *);
// Change hidden attribute function
void change_hidden_attribute(menuitem *, void *);
// Change read only attribute function
void change_read_only_attribute(menuitem *, void *);
// Change system attribute function
void change_system_attribute(menuitem *, void *);
// Delete function
void delete_item(menuitem *, void *);
// Show archive items function
void show_archive_items(menuitem *, void *);
// Show hidden items function
void show_hidden_items(menuitem *, void *);
// Show read only items function
void show_read_only_items(menuitem *, void *);
// Show system items function
void show_system_items(menuitem *, void *);


// Bar Menus

// Internal applications menu of bar
static struct menuitem internal_applications_menu[3];

// Exit menu of bar
static struct menuitem exit_menu[2];

// Menubar of bar
static struct menuitembar menus_of_bar[3];

// File Manager Menus

// File menu of file manager
static struct menuitem file_menu[4];

// Edit menu of file manager
static struct menuitem edit_menu[12];

// View menu of file manager
static struct menuitem view_menu[6];

// Go menu of file manager
static struct menuitem go_menu[4];

// Menubar of file manager
static struct menuitembar menus_of_file_manager[5];
