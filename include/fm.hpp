/*
 *  file.h - File manager source file
 *  Written by Ercan Ersoy.
 */

// Common Variables

// Current directory string pointer variable
char *current_directory = (char *)malloc(MAX_PATH_LENGTH + 1);
// Selected directory or file
int selected_directory_or_file = 0;
// Selected drive
int selected_drive = 0;
// Show archive item variable
bool show_archive_item = true;
// Show hidden item variable
bool show_hidden_item = false;
// Show read only item variable
bool show_read_only_item = true;
// Show system item variable
bool show_system_item = false;
// Clipboard item name string pointer variable
char *clipboard_item_name = (char *)calloc(NAME_MAX, 1);;
// Clipboard item path string pointer variable
char *clipboard_item_path = (char *)calloc(MAX_PATH_LENGTH, 1);;
// Clipboard status variable
unsigned char clipboard_status = 0;  // 0 for nothing, 1 for copying, 2 for cutting.
// History path names string array variable
char history_path_names[HISTORY_LENGTH][MAX_PATH_LENGTH + 1];
// History index variable
int history_index = 0;
// Item attributes variable
unsigned char *item_attributes = (unsigned char *)"----";
// Kitten library status variable
extern int kitten_status;

// File Manager Window Variables

// Window of file manager pointer variable
window *file_manager = new window(5, 4, 68, 19);
// Menubar of file manager pointer variable
menubar *file_manager_menus = new menubar(0, 0, _video_cols, 1);
// Current directory label of file manager pointer variable
label *current_directory_label = new label(1, 2, 65, 1);
// Current drivers listbox of file manager pointer variable
listbox *drivers = new listbox(0, 3, 66, 5);
// Current directories and files of file manager pointer variable
listbox *directories_and_files = new listbox(0, 8, 66, 8);
// "Item Attribute:" message label
label *item_attributes_label = new label(1, 16, 24, 1);
// Item attribute label
label *item_attributes_view = new label(26, 16, 4, 1);

// Function Prototypes

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
// Exit window manager function
void exit_window_manager(menuitem *, void *);
// Command run function
void command_run(char const *);
// Quit function
void quit(menuitem *, void *);
// Copy file function
void copy_file(char *, char *);

// File Manager Menus

// File menu of file manager
struct menuitem file_menu[] =
{
   {reinterpret_cast<unsigned char const *>(kittengets(5, 1, "Open")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, item_open, 0},  // Open
   {reinterpret_cast<unsigned char const *>(kittengets(5, 2, "New Directory")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, new_directory, 0},  // New directory
   {reinterpret_cast<unsigned char const *>(kittengets(5, 3, "Exit")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, file_manager_exit, 0},  // Exit
   {0}
};

// Edit menu of file manager
struct menuitem edit_menu[] =
{
   {reinterpret_cast<unsigned char const *>(kittengets(6, 1, "Cut")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, cut_item, 0},  // Cut
   {reinterpret_cast<unsigned char const *>(kittengets(6, 2, "Copy")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, copy_item, 0},  // Copy
   {reinterpret_cast<unsigned char const *>(kittengets(6, 3, "Paste")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, paste_item, 0},  // Paste
   {reinterpret_cast<unsigned char const *>(kittengets(6, 4, "Rename")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, rename_item, 0},  // Reanme
   {reinterpret_cast<unsigned char const *>(kittengets(6, 5, "Change Attribute")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SUBMENU | MENUITEM_SEPERATOR, 0, 0},  // Change Attribute
   {reinterpret_cast<unsigned char const *>(kittengets(6, 6, "Archive")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX, change_archive_attribute, 0},  // Archive
   {reinterpret_cast<unsigned char const *>(kittengets(6, 7, "Hidden")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX, change_hidden_attribute, 0},  // Hidden
   {reinterpret_cast<unsigned char const *>(kittengets(6, 8, "Read Only")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX, change_read_only_attribute, 0},  // Read Only
   {reinterpret_cast<unsigned char const *>(kittengets(6, 9, "System")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX, change_system_attribute, 0},  // System
   {0},
   {reinterpret_cast<unsigned char const *>(kittengets(6, 10, "Delete")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, delete_item, 0},  // Delete
   {0}
};

// View menu of file manager
struct menuitem view_menu[] =
{
   {reinterpret_cast<unsigned char const *>(kittengets(7, 1, "Refresh")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, directory_view, 0},  // Refresh
   {reinterpret_cast<unsigned char const *>(kittengets(7, 2, "Show Archive Items")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX | MENUITEM_VALUE, show_archive_items, 0},  // Show archive files
   {reinterpret_cast<unsigned char const *>(kittengets(7, 3, "Show Hidden Items")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX, show_hidden_items, 0},  // Show hidden files
   {reinterpret_cast<unsigned char const *>(kittengets(7, 4, "Show Read Only Items")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX| MENUITEM_VALUE, show_read_only_items, 0},  // Show read only files
   {reinterpret_cast<unsigned char const *>(kittengets(7, 5, "Show System Items")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX, show_system_items, 0},  // Show system files
   {0}
};

// Go menu of file manager
struct menuitem go_menu[] =
{
   {reinterpret_cast<unsigned char const *>(kittengets(8, 1, "Back")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_DISABLED, history_back, 0},  // Back
   {reinterpret_cast<unsigned char const *>(kittengets(8, 2, "Foward")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_DISABLED, history_foward, 0},  // Foward
   {reinterpret_cast<unsigned char const *>(kittengets(8, 3, "Up")), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_DISABLED, change_to_upper_directory, 0},  // Up
   {0}
};

// Menubar of File Manager

struct menuitembar menus_of_file_manager[] =
{
   {reinterpret_cast<unsigned char const *>(kittengets(5, 0, "File")), 0, SCAN_NONE, file_menu},  // File
   {reinterpret_cast<unsigned char const *>(kittengets(6, 0, "Edit")), 0, SCAN_NONE, edit_menu},  // Edit
   {reinterpret_cast<unsigned char const *>(kittengets(7, 0, "View")), 0, SCAN_NONE, view_menu},  // View
   {reinterpret_cast<unsigned char const *>(kittengets(8, 0, "Go")), 0, SCAN_NONE, go_menu},  // Go
   {0}
};
