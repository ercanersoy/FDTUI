/*********************************
 * FreeDOS TUI Shell Source File *
 *********************************/

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
window *file_manager = new window(5, 6, 68, 17);
// Menubar of file manager pointer variable
menubar *file_manager_menus = new menubar(0, 0, _video_cols, 1);
// Current directory label of file manager pointer variable
label *current_directory_label = new label(1, 2, 65, 1);
// Current drivers listbox of file manager pointer variable
listbox *drivers = new listbox(1, 3, 65, 3);
// Current directories and files of file manager pointer variable
listbox *directories_and_files = new listbox(1, 6, 65, 9);

// Common Functions

// Command run function
void command_run(char const* command)
{
   // Mouse release on memory
   mouse_deinit();

   // Run command
   system(command);

   // Mouse initalization
   mouse_init();

   // Mouse show
   mouse_show();

   // Draw window manager
   wm_draw(0);
}

// Directory view function
void directory_view(menuitem *, void *)
{
   // Element pointer variable
   struct dirent *element;

   // Directory pointer variable
   DIR *directory = opendir(getcwd(current_directory, PATH_MAX + 1));

   // Clear the directories and files listbox
   directories_and_files->remove_all();

   // Set text of current directory label
   current_directory_label->set_text((unsigned char *)current_directory);

   while(element = readdir(directory))  // If element is exists
   {
      if(!strcmp(element->d_name, ".") || !strcmp(element->d_name, ".."))  // If item names not "." or ".."
      {
         // Continue while loop
         continue;
      }
      else if(element->d_attr == _A_HIDDEN && !show_hidden_file)  // If showing hidden files condition is false
      {
         // Continue while loop
         continue;
      }

      // Add item on the directories and files listbox
      directories_and_files->add(reinterpret_cast<unsigned char const*>(element->d_name));
   }
}

// Copy file function
void copy_file(char *source_file, char *target_file)
{
   // Source pointer variable
   FILE *source;
   // Target pointer variable
   FILE *target;
   // Character buffer variable
   char buffer = 0;

   // Source file open
   source = fopen(source_file, "rb");
   target = fopen(target_file, "wb");

   while((buffer = fgetc(source)) != 255 && !feof(source))  // While read source file to buffer as one character
   {
      // Write buffer target
      fputc(buffer, target);
   }

   // Close source file
   fclose(source);
   // Close target file
   fclose(target);
}

// Copy directory function
void copy_directory(char *source_directory, char *target_directory)
{
   // Directory element pointer variable
   DIR *element;
   // Source item properties string pointer variable
   struct dirent *source_item_properties;
   // Source tem name string pointer variable
   char *source_item_name = (char *)malloc(256);
   // Item path string pointer variable
   char *source_item_path = (char *)malloc(MAX_PATH_LENGTH + 1);
   // Target item path string pointer variable
   char *target_item_path = (char *)malloc(MAX_PATH_LENGTH + 1);
   // Status variable
   struct stat status;

   // Make target directory
   mkdir(target_directory);

   // Open source directory
   element = opendir(source_directory);
   // Get source item properties
   source_item_properties = readdir(element);
   // Get source item name
   source_item_name = source_item_properties->d_name;

   while(source_item_name[0] != 1)    // While if item name is exists
   {
      if(strcmp(source_item_name, ".") && strcmp(source_item_name, "..") && (strlen(source_item_path) + strlen(target_item_path) <= (MAX_PATH_LENGTH + 1) * 2))  // If item provide item condition
      {
         // Make source directory path
         strcpy(source_item_path, source_directory);
         strcat(source_item_path, "\\");
         strcat(source_item_path, source_item_name);

         // Make target directory path
         strcpy(target_item_path, target_directory);
         strcat(target_item_path, "\\");
         strcat(target_item_path, source_item_name);

         // Get source item status
         stat(source_item_path, &status);

         if(S_ISDIR(status.st_mode))  // If source item is directory
         {
            // Copy directory
            copy_directory(source_item_path, target_item_path);
         }
         else  // If source item is file
         {
            // Copy file
            copy_file(source_item_path, target_item_path);
         }
      }

      // Get source item properties
      source_item_properties = readdir(element);
      // Get source item name
      source_item_name = source_item_properties->d_name;

      // Clean source item path
      *source_item_path = '\0';
      // Clean target item path
      *target_item_path = '\0';
   }

   // Close directory
   closedir(element);

   // If source item name string
   free(source_item_name);
   // If source item path string
   free(source_item_path);
   // Free target item path string
   free(target_item_path);
}

