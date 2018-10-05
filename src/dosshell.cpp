/*
 *  dosshell.cpp - Main source file
 *  Written by Ercan Ersoy and Mark Olesen.
 */

#include <dosshell.h>


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

   // Clear screen
   system("CLS");

   // Run command
   system(command);

   // Show end
   printf(kittengets(1, 1, "\r\nPress any key to return to the DOSSHELL."));
   getchar();

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
   DIR *directory = opendir(getcwd(current_directory, MAX_NAME_LENGTH + 1));
   // Element status variable
   struct stat element_status;
   // Listbox item of element  variable
   char listbox_item_of_element[MAX_NAME_LENGTH + 8] = { 0 };
   // Current drive letter  variable
   unsigned int current_drive_letter;
   // Drive letter count information  variable
   unsigned int drive_letter_count;
   // Drive letter information  variable
   unsigned int drive_letter;
   // Previous drive letter information  variable
   unsigned int previous_drive_letter = 0;
   // Drive  variable
   unsigned char *drive = (unsigned char *)malloc(3);

   // Clear the directories and files listbox
   directories_and_files->remove_all();

   // Set text of current directory label
   current_directory_label->set_text((unsigned char *)current_directory);

   while(element = readdir(directory))  // If element is exists
   {
      if(!strcmp(element->d_name, ".") || (!strcmp(element->d_name, "..") && (strlen(current_directory) == 3)))  // If item names not "." or ".." and currnet directory is upper directory
      {
         // Continue while loop
         continue;
      }

      if((element->d_attr & _A_ARCH) && !show_archive_item)  // If archive item not showing condition is true
      {
         // Continue while loop
         continue;
      }

      if((element->d_attr & _A_HIDDEN) && !show_hidden_item)  // If hidden item not showing condition is true
      {
         // Continue while loop
         continue;
      }

      if((element->d_attr & _A_RDONLY) && !show_read_only_item)  // If read only item not showing condition is true
      {
         // Continue while loop
         continue;
      }

      if((element->d_attr & _A_SYSTEM) && !show_system_item)  // If system item not showing condition is true
      {
         // Continue while loop
         continue;
      }

      stat(element->d_name, &element_status);

      if(S_ISDIR(element_status.st_mode))  // If item is directory
      {
         strcat(listbox_item_of_element, "<DIR>  ");
         // Add item on the directories listbox
         directories_and_files->add(reinterpret_cast<unsigned char const*>(strcat(listbox_item_of_element, element->d_name)));
      }
      else  // If item is file
      {
         strcat(listbox_item_of_element, "<FILE> ");
         // Add item on the files listbox
         directories_and_files->add(reinterpret_cast<unsigned char const*>(strcat(listbox_item_of_element, element->d_name)));
      }

      listbox_item_of_element[0] = '\0';
   }

   closedir(directory);

   // Set selected item variable
   selected_directory_or_file = 0;

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

   // Draw window manager
   wm_draw(NULL);
}

// Upper directory control function
void upper_directory_control_for_go_menu_item(void)
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

// Get item name function
char *get_item_name(char* item_name, int selected)
{
   int i;  // Counter variable

   // Copy selected listbox item
   item_name = strcpy(item_name, (char *)directories_and_files->get_item(selected));

   // Shift listbox item for item name
   for(i = 0; i < MAX_NAME_LENGTH; i++)
   {
      // Shifting
      item_name[i] = item_name[i + 7];
   }

   // Clear last characters
   for(i = MAX_NAME_LENGTH - 1; i < MAX_NAME_LENGTH + 8; i++)
   {
      // Clearing
      item_name[i] = '\0';
   }

   // Return item name
   return item_name;
}

