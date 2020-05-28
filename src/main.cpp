/*
 *  main.cpp - Main source file
 *  Written by Ercan Ersoy.
 */

#include <dosshell.hpp>
#include <main.hpp>

// Main function
int main(int argc, char *argv[])
{
   // Initalization

   if(argc == 2)  // If argument count is 2
   {
      if(argv[1][0] == '/' && argv[1][1] == '?')  // If help argument have used
      {
         // Help message.
         puts("FDTUI 0.4");
         puts("");
         puts(kittengets(0, 0, "  FDTUI is a TUI Shell for FreeDOS."));
         puts("");
         puts(kittengets(0, 1, "  Copyright (C) 2017-2020 Ercan Ersoy"));
         puts(kittengets(0, 2, "  FDTUI licensed under GNU GPL version 2 and GNU GPL version 3."));

         if(kitten_status != -1)  // If Kitten library is open
         {
            // Close Kitten library
            kittenclose();
         }

         // Exit FDTUI
         return EXIT_SUCCESS;
      }
   }

   // Initalization window and get error information
   wm_error error= wm_init();

   if(error)  // If FDOSTUI initalization give error
   {
      // Display error message
      puts(kittengets(1, 0, "Unable to initialize the FDOSTUI subsystem."));

      // Close Kitten Library
      kittenclose();

      // Exit FDTUI with error
      return error;
   }

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

   // Exit FDTUI
   return EXIT_SUCCESS;
}