// Remove item function
char remove_item(char *removing_item_path)
{
   // Directory element pointer variable
   DIR *element;
   // Item properties pointer variable
   struct dirent *item_properties;
   // Item name string pointer variable
   char *item_name = (char *) malloc(256);
   // Item path string pointer variable
   char *item_path = (char *) malloc(260);

   // Open directory
   element = opendir(removing_item_path);
   // Get item properties
   item_properties = readdir(element);
   // Get item name
   item_name = item_properties->d_name;

   while(item_name[0] != 1)  // While if item name is exists
   {
      if(strcmp(item_name, ".") && strcmp(item_name, "..") && (strlen(item_path) + strlen(item_path) <= (MAX_PATH_LENGTH + 1) * 2))  // If item provide item condition
      {
         // Make item path
         strcpy(item_path, removing_item_path);
         strcat(item_path, "\\");
         strcat(item_path, item_name);

         if(remove(item_path))  // If remove item as file
         {
            // Remove item as directory
            remove_item(item_path);
         }
      }
      
      // Get item porperties
      item_properties = readdir(element);
      // Get item name
      item_name = item_properties->d_name;

      // Clean item path string
      *item_path = '\0';
   }

   // Close directory
   closedir(element);

   // Clean item name string
   free(item_name);
   // Clean item path string
   free(item_path);

   // Remove directory and return status
   return rmdir(removing_item_path);
}

// History back function
void history_back(menuitem *, void *)
{

}

// History foward function
void history_foward(menuitem *, void *)
{

}

// Change to upper directory function
void change_to_upper_directory(menuitem *, void *)
{
   // Change current directory to  upper directory
   chdir("..");

   // Refresh directory
   directory_view(0, 0);
}

// Bar Menu Functions

// Show file manager function
void show_file_manager(menuitem *, void *)
{
   if(!file_manager->get_visible())
   {
      file_manager->set_visible();

      directory_view(0, 0);

      wm_draw(file_manager);
   }
   return;
}

// Run application EDIT function
void run_application_edit(menuitem *, void *)
{
   // Run EDIT
   command_run("EDIT");
}

// Run application EDLIN function
void run_application_edlin(menuitem *, void *)
{
   // Run EDLIN
   command_run("EDLIN");
}

// Quit function
void quit(menuitem *, void *)
{
   // Exit window manager
   wm_deinit();

   // Exit FreeDOS TUI Shell
   exit(0);
}

// Reboot function
void reboot(menuitem *, void *)
{
   // Exit window manager
   wm_deinit();

   // Reboot computer via FDAPM
   command_run("FDAPM WARMBOOT");
   
   return;
}

// Poweroff function
void poweroff(menuitem *, void *)
{
   // Exit window manager
   wm_deinit();

   // Poweroff computer via FDAPM
   command_run("FDAPM POWEROFF");
}

// Bar Menus

// Internal applications menu of bar
struct menuitem internal_applications_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_FILE_MANAGER), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, show_file_manager, 0},  // File manager
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

// Menu Function Menus

void item_open(menuitem *, void *)
{
   // Get item name
   char *item_name = (char *)directories_and_files->get_item(directories_and_files->get_selected_first());
   // Status variable
   struct stat status;

   // Get item status
   stat(item_name, &status);

   if(S_ISDIR(status.st_mode))  // If item is dir
   {
      // Change directory
      chdir(item_name);

       // Refresh directories
      directory_view(0, 0);
   }
   else  // If item is file
   {
      // Run executable file
      command_run(item_name);
   }
}

void new_directory(menuitem *, void *)
{
   // Make new directory of specified name
   mkdir((char *)popup_input(reinterpret_cast<unsigned char const*>(STRING_NEW_DIRECTORY), reinterpret_cast<unsigned char const*>(STRING_DIRECTORY_NAME), reinterpret_cast<unsigned char const*>("")));

   // Refresh directories
   directory_view(0, 0);
}

// Cut function
void cut_item(menuitem *, void *)
{
   // Get selected item
   strcat(clipboard_item_name, (char *)directories_and_files->get_item(directories_and_files->get_selected_first()));
   // Set clipboard item path
   strcat(clipboard_item_path, current_directory);
   strcat(clipboard_item_path, "\\");
   strcat(clipboard_item_path, clipboard_item_name);
   // Set clipboard status to cut
   clipboard_status = 2;
}

// Copy function
void copy_item(menuitem *, void *)
{
   // Get selected item
   strcat(clipboard_item_name, (char *)directories_and_files->get_item(directories_and_files->get_selected_first()));
   // Set clipboard item path
   strcat(clipboard_item_path, current_directory);
   strcat(clipboard_item_path, "\\");
   strcat(clipboard_item_path, clipboard_item_name);
   // Set clipboard status to copy
   clipboard_status = 1;
}