// Item attributes viewing function
void item_attributes_viewing(int selected)
{
   // Item name variable
   char *item_name = (char *)calloc(MAX_NAME_LENGTH + 8, 1);
   // Selected item attributes variable
   unsigned int selected_directory_or_file_attributes;

   // Get selected item attributes
   _dos_getfileattr(reinterpret_cast<char const*>(get_item_name(item_name, selected)), &selected_directory_or_file_attributes);

   if(selected_directory_or_file_attributes & _A_ARCH)  // If item is archive item
   {
      // Set item attributes as archive
      item_attributes[0] = 'A';

      // Set "Archive" menu item
      edit_menu[5].m_flags |= MENUITEM_VALUE;
   }
   else  // If item isn't archive item
   {
      // Set item attributes as not archive
      item_attributes[0] = '-';

      // Clear "Archive" menu item
      edit_menu[5].m_flags &= edit_menu[5].m_flags ^ MENUITEM_VALUE;
   }

   if(selected_directory_or_file_attributes & _A_HIDDEN)  // If item is hidden item
   {
      // Set item attributes as hidden
      item_attributes[1] = 'H';

      // Set "Hidden" menu item
      edit_menu[6].m_flags |= MENUITEM_VALUE;
   }
   else  // If item isn't hidden item
   {
      // Set item attributes as not hidden
      item_attributes[1] = '-';

      // Clear "Hidden" menu item
      edit_menu[6].m_flags &= edit_menu[6].m_flags ^ MENUITEM_VALUE;
   }

   if(selected_directory_or_file_attributes & _A_RDONLY)  // If item is read only item
   {
      // Set item attributes as read only
      item_attributes[2] = 'R';

      // Set "Read Only" menu item
      edit_menu[7].m_flags |= MENUITEM_VALUE;
   }
   else  // If item isn't read only item
   {
      // Set item attributes as not read only
      item_attributes[2] = '-';

      // Clear "Read Only" menu item
      edit_menu[7].m_flags &= edit_menu[7].m_flags ^ MENUITEM_VALUE;
   }

   if(selected_directory_or_file_attributes & _A_SYSTEM)  // If item is system item
   {
      // Set item attributes as system
      item_attributes[3] = 'S';

      // Set "System" menu item
      edit_menu[8].m_flags |= MENUITEM_VALUE;
   }
   else  // If item isn't system item
   {
      // Set item attributes as not system
      item_attributes[3] = '-';

      // Clear "System" menu item
      edit_menu[8].m_flags &= edit_menu[8].m_flags ^ MENUITEM_VALUE;
   }

   // Set item attributes label text
   item_attributes_view->set_text(item_attributes);

   // Free item name variable
   free(item_name);
}

// Select directory or file function
void select_directory_or_file(listbox const *, void *)
{
   if(selected_directory_or_file == directories_and_files->get_selected_first())  // If selected item is clicked
   {
      // Open item
      item_open(NULL, NULL);

      // Refresh directory
      directory_view(NULL, NULL);

      // Exit function
      return;
   }

   // Item attribute viewing function
   item_attributes_viewing(selected_directory_or_file);

   // Set selected directory or file
   selected_directory_or_file = directories_and_files->get_selected_first();

   // Draw window manager
   wm_draw(NULL);
}

// Change current drive function
void change_current_drive(listbox const *, void *)
{
   if(selected_drive == drivers->get_selected_first())  // If selected item is clicked
   {
      // Drive variable
      char *drive = (char *)malloc(4);
      // Drive letter information variable
      unsigned int drive_letter_information = 0;
      // Drive letter count information variable
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

      // Item attribute viewing function
      item_attributes_viewing(0);

      // Change current directory
      change_current_directory(strcat(drive, "\\"), 0);

      // Free drive information from memory
      free(drive);
   }

   // Set selected drive
   selected_drive = drivers->get_selected_first();

   // Draw window manager
   wm_draw(NULL);
}

// Change current directory function
void change_current_directory(char *directory, unsigned char status)
{
   // Previous drive letter variable
   char previous_drive_letter;
   // Drive letter count variable
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
         strcpy(history_path_names[history_index], getcwd(current_directory, MAX_PATH_LENGTH + 1));

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
         strcpy(history_path_names[history_index], getcwd(current_directory, MAX_PATH_LENGTH + 1));
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
   upper_directory_control_for_go_menu_item();

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
   // Source item name string pointer variable
   char *source_item_name = (char *)malloc(MAX_NAME_LENGTH + 1);
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
   char *item_name = (char *)malloc(MAX_NAME_LENGTH + 1);
   // Item path string pointer variable
   char *item_path = (char *)malloc(MAX_PATH_LENGTH + 1);

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
void history_back(menuitem *, void *)
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
void change_to_upper_directory(menuitem *, void *)
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

	  // Item attributes viewing
      item_attributes_viewing(0);

      // Draw file manager
      wm_draw(file_manager);
   }
}

