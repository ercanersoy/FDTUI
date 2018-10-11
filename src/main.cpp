/*
 *  main.cpp - Main source file
 *  Written by Ercan Ersoy.
 */

#include <dosshell.h>


// Main function
int main(int argc, char *argv[])
{
   // Definition variables

   // Current directory string pointer variable
   current_directory = (char *)malloc(MAX_PATH_LENGTH + 1);
   // Selected directory or file
   selected_directory_or_file = 0;
   // Selected drive
   selected_drive = 0;
   // Show archive item variable
   show_archive_item = true;
   // Show hidden item variable
   show_hidden_item = false;
   // Show read only item variable
   show_read_only_item = true;
   // Show system item variable
   show_system_item = false;
   // Clipboard item name string pointer variable
   clipboard_item_name = (char *)calloc(NAME_MAX, 1);
   // Clipboard item path string pointer variable
   clipboard_item_path = (char *)calloc(MAX_PATH_LENGTH, 1);
   // Clipboard status variable
   clipboard_status = 0;  // 0 for nothing, 1 for copying, 2 for cutting.
   // History path names string array variable
   //history_path_names = (char *)calloc(HISTORY_LENGTH, MAX_PATH_LENGTH + 1);
   // History index variable
   history_index = 0;
   // Item attributes variable
   item_attributes = (unsigned char *)"----";
   // Open Kitten library
   kitten_status = kittenopen("DOSSHELL");
   // Disable ESC key variable from FDOSTUI
   _wm_esc_flag = false;

   // Bar

   // Window of bar pointer variable
   bar = new window(0, 0, _video_cols, 3);
   // Menubar of bar pointer variable
   bar_menus = new menubar(0, 0, _video_cols, 1);


   // File Manager

   // Window of file manager pointer variable
   file_manager = new window(5, 4, 68, 19);
   // Menubar of file manager pointer variable
   file_manager_menus = new menubar(0, 0, _video_cols, 1);
   // Current directory label of file manager pointer variable
   current_directory_label = new label(1, 2, 65, 1);
   // Current drivers listbox of file manager pointer variable
   drivers = new listbox(0, 3, 66, 5);
   // Current directories and files of file manager pointer variable
   directories_and_files = new listbox(0, 8, 66, 8);
   // "Item Attribute:" message label
   item_attributes_label = new label(0, 16, 16, 1);
   // Item attribute label
   item_attributes_view = new label(17, 16, 4, 1);

   // Internal Applications menu of Bar

   // File manager
   internal_applications_menu[0].m_label = reinterpret_cast<unsigned char const*>(kittengets(2, 1, "File Manager"));
   internal_applications_menu[0].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   internal_applications_menu[0].m_accel_label = 0;
   internal_applications_menu[0].m_accel_scancode = SCAN_NONE;
   internal_applications_menu[0].m_flags = 0;
   internal_applications_menu[0].m_signal = show_file_manager;
   internal_applications_menu[0].m_user_data = 0;

   // Run
   internal_applications_menu[1].m_label = reinterpret_cast<unsigned char const*>(kittengets(2, 2, "Run"));
   internal_applications_menu[1].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   internal_applications_menu[1].m_accel_label = 0;
   internal_applications_menu[1].m_accel_scancode = SCAN_NONE;
   internal_applications_menu[1].m_flags = 0;
   internal_applications_menu[1].m_signal = show_run;
   internal_applications_menu[1].m_user_data = 0;

   // Null
   internal_applications_menu[2].m_label = 0;
   internal_applications_menu[2].m_mnemonic_offset = 0;
   internal_applications_menu[2].m_accel_label = 0;
   internal_applications_menu[2].m_accel_scancode = SCAN_NONE;
   internal_applications_menu[2].m_flags = 0;
   internal_applications_menu[2].m_signal = 0;
   internal_applications_menu[2].m_user_data = 0;

   // Exit Menu of Bar

   // Quit
   exit_menu[0].m_label = reinterpret_cast<unsigned char const*>(kittengets(3, 1, "Quit"));
   exit_menu[0].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   exit_menu[0].m_accel_label = 0;
   exit_menu[0].m_accel_scancode = SCAN_NONE;
   exit_menu[0].m_flags = 0;
   exit_menu[0].m_signal = quit;
   exit_menu[0].m_user_data = 0;

   // Null
   exit_menu[1].m_label = 0;
   exit_menu[1].m_mnemonic_offset = 0;
   exit_menu[1].m_accel_label = 0;
   exit_menu[1].m_accel_scancode = SCAN_NONE;
   exit_menu[1].m_flags = 0;
   exit_menu[1].m_signal = 0;
   exit_menu[1].m_user_data = 0;

   // Menu Item Bar of Bar

   // Internal applications
   menus_of_bar[0].m_label =reinterpret_cast<unsigned char const*>(kittengets(2, 0, "Internal Applications"));
   menus_of_bar[0].m_mnemonic_offset = 0;
   menus_of_bar[0].m_accel_scancode = SCAN_NONE;
   menus_of_bar[0].m_items = internal_applications_menu;

   // Exit
   menus_of_bar[1].m_label = reinterpret_cast<unsigned char const*>(kittengets(3, 0, "Exit"));
   menus_of_bar[1].m_mnemonic_offset = 0;
   menus_of_bar[1].m_accel_scancode = SCAN_NONE;
   menus_of_bar[1].m_items = exit_menu;

   // Null
   menus_of_bar[2].m_label = 0;
   menus_of_bar[2].m_mnemonic_offset = 0;
   menus_of_bar[2].m_accel_scancode = SCAN_NONE;
   menus_of_bar[2].m_items = 0;

   // File Menu of File Manager

   // Open
   file_menu[0].m_label = reinterpret_cast<unsigned char const*>(kittengets(5, 1, "Open"));
   file_menu[0].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   file_menu[0].m_accel_label = 0;
   file_menu[0].m_accel_scancode = SCAN_NONE;
   file_menu[0].m_flags = MENUITEM_SEPERATOR;
   file_menu[0].m_signal = item_open;
   file_menu[0].m_user_data = 0;

   // New directory
   file_menu[1].m_label = reinterpret_cast<unsigned char const*>(kittengets(5, 2, "New Directory"));
   file_menu[1].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   file_menu[1].m_accel_label = 0;
   file_menu[1].m_accel_scancode = SCAN_NONE;
   file_menu[1].m_flags = MENUITEM_SEPERATOR;
   file_menu[1].m_signal = new_directory;
   file_menu[1].m_user_data = 0;

   // Exit
   file_menu[2].m_label = reinterpret_cast<unsigned char const*>(kittengets(5, 3, "Exit"));
   file_menu[2].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   file_menu[2].m_accel_label = 0;
   file_menu[2].m_accel_scancode = SCAN_NONE;
   file_menu[2].m_flags = 0;
   file_menu[2].m_signal = file_manager_exit;
   file_menu[2].m_user_data = 0;

   // Null
   file_menu[3].m_label = 0;
   file_menu[3].m_mnemonic_offset = 0;
   file_menu[3].m_accel_label = 0;
   file_menu[3].m_accel_scancode = SCAN_NONE;
   file_menu[3].m_flags = 0;
   file_menu[3].m_signal = 0;
   file_menu[3].m_user_data = 0;

   // Edit Menu of File Manager

   // Cut
   edit_menu[0].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 1, "Cut"));
   edit_menu[0].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[0].m_accel_label = 0;
   edit_menu[0].m_accel_scancode = SCAN_NONE;
   edit_menu[0].m_flags = 0;
   edit_menu[0].m_signal = cut_item;
   edit_menu[0].m_user_data = 0;

   // Copy
   edit_menu[1].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 2, "Copy"));
   edit_menu[1].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[1].m_accel_label = 0;
   edit_menu[1].m_accel_scancode = SCAN_NONE;
   edit_menu[1].m_flags = 0;
   edit_menu[1].m_signal = copy_item;
   edit_menu[1].m_user_data = 0;

   // Paste
   edit_menu[2].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 3, "Paste"));
   edit_menu[2].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[2].m_accel_label = 0;
   edit_menu[2].m_accel_scancode = SCAN_NONE;
   edit_menu[2].m_flags = MENUITEM_SEPERATOR;
   edit_menu[2].m_signal = paste_item;
   edit_menu[2].m_user_data = 0;

   // Rename
   edit_menu[3].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 4, "Rename"));
   edit_menu[3].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[3].m_accel_label = 0;
   edit_menu[3].m_accel_scancode = SCAN_NONE;
   edit_menu[3].m_flags = MENUITEM_SEPERATOR;
   edit_menu[3].m_signal = rename_item;
   edit_menu[3].m_user_data = 0;

   // Change attribute
   edit_menu[4].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 5, "Change Attribute"));
   edit_menu[4].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[4].m_accel_label = 0;
   edit_menu[4].m_accel_scancode = SCAN_NONE;
   edit_menu[4].m_flags = MENUITEM_SUBMENU | MENUITEM_SEPERATOR;
   edit_menu[4].m_signal = 0;
   edit_menu[4].m_user_data = 0;

   // Archive
   edit_menu[5].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 6, "Archive"));
   edit_menu[5].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[5].m_accel_label = 0;
   edit_menu[5].m_accel_scancode = SCAN_NONE;
   edit_menu[5].m_flags = MENUITEM_CHECKBOX;
   edit_menu[5].m_signal = change_archive_attribute;
   edit_menu[5].m_user_data = 0;

   // Hidden
   edit_menu[6].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 7, "Hidden"));
   edit_menu[6].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[6].m_accel_label = 0;
   edit_menu[6].m_accel_scancode = SCAN_NONE;
   edit_menu[6].m_flags = MENUITEM_CHECKBOX;
   edit_menu[6].m_signal = change_hidden_attribute;
   edit_menu[6].m_user_data = 0;

   // Read Only
   edit_menu[7].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 8, "Read Only"));
   edit_menu[7].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[7].m_accel_label = 0;
   edit_menu[7].m_accel_scancode = SCAN_NONE;
   edit_menu[7].m_flags = MENUITEM_CHECKBOX;
   edit_menu[7].m_signal = change_read_only_attribute;
   edit_menu[7].m_user_data = 0;

   // System
   edit_menu[8].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 9, "System"));
   edit_menu[8].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[8].m_accel_label = 0;
   edit_menu[8].m_accel_scancode = SCAN_NONE;
   edit_menu[8].m_flags = MENUITEM_CHECKBOX;
   edit_menu[8].m_signal = change_system_attribute;
   edit_menu[8].m_user_data = 0;

   // Null
   edit_menu[9].m_label = 0;
   edit_menu[9].m_mnemonic_offset = 0;
   edit_menu[9].m_accel_label = 0;
   edit_menu[9].m_accel_scancode = SCAN_NONE;
   edit_menu[9].m_flags = 0;
   edit_menu[9].m_signal = 0;
   edit_menu[9].m_user_data = 0;

   // Delete
   edit_menu[10].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 10, "Delete"));
   edit_menu[10].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   edit_menu[10].m_accel_label = 0;
   edit_menu[10].m_accel_scancode = SCAN_NONE;
   edit_menu[10].m_flags = 0;
   edit_menu[10].m_signal = delete_item;
   edit_menu[10].m_user_data = 0;

   // Null
   edit_menu[11].m_label = 0;
   edit_menu[11].m_mnemonic_offset = 0;
   edit_menu[11].m_accel_label = 0;
   edit_menu[11].m_accel_scancode = SCAN_NONE;
   edit_menu[11].m_flags = 0;
   edit_menu[11].m_signal = 0;
   edit_menu[11].m_user_data = 0;

   // View Menu of File Manager

   // Refresh
   view_menu[0].m_label = reinterpret_cast<unsigned char const*>(kittengets(7, 1, "Refresh"));
   view_menu[0].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   view_menu[0].m_accel_label = 0;
   view_menu[0].m_accel_scancode = SCAN_NONE;
   view_menu[0].m_flags = MENUITEM_SEPERATOR;
   view_menu[0].m_signal = directory_view;
   view_menu[0].m_user_data = 0;

   // Show Archive Items
   view_menu[1].m_label = reinterpret_cast<unsigned char const*>(kittengets(7, 2, "Show Archive Items"));
   view_menu[1].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   view_menu[1].m_accel_label = 0;
   view_menu[1].m_accel_scancode = SCAN_NONE;
   view_menu[1].m_flags = MENUITEM_CHECKBOX | MENUITEM_VALUE;
   view_menu[1].m_signal = show_archive_items;
   view_menu[1].m_user_data = 0;

   // Show Hidden Items
   view_menu[2].m_label = reinterpret_cast<unsigned char const*>(kittengets(7, 3, "Show Hidden Items"));
   view_menu[2].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   view_menu[2].m_accel_label = 0;
   view_menu[2].m_accel_scancode = SCAN_NONE;
   view_menu[2].m_flags = MENUITEM_CHECKBOX | MENUITEM_VALUE;
   view_menu[2].m_signal = show_hidden_items;
   view_menu[2].m_user_data = 0;

   // Show Read Only Items
   view_menu[3].m_label = reinterpret_cast<unsigned char const*>(kittengets(7, 4, "Show Read Only Items"));
   view_menu[3].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   view_menu[3].m_accel_label = 0;
   view_menu[3].m_accel_scancode = SCAN_NONE;
   view_menu[3].m_flags = MENUITEM_CHECKBOX;
   view_menu[3].m_signal = show_read_only_items;
   view_menu[3].m_user_data = 0;

   // Show System Items
   view_menu[4].m_label = reinterpret_cast<unsigned char const*>(kittengets(7, 4, "Show System Items"));
   view_menu[4].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   view_menu[4].m_accel_label = 0;
   view_menu[4].m_accel_scancode = SCAN_NONE;
   view_menu[4].m_flags = MENUITEM_CHECKBOX;
   view_menu[4].m_signal = show_system_items;
   view_menu[4].m_user_data = 0;

   // Null
   view_menu[5].m_label = 0;
   view_menu[5].m_mnemonic_offset = 0;
   view_menu[5].m_accel_label = 0;
   view_menu[5].m_accel_scancode = SCAN_NONE;
   view_menu[5].m_flags = 0;
   view_menu[5].m_signal = 0;
   view_menu[5].m_user_data = 0;

   // Go Menu of File Manager

   // Back
   go_menu[0].m_label = reinterpret_cast<unsigned char const*>(kittengets(8, 1, "Back"));
   go_menu[0].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   go_menu[0].m_accel_label = 0;
   go_menu[0].m_accel_scancode = SCAN_NONE;
   go_menu[0].m_flags = MENUITEM_DISABLED;
   go_menu[0].m_signal = history_back;
   go_menu[0].m_user_data = 0;

   // Foward
   go_menu[1].m_label = reinterpret_cast<unsigned char const*>(kittengets(8, 2, "Foward"));
   go_menu[1].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   go_menu[1].m_accel_label = 0;
   go_menu[1].m_accel_scancode = SCAN_NONE;
   go_menu[1].m_flags = MENUITEM_DISABLED;
   go_menu[1].m_signal = history_foward;
   go_menu[1].m_user_data = 0;

   // Up
   go_menu[2].m_label = reinterpret_cast<unsigned char const*>(kittengets(8, 3, "Up"));
   go_menu[2].m_mnemonic_offset = MENUITEM_MNEMONIC_NONE;
   go_menu[2].m_accel_label = 0;
   go_menu[2].m_accel_scancode = SCAN_NONE;
   go_menu[2].m_flags = MENUITEM_DISABLED;
   go_menu[2].m_signal = change_to_upper_directory;
   go_menu[2].m_user_data = 0;

   // Null
   go_menu[3].m_label = 0;
   go_menu[3].m_mnemonic_offset = 0;
   go_menu[3].m_accel_label = 0;
   go_menu[3].m_accel_scancode = SCAN_NONE;
   go_menu[3].m_flags = 0;
   go_menu[3].m_signal = 0;
   go_menu[3].m_user_data = 0;

   // Menu Item Bar of File Manager

   // File
   menus_of_file_manager[0].m_label = reinterpret_cast<unsigned char const*>(kittengets(5, 0, "File"));
   menus_of_file_manager[0].m_mnemonic_offset = 0;
   menus_of_file_manager[0].m_accel_scancode = SCAN_NONE;
   menus_of_file_manager[0].m_items = file_menu;

   // Edit
   menus_of_file_manager[1].m_label = reinterpret_cast<unsigned char const*>(kittengets(6, 0, "Edit"));
   menus_of_file_manager[1].m_mnemonic_offset = 0;
   menus_of_file_manager[1].m_accel_scancode = SCAN_NONE;
   menus_of_file_manager[1].m_items = edit_menu;

   // View
   menus_of_file_manager[2].m_label = reinterpret_cast<unsigned char const*>(kittengets(7, 0, "View"));
   menus_of_file_manager[2].m_mnemonic_offset = 0;
   menus_of_file_manager[2].m_accel_scancode = SCAN_NONE;
   menus_of_file_manager[2].m_items = view_menu;

   // Go
   menus_of_file_manager[3].m_label = reinterpret_cast<unsigned char const*>(kittengets(8, 0, "Go"));
   menus_of_file_manager[3].m_mnemonic_offset = 0;
   menus_of_file_manager[3].m_accel_scancode = SCAN_NONE;
   menus_of_file_manager[3].m_items = go_menu;
   
   // Null
   menus_of_file_manager[4].m_label = 0;
   menus_of_file_manager[4].m_mnemonic_offset = 0;
   menus_of_file_manager[4].m_accel_scancode = SCAN_NONE;
   menus_of_file_manager[4].m_items = 0;

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
