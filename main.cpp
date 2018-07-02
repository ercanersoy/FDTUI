/*
 *  main.c - Main source file
 *  Written by Ercan Ersoy and Mark Olesen.
 */

#include "main.h"

// Common Functions

// Exit window manager function
void exit_window_manager(menuitem *, void *)
{
   // Close window manager
   wm_deinit();

   // Release current directory string on memory
   free(current_directory);
   // Release clipboard item path name string on memory
   free(clipboard_item_name);
   // Release clipboard item path string on memory
   free(clipboard_item_path);
}

// Command run function
void command_run(char const* command)
{
   // Mouse release on memory
   mouse_deinit();

   system("CLS");

   // Run command
   system(command);

   // Mouse initalization
   mouse_init();

   // Mouse show
   mouse_show();

   // Hide cursor
   cursor_hide();

   // Draw window manager
   wm_draw(NULL);
}

// Directory view function
void directory_view(menuitem *, void *)
{
   // Element pointer variable
   struct dirent *element;
   // Directory pointer variable
   DIR *directory = opendir(getcwd(current_directory, PATH_MAX + 1));
   // Current drive letter
   unsigned int current_drive_letter;
   // Drive letter count information
   unsigned int drive_letter_count;
   // Drive letter information
   unsigned int drive_letter;
   // Previous drive letter information
   unsigned int previous_drive_letter = 0;
   // Drive
   unsigned char *drive = (unsigned char *)malloc(3);

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

   closedir(directory);

   // Clear the drivers listbox
   drivers->remove_all();

   // Get current drive letter
   current_drive_letter = (int)current_directory[0] - 64;

   // Get drive letter count
   _dos_setdrive(NULL, &drive_letter_count);

   // All drive letters been tested
   for(int i = 1; i <= drive_letter_count; i++)
   {
      // Drive letter test
      _dos_setdrive(i, &drive_letter_count);

      //If drive letter is exists
      _dos_getdrive(&drive_letter);

      if(!drive_letter || drive_letter == previous_drive_letter) // If drive letter is null or drive letter is not changed
      {
         // Continue loop
         continue;
      }

      // Drive letter calculation and assignment
      *drive = (unsigned char)(64 + i);
      // Assignment double dot
      *(drive + 1) = ':';
      // Assignment null
      *(drive + 2) = '\0';

      // Add drive to drivers listbox
      drivers->add(drive);

      // Set previous drive letter
      previous_drive_letter = i;
   }

   // Free drive string
   free(drive);

   // Set current drive letter
   _dos_setdrive(current_drive_letter, &drive_letter_count);

   wm_draw(NULL);
}

// Upper directory control function
void upper_directory_control(void)
{
   if(strlen(current_directory) == 3)  // If directory exists
   {
      // Up directory menu disabled
      go_menu[2].m_flags |= MENUITEM_DISABLED;
   }
   else  // If not directory exists
   {
      // Up directory menu enabled
      go_menu[2].m_flags = !(go_menu[2].m_flags ^ MENUITEM_DISABLED);
   }
}

// Change current drive function
void change_current_drive(listbox const *, void *)
{
   // Drive
   char *drive = (char *)malloc(4);
   // Drive letter information
   unsigned int drive_letter_information = 0;
   // Drive letter count information
   unsigned int drive_letter_count = 0;

   // Get item from drivers listbox
   strcpy(drive, (char *)drivers->get_item(drivers->get_selected_first()));

   // Calculate drive
   drive_letter_information = *drive - 64;

   // Change current drive
   _dos_setdrive(drive_letter_information, &drive_letter_count);
   // Get drive letter information
   _dos_getdrive(&drive_letter_information);

   if(!drive_letter_information)  // If drive letter information is null
   {
      // Free drive information from memory
      free(drive);

      // Exit function
      return;
   }

   // Change current directory
   change_current_directory(strcat(drive, "\\"), 0);

   // Free drive information from memory
   free(drive);
}