// Show file manager function
void show_run(menuitem *, void *)
{
   // Input of command line
   char* command = (char *)popup_input(reinterpret_cast<unsigned char const*>(kittengets(12, 0, "Run")), reinterpret_cast<unsigned char const*>(kittengets(12, 1, "Command to Run:")), reinterpret_cast<unsigned char const*>(""));

   if(command)  // If command is not blank
   {
      // Run command
      command_run(command);
   }
   
}

// Quit function
void quit(menuitem *, void *)
{
   // Exit window manager
   exit_window_manager(NULL, NULL);

   if(kitten_status != -1)  // If Kitten library is open
   {
      // Close Kitten library
      kittenclose();
   }

   // Exit FreeDOS TUI Shell
   exit(EXIT_SUCCESS);
}


// File Manager Menu Functions

// Item open function
void item_open(menuitem *, void *)
{
   // Item name variable
   char *item_name = (char *)calloc(MAX_NAME_LENGTH + 8, 1);
   // Status variable
   struct stat status;

   // Copy listbox item
   item_name = get_item_name(item_name, directories_and_files->get_selected_first());

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

   free(item_name);
}

// New directory function
void new_directory(menuitem *, void *)
{
   // Make new directory of specified name
   mkdir((char *)popup_input(reinterpret_cast<unsigned char const*>(kittengets(9, 0, "New Directory")), reinterpret_cast<unsigned char const*>(kittengets(9, 1, "Directory Name:")), reinterpret_cast<unsigned char const*>("")));

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
   // Item name variable
   char *item_name = (char *)calloc(MAX_PATH_LENGTH + 8, 1);

   // Copy listbox item
   item_name = get_item_name(item_name, directories_and_files->get_selected_first());

   // Get selected item
   strcat(clipboard_item_name, item_name);
   // Set clipboard item path
   strcat(clipboard_item_path, current_directory);
   strcat(clipboard_item_path, "\\");
   strcat(clipboard_item_path, clipboard_item_name);
   // Set clipboard status to cut
   clipboard_status = 2;

   free(item_name);
}

