/*********************************
 * FreeDOS TUI Shell Source File *
 *********************************/

#include <direct.h>
#include <mouse.h>
#include <stdio.h>
#include <stdlib.h>
#include <fdostui.hpp>

// Current Directory

char current_directory[PATH_MAX + 1];

// Bar

window *bar = new window(0, 0, _video_cols, 3);
menubar *bar_menus = new menubar(0, 0, _video_cols, 1);

// File Manager

window *file_manager = new window(5, 6, 68, 17);
menubar *file_manager_menus = new menubar(0, 0, _video_cols, 1);
label *current_directory_label = new label(1, 2, 65, 3);
listbox *drivers = new listbox(1, 3, 65, 3);
listbox *directories_and_files = new listbox(1, 6, 65, 9);

// Run Function

void command_run(char const* command)
{
   mouse_deinit();
   system(command);
   mouse_init();
   mouse_show();
   wm_draw(0);
}

// Menu Functions

void application_file_manager(menuitem *, void *)
{
   file_manager->draw();
   return;
}

void application_edit(menuitem *, void *)
{
   command_run("EDIT");
   return;
}

void application_edlin(menuitem *, void *)
{
   command_run("EDLIN");
   return;
}

void dosshell_quit(menuitem *, void *)
{
    wm_deinit();
    exit(0);
}

void reboot(menuitem *, void *)
{
   wm_deinit();
   command_run("FDAPM WARMBOOT");
   return;
}

void poweroff(menuitem *, void *)
{
   wm_deinit();
   command_run("FDAPM POWEROFF");
   return;
}

// Bar Menus

struct menuitem internal_applications_menu[] =
{
   {reinterpret_cast<unsigned char const*>("File Manager"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, application_file_manager, 0},
   {0}
};

struct menuitem dos_applications_menu[] =
{
   {reinterpret_cast<unsigned char const*>("EDIT"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, application_edit, 0},
   {reinterpret_cast<unsigned char const*>("EDLIN"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, application_edlin, 0},
   {0}
};

struct menuitem exit_menu[] =
{
   {reinterpret_cast<unsigned char const*>("Quit"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, dosshell_quit, 0},
   {reinterpret_cast<unsigned char const*>("Reboot"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, reboot, 0},
   {reinterpret_cast<unsigned char const*>("Power Off"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, poweroff, 0},
   {0}
};

struct menuitembar menus_of_bar[] =
{
   {reinterpret_cast<unsigned char const*>("Internal Apps"), 0, SCAN_NONE, internal_applications_menu},
   {reinterpret_cast<unsigned char const*>("DOS Apps"), 0, SCAN_NONE, dos_applications_menu},
   {reinterpret_cast<unsigned char const*>("Exit"), 0, SCAN_NONE, exit_menu},
   {0}
};

// File Manager Menus

struct menuitem file_menu[] =
{
   {reinterpret_cast<unsigned char const*>("Open"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, 0, 0},
   {reinterpret_cast<unsigned char const*>("New Folder"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {0}
};

struct menuitem edit_menu[] =
{
   {reinterpret_cast<unsigned char const*>("Cut"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("Copy"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("Paste"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, 0, 0},
   {reinterpret_cast<unsigned char const*>("Delete"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {0}
};

struct menuitem view_menu[] =
{
   {reinterpret_cast<unsigned char const*>("Refresh"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, 0, 0},
   {reinterpret_cast<unsigned char const*>("Show Hidden Files"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX | MENUITEM_SEPERATOR, 0, 0},
   {reinterpret_cast<unsigned char const*>("Sort"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE,  MENUITEM_SUBMENU, 0, 0},
   {reinterpret_cast<unsigned char const*>("By Name"), MENUITEM_MNEMONIC_NONE, 0,  SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("By Size"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("By Type"), MENUITEM_MNEMONIC_NONE, 0,  SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("By Date"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {0},
   {0}
};

struct menuitem go_menu[] =
{
   {reinterpret_cast<unsigned char const*>("Back"), MENUITEM_MNEMONIC_NONE, 0,  SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("Foward"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("Up"), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {0}
};
struct menuitembar menus_of_file_manager[] =
{
   {reinterpret_cast<unsigned char const*>("File"), 0, SCAN_NONE, file_menu},
   {reinterpret_cast<unsigned char const*>("Edit"), 0, SCAN_NONE, edit_menu},
   {reinterpret_cast<unsigned char const*>("View"), 0, SCAN_NONE, view_menu},
   {reinterpret_cast<unsigned char const*>("Go"), 0, SCAN_NONE, go_menu},
   {0}
};

// Main Function

int main(void)
{
   wm_error error= wm_init();

   if (error)
   {
      puts("Unable to initialze FDOSTUI subsystem.");
      return error;
   }

   // Current Directory
   
   getcwd(current_directory, PATH_MAX + 1);

   // Bar

   wm_register_window(bar);

   bar_menus->set_menu(menus_of_bar);

   bar->set_attributes(window::EMPTY);
   bar->add(bar_menus);

   // File Manager

   wm_register_window(file_manager);

   file_manager_menus->set_menu(menus_of_file_manager);

   current_directory_label->set_text((unsigned char *) current_directory);

   file_manager->set_title((unsigned char *)"File Manager");
   file_manager->add(file_manager_menus);
   file_manager->add(current_directory_label);
   file_manager->add(drivers);
   file_manager->add(directories_and_files);

   // Draw bar
   
   wm_draw_widget(bar);
   
   // Run Window Manager

   wm_run();

   // Exit Window Manager

   wm_deinit();

   return 0;
}
