/*
 *  main.cpp - Header file of main source file
 *  Written by Ercan Ersoy.
 */

// Common Variables

// Kitten library status variable
int kitten_status = kittenopen("DOSSHELL");;
// Disable ESC key variable from FDOSTUI
extern bool _wm_esc_flag;
// History path names string array variable
extern char history_path_names[][MAX_PATH_LENGTH + 1];
// Current directory string pointer variable
extern char *current_directory;
// Window of file manager pointer variable
extern window *file_manager;
// Menubar of file manager pointer variable
extern menubar *file_manager_menus;
// Current directory label of file manager pointer variable
extern label *current_directory_label;
// Current drivers listbox of file manager pointer variable
extern listbox *drivers;
// Current directories and files of file manager pointer variable
extern listbox *directories_and_files;
// "Item Attribute:" message label
extern label *item_attributes_label;
// Item attribute label
extern label *item_attributes_view;
// Menus of File Manager
extern struct menuitembar menus_of_file_manager[];

// Function Prototypes

// Command run function
void command_run(char const *);
// Exit window manager function
void exit_window_manager(menuitem *, void *);
// Quit function
void quit(menuitem *, void *);
// Upper directory control function
void upper_directory_control_for_go_menu_item(void);
// Select directory or file function
void select_directory_or_file(listbox const *, void *);
// Change current drive function
void change_current_drive(listbox const *, void *);
// Show file manager function
void show_file_manager(menuitem *, void *);
// Show run function
void show_run(menuitem *, void *);

// Bar Window Variables

// Window of bar pointer variable
static window *bar= new window(0, 0, _video_cols, 3);
// Menubar of bar pointer variable
static menubar *bar_menus = new menubar(0, 0, _video_cols, 1);

// Bar Menus

// Internal applications menu of bar
struct menuitem internal_applications_menu[] =
{
   {reinterpret_cast<unsigned char const*>(kittengets(2, 1, "File Manager")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, show_file_manager, 0},  // File manager
   {reinterpret_cast<unsigned char const*>(kittengets(2, 2, "Run")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, show_run, 0},  // Run
   {0}
};

// Exit menu of bar
struct menuitem exit_menu[] =
{
   {reinterpret_cast<unsigned char const*>(kittengets(3, 1, "Quit")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, quit, 0},  // Quit
   {0}
};

// Menubar of Bar

struct menuitembar menus_of_bar[] =
{
   {reinterpret_cast<unsigned char const*>(kittengets(2, 0, "Internal Applications")), 0, SCAN_NONE, internal_applications_menu},  // Internal Applications
   {reinterpret_cast<unsigned char const*>(kittengets(3, 0, "Exit")), 0, SCAN_NONE, exit_menu},  // Exit
   {0}
};