// Copy function
void copy_item(menuitem *, void *)
{
   // Item name variable
   char *item_name = (char *)calloc(MAX_PATH_LENGTH + 8, 1);

   // Copy listbox item
   item_name = get_item_name(item_name, directories_and_files->get_selected_first());

   // Get selected item
   strcat(clipboard_item_name, item_name);
   // Set clipboard item path
   strcat(clipboard_item_path, current_directory);
   strcat(clipboard_item_path, "\\");
   strcat(clipboard_item_path, clipboard_item_name);
   // Set clipboard status to copy
   clipboard_status = 1;

   free(item_name);
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

// Rename function
void rename_item(menuitem *, void *)
{
   // Item name variable
   char *item_name = (char *)calloc(MAX_NAME_LENGTH + 8, 1);

   // Copy listbox item
   item_name = get_item_name(item_name, directories_and_files->get_selected_first());

   // Rename item to specified name
   rename((char *)item_name, (char *)popup_input(reinterpret_cast<unsigned char const*>(kittengets(10, 0, "Rename")), reinterpret_cast<unsigned char const*>(kittengets(10, 1, "New Name:")), reinterpret_cast<unsigned char const*>("")));

   // Refresh directories
   directory_view(NULL, NULL);

   free(item_name);
}

// Change archive attribute function
void change_archive_attribute(menuitem *menu_item, void *)
{
   // Attribute variable
   unsigned int attribute;
   // Item name variable
   char *item_name = (char *)calloc(MAX_NAME_LENGTH + 8, 1);

   // Get item name
   item_name = get_item_name(item_name, directories_and_files->get_selected_first());

   // Get item attribute
   _dos_getfileattr(item_name, &attribute);

   if(menu_item->m_flags & MENUITEM_VALUE)  // If checked
   {
      // Add archive file attribute
      _dos_setfileattr(item_name, attribute | _A_ARCH);
   }
   else  // If not checked
   {
      // Remove archive file attribute
      _dos_setfileattr(item_name, !(attribute ^ _A_ARCH));
   }

   free(item_name);

   // Refresh directories
   directory_view(NULL, NULL);
}

// Change hidden attribute function
void change_hidden_attribute(menuitem *menu_item, void *)
{
   // Attribute variable
   unsigned int attribute;
   // Item name variable
   char *item_name = (char *)calloc(MAX_NAME_LENGTH + 8, 1);

   // Get item name
   item_name = get_item_name(item_name, directories_and_files->get_selected_first());

   // Get item attribute
   _dos_getfileattr(item_name, &attribute);

   if(menu_item->m_flags & MENUITEM_VALUE)  // If checked
   {
      // Add hidden file attribute
      _dos_setfileattr(item_name, attribute | _A_HIDDEN);
   }
   else  // If not checked
   {
      // Remove hidden file attribute
      _dos_setfileattr(item_name, !(attribute ^ _A_HIDDEN));
   }

   free(item_name);

   // Refresh directories
   directory_view(NULL, NULL);
}

// Change read only attribute function
void change_read_only_attribute(menuitem *menu_item, void *)
{
   // Attribute variable
   unsigned int attribute;
   // Item name variable
   char *item_name = (char *)calloc(MAX_NAME_LENGTH + 8, 1);

   // Get item name
   item_name = get_item_name(item_name, directories_and_files->get_selected_first());

   // Get item attribute
   _dos_getfileattr(item_name, &attribute);

   if(menu_item->m_flags & MENUITEM_VALUE)  // If checked
   {
      // Add read only file attribute
      _dos_setfileattr(item_name, attribute | _A_RDONLY);
   }
   else  // If not checked
   {
      // Remove read only file attribute
      _dos_setfileattr(item_name, !(attribute ^ _A_RDONLY));
   }

   free(item_name);

   // Refresh directories
   directory_view(NULL, NULL);
}

// Change system attribute function
void change_system_attribute(menuitem *menu_item, void *)
{
   // Attribute variable
   unsigned int attribute;
   // Item name variable
   char *item_name = (char *)calloc(MAX_NAME_LENGTH + 8, 1);

   // Get item name
   item_name = get_item_name(item_name, directories_and_files->get_selected_first());

   // Get item attribute
   _dos_getfileattr(item_name, &attribute);

   if(menu_item->m_flags & MENUITEM_VALUE)  // If checked
   {
      // Add system file attribute
      _dos_setfileattr(item_name, attribute | _A_SYSTEM);
   }
   else  // If not checked
   {
      // Remove system file attribute
      _dos_setfileattr(item_name, !(attribute ^ _A_SYSTEM));
   }

   free(item_name);

   // Refresh directories
   directory_view(NULL, NULL);
}

// Delete Function
void delete_item(menuitem *, void *)
{
   // Item name variable
   char *item = (char *)calloc(MAX_PATH_LENGTH + 8, 1);

   // Copy listbox item
   item = get_item_name(item, directories_and_files->get_selected_first());

   if(remove(item))  // If file and it can be deleted
   {
      // Remove item
      remove_item(item);
   }

   // Refresh directories
   directory_view(NULL, NULL);

   free(item);
}

// Show archive items function
void show_archive_items(menuitem *menu_item, void *)
{
   if(menu_item->m_flags & MENUITEM_VALUE)  // If checked
   {
      // Set show archive item as true
      show_archive_item = true;
   }
   else  // If not checked
   {
      // Set show archive item as false
      show_archive_item = false;
   }
   
   // Refresh directories
   directory_view(NULL, NULL);  
}

// Show hidden items function
void show_hidden_items(menuitem *menu_item, void *)
{
   if(menu_item->m_flags & MENUITEM_VALUE)  // If checked
   {
      // Set show hidden item as true
      show_hidden_item = true;
   }
   else  // If not checked
   {
      // Set show hidden item as false
      show_hidden_item = false;
   }
   
   // Refresh directories
   directory_view(NULL, NULL);  
}

// Show read only items function
void show_read_only_items(menuitem *menu_item, void *)
{
   if(menu_item->m_flags & MENUITEM_VALUE)  // If checked
   {
      // Set show read only item as true
      show_read_only_item = true;
   }
   else  // If not checked
   {
      // Set show read only item as false
      show_read_only_item = false;
   }
   
   // Refresh directories
   directory_view(NULL, NULL);  
}

// Show system items function
void show_system_items(menuitem *menu_item, void *)
{
   if(menu_item->m_flags & MENUITEM_VALUE)  // If checked
   {
      // Set show system item as true
      show_system_item = true;
   }
   else  // If not checked
   {
      // Set show system item as false
      show_system_item = false;
   }
   
   // Refresh directories
   directory_view(NULL, NULL);  
}


// Main Function

int main(int argc, char *argv[])
{
   // Initalization

   if(argc == 2)  // If argument count is 2.
   {
      if(argv[1][0] == '/' && argv[1][1] == '?')  // If help argument have used.
      {
         // Help message.
         puts("FreeDOS TUI Shell 0.2");
         puts("");
         puts(kittengets(0, 0, "  FreeDOS TUI Shell is a TUI Shell for FreeDOS."));
         puts("");
         puts(kittengets(0, 1, "  Copyright (C) 2018 Ercan Ersoy"));
         puts(kittengets(0, 2, "  FreeDOS TUI Shell licensed under GNU GPL version 3."));

         // Exit FreeDOS TUI Shell
         return EXIT_SUCCESS;
      }
   }

   // Initializtion window and get error information
   wm_error error= wm_init();

   if(error)  // If FDOSTUI initalization give error
   {
      // Display error message
      puts(kittengets(1, 0, "Unable to initialze FDOSTUI subsystem."));

      // CLose Kitten Library
      kittenclose();

      // Exit FreeDOS TUI Shell with error
      return error;
   }


   // Current Directory

   // Current directory variable allocate
   current_directory = (char *) malloc(MAX_PATH_LENGTH + 1);
   // Get current directory
   getcwd(current_directory, MAX_PATH_LENGTH + 1);

   // Upper directory control
   upper_directory_control_for_go_menu_item();


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

   // Set menu of file manager
   file_manager_menus->set_menu(menus_of_file_manager);

   // Set current directory label text
   current_directory_label->set_text((unsigned char *)current_directory);

   // Set signal of drivers listbox
   drivers->set_signal_selected(change_current_drive);
   drivers->set_emit_selected_signal_always(true);

   // Set signal of directories and files listbox
   directories_and_files->set_signal_selected(select_directory_or_file);
   directories_and_files->set_emit_selected_signal_always(true);

   // Set "Item Attribute:" message label text
   item_attributes_label->set_text((unsigned char *)kittengets(11, 0, "Item Attributes:"));

   // Set file manager window
   file_manager->set_attributes(window::TITLE | window::BORDER);
   file_manager->set_title((unsigned char *)kittengets(4, 0, "File Manager"));
   file_manager->add(file_manager_menus);
   file_manager->add(current_directory_label);
   file_manager->add(drivers);
   file_manager->add(directories_and_files);
   file_manager->add(item_attributes_label);
   file_manager->add(item_attributes_view);
   file_manager->set_hidden();

   // Draw bar
   wm_draw_widget(bar);

   // Run window manager
   wm_run();

   // Exit window manager
   exit_window_manager(NULL, NULL);

   if(kitten_status != -1)  // If Kitten library is open
   {
      // Close Kitten library
      kittenclose();
   }

   // Exit FreeDOS TUI Shell
   return EXIT_SUCCESS;
}