// Paste Function
void paste_item(menuitem *, void *)
{
   // Target path variable
   char *target_path = current_directory;
   // Status variable
   struct stat status;

   // Make clipboard item name
   strcat(target_path, "\\");
   strcat(target_path, clipboard_item_name);

   if(clipboard_status == 1)  // If copy
   {
      // Get clipboard source item status
      stat(clipboard_item_path, &status);

      if(S_ISDIR(status.st_mode))  // If item is dir
      {
         // Copy directory
         copy_directory(clipboard_item_path, target_path);
      }
      else  // If item is file
      {
         // Copy file
         copy_file(clipboard_item_path, target_path);
      }
      
   }
   else if(clipboard_status == 2)  // If cut
   {
      // Move file
      rename(clipboard_item_path, target_path);
   }

   // Clean clipboard
   clipboard_status = 0;
   *clipboard_item_name = '\0';
   *clipboard_item_path = '\0';
   
   // Refresh directories
   directory_view(0, 0);
}

// Delete Function
void delete_item(menuitem *, void *)
{
   // Get selected item
   char *item = (char *)directories_and_files->get_item(directories_and_files->get_selected_first());

   if(remove(item))  // If file and it can be deleted
   {
      // Remove item
      remove_item(item);
   }
   
   // Refresh directories
   directory_view(0, 0);
}

// Exit file manager function
void file_manager_exit(menuitem *, void *)
{
   // Hide file manager window
   file_manager->set_hidden();
}


// Show hidden files function
void show_hidden_files(struct menuitem *menu_item, void *)
{
   if(menu_item->m_flags & MENUITEM_VALUE)  // If checked
   {
      show_hidden_file = true;
   }
   else
   {
      show_hidden_file = false;
   }
   
   // Refresh directories
   directory_view(0, 0);  
}

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
   {reinterpret_cast<unsigned char const*>(STRING_SHOW_HIDDEN_FILES), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX | MENUITEM_SEPERATOR, show_hidden_files, 0},  // Show hidden files
   {reinterpret_cast<unsigned char const*>(STRING_SORT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE,  MENUITEM_SUBMENU, 0, 0},  // Sort
   {reinterpret_cast<unsigned char const*>(STRING_BY_NAME), MENUITEM_MNEMONIC_NONE, 0,  SCAN_NONE, 0, 0, 0},  // By name
   {reinterpret_cast<unsigned char const*>(STRING_BY_SIZE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},  // By size
   {reinterpret_cast<unsigned char const*>(STRING_BY_TYPE), MENUITEM_MNEMONIC_NONE, 0,  SCAN_NONE, 0, 0, 0},  // By type
   {reinterpret_cast<unsigned char const*>(STRING_BY_DATE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},  // By date
   {0},
   {0}
};

// Go menu of file manager
struct menuitem go_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_BACK), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, history_back, 0},  // Back
   {reinterpret_cast<unsigned char const*>(STRING_FOWARD), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, history_foward, 0},  // Foward
   {reinterpret_cast<unsigned char const*>(STRING_UP), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, change_to_upper_directory, 0},  // Up
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

// Main Function

int main(void)
{
   // Initializtion window and get error information
   wm_error error= wm_init();

   if(error)  // If FDOSTUI initalization give error
   {
      puts(STRING_UNABLE_TO_INITIALZE_FDOSTUI_SYSTEM);
      return error;
   }

   // Current Directory
   
   current_directory = (char *) malloc(PATH_MAX + 1);
   getcwd(current_directory, PATH_MAX + 1);

   // Bar

   // Register bar window
   wm_register_window(bar);

   // Set menu of bar
   bar_menus->set_menu(menus_of_bar);

   // Set bar window
   bar->set_attributes(window::EMPTY);
   bar->add(bar_menus);

   // File Manager

   // Register file manager window
   wm_register_window(file_manager);

   // Set menu of file manager
   file_manager_menus->set_menu(menus_of_file_manager);

   // Set current directory label text
   current_directory_label->set_text((unsigned char *)current_directory);

   // Set file manager window
   file_manager->set_attributes(window::TITLE | window::BORDER);
   file_manager->set_title((unsigned char *)STRING_FILE_MANAGER);
   file_manager->add(file_manager_menus);
   file_manager->add(current_directory_label);
   file_manager->add(drivers);
   file_manager->add(directories_and_files);
   file_manager->set_hidden();

   // Drawing
   wm_draw_widget(bar);

   // Run Window Manager
   wm_run();

   // Exit Window Manager
   wm_deinit();

   free(current_directory);
   free(clipboard_item_name);
   free(clipboard_item_path);

   // Exit FreeDOS TUI Shell
   return EXIT_SUCCESS;
}
