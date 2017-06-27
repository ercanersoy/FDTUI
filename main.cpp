/*********************************
 * FreeDOS TUI Shell Source File *
 *********************************/

#include <fdostui.hpp>
#include <stdio.h>

// Bar Menus

struct menuitem dos_applications_menu[] =
{
   {reinterpret_cast<unsigned char const*>("EDIT"), 0, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("EDLIN"), 0, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("FDSHIELD"), 0, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("DEFRAG"), 0, 0, SCAN_NONE, 0, 0, 0},
   {0}
};

struct menuitem exit_menu[] =
{
   {reinterpret_cast<unsigned char const*>("Reboot"), 0, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>("Power Off"), 0, 0, SCAN_NONE, 0, 0, 0},
   {0}
};

struct menuitembar menus_of_bar[] =
{
   {reinterpret_cast<unsigned char const*>("File Manager"), 0, SCAN_NONE, 0},
   {reinterpret_cast<unsigned char const*>("DOS Applications"), 0, SCAN_NONE, dos_applications_menu},
   {reinterpret_cast<unsigned char const*>("About"), 0, SCAN_NONE, 0},
   {reinterpret_cast<unsigned char const*>("Exit"), 0, SCAN_NONE, exit_menu},
   {0}
};

// File Manager Menus

struct menuitem file_menu[] =
{
   {0}
};

struct menuitem edit_menu[] =
{
   {0}
};

struct menuitem view_menu[] =
{
   {0}
};

struct menuitem go_menu[] =
{
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

   // Bar

   window *bar = new window(0, 0, _video_cols, 3);
   wm_register_window(bar);
   bar->set_attributes(window::EMPTY);
   
   menubar *bar_menus= new menubar(0, 0, _video_cols, 1);
   bar_menus->set_menu(menus_of_bar);
   
   bar->add(bar_menus);
   
   // File Manager
   
   window *file_manager = new window(5, 6, 68, 15);
   wm_register_window(file_manager);
   file_manager->set_title((unsigned char *)"File Manager");
   
   menubar *file_manager_menus= new menubar(0, 0, _video_cols, 1);
   file_manager_menus->set_menu(menus_of_file_manager);
   
   file_manager->add(file_manager_menus);
   
   // Draw bar
   
   wm_draw(bar);

   // Run Window Manager

   wm_run();

   // Exit Window Manager

   wm_deinit();

   return 0;
}