// Change current directory function
void change_current_directory(char *directory, unsigned char status)
{
   // Previous drive letter
   char previous_drive_letter;
   // Drive letter count
   unsigned int drive_letter_count = 0;

   if(status == 0)  // If status is adding new history item
   {
      if(directory != 0)  // If directory pointer is not null
      {
         // Change Directory
         chdir(directory);
      }

      if(history_index < HISTORY_LENGTH - 1)  // If history index is less than history length minus 1
      {
         // History index increase
         history_index++;
         // History save
         strcpy(history_path_names[history_index], getcwd(current_directory, PATH_MAX + 1));

         for(int i = history_index + 1; i < HISTORY_LENGTH - 1; i++)  // History path names clear loop
         {
            // History path names clear
            history_path_names[i][0] = '\0';
         }
      }
      else  // If history index is equals history length - 1
      {
         for(int i = 0; i < HISTORY_LENGTH - 1; i++)
         {
            // History shifting
            strcpy(history_path_names[i], history_path_names[i + 1]);
         }

         // History save
         strcpy(history_path_names[history_index], getcwd(current_directory, PATH_MAX + 1));
      }
   }
   else if(status == 1)  // If status is changing previous history item
   {
      // Get previous drive letter
      previous_drive_letter = history_path_names[history_index][0];

      // Decrase History
      history_index--;

      if(history_path_names[history_index][0] != previous_drive_letter)    // If equals currnet drive letter and previous drive letter
      {
         // Change current drive
        _dos_setdrive(history_path_names[history_index][0] - 64, &drive_letter_count);
      }

      // Change Directory
      chdir(history_path_names[history_index]);
   }
   else if(status == 2)  // If status is changing previous history item
   {
      // Get previous drive letter
      previous_drive_letter = history_path_names[history_index][0];

      // Increase History
      history_index++;

      if(history_path_names[history_index][0] != previous_drive_letter)  // If equals currnet drive letter and previous drive letter
      {
         // Change current drive
        _dos_setdrive(history_path_names[history_index][0] - 64, &drive_letter_count);
      }

      // Change Directory
      chdir(history_path_names[history_index]);
   }

   // Upper directory control
   upper_directory_control();

   if(history_index == 0)  // If history index equals 0
   {
      // Back menu item disabled
      go_menu[0].m_flags |= MENUITEM_DISABLED;
   }
   else
   {
      // Back menu item enabled
      go_menu[0].m_flags = !(go_menu[0].m_flags ^ MENUITEM_DISABLED);
   }

   if((history_index == HISTORY_LENGTH - 1) || history_path_names[history_index + 1][0] == '\0')  // If history index equals history length minus 1 or next history item equals 0
   {
      // Foward menu item disabled
      go_menu[1].m_flags |= MENUITEM_DISABLED;
   }
   else
   {
      // Foward menu item enabled
      go_menu[1].m_flags = !(go_menu[1].m_flags ^ MENUITEM_DISABLED);
   }

   // Refresh directory
   directory_view(NULL, NULL);
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
   // Target file open
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
      *source_item_path = NULL;
      // Clean target item path
      *target_item_path = NULL;
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
      *item_path = NULL;
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
void history_back(struct menuitem *menu_item, void *)
{
   // Change current directory to back history item
   change_current_directory(NULL, 1);
}

// History foward function
void history_foward(menuitem *, void *)
{
   // Change current directory to foward history item
   change_current_directory(NULL, 2);
}

// Change to upper directory function
void change_to_upper_directory(struct menuitem *menu_item, void *)
{
   // Change current directory to upper directory
   change_current_directory("..", 0);
}

// Bar Menu Functions

// Show file manager function
void show_file_manager(menuitem *, void *)
{
   if(!file_manager->get_visible())  // If file manager visible
   {
      // File manager set to visible
      file_manager->set_visible();

      // Refresh directory
      directory_view(NULL, NULL);

      // Draw file manager
      wm_draw(file_manager);
   }
}

// Show file manager function
void show_run(menuitem *, void *)
{
   // Run command
   command_run((char *)popup_input(reinterpret_cast<unsigned char const*>(STRING_RUN), reinterpret_cast<unsigned char const*>(STRING_COMMAND_TO_RUN), reinterpret_cast<unsigned char const*>("")));
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
   exit_window_manager(NULL, NULL);

   // Exit FreeDOS TUI Shell
   exit(EXIT_SUCCESS);
}

// Reboot function
void reboot(menuitem *, void *)
{
   // Exit window manager
   exit_window_manager(NULL, NULL);

   // Reboot computer via FDAPM
   command_run("FDAPM WARMBOOT");
   
   return;
}

// Poweroff function
void poweroff(menuitem *, void *)
{
   // Exit window manager
   exit_window_manager(NULL, NULL);

   // Poweroff computer via FDAPM
   command_run("FDAPM POWEROFF");
}

// Menu Function Menus

// Item open function
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
      change_current_directory(item_name, 0);
   }
   else  // If item is file
   {
      // Run executable file
      command_run(item_name);
   }
}

// New directory function
void new_directory(menuitem *, void *)
{
   // Make new directory of specified name
   mkdir((char *)popup_input(reinterpret_cast<unsigned char const*>(STRING_NEW_DIRECTORY), reinterpret_cast<unsigned char const*>(STRING_DIRECTORY_NAME), reinterpret_cast<unsigned char const*>("")));

   // Refresh directories
   directory_view(NULL, NULL);
}

// Exit file manager function
void file_manager_exit(menuitem *, void *)
{
   // Hide file manager window
   file_manager->set_hidden();
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
   directory_view(NULL, NULL);
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
   directory_view(NULL, NULL);
}

// Show hidden files function
void show_hidden_files(menuitem *menu_item, void *)
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
   directory_view(NULL, NULL);  
}

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

   // Current directory variable allocate
   current_directory = (char *) malloc(PATH_MAX + 1);
   // Get current directory
   getcwd(current_directory, PATH_MAX + 1);

   // Upper directory control
   upper_directory_control();

   // Bar

   // Register bar window
   wm_register_window(bar);

   // Set menu of bar
   bar_menus->set_menu(menus_of_bar);

   // Set bar window
   bar->set_attributes(window::EMPTY);
   bar->add(bar_menus);

   // File Manager

   // History initalizing
   strcpy(history_path_names[0], current_directory);

   // Register file manager window
   wm_register_window(file_manager);

   // Set current directory label text
   current_directory_label->set_text((unsigned char *)current_directory);

   // Set menu of drivers
   drivers->set_signal_selected(change_current_drive);

   // Set menu of file manager
   file_manager_menus->set_menu(menus_of_file_manager);

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
   exit_window_manager(NULL, NULL);

   // Exit FreeDOS TUI Shell
   return EXIT_SUCCESS;
}
