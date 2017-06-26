#include <fdostui.hpp>
#include <stdio.h>

struct menuitem dos_applications_menu[] =
{
   {0}
};

struct menuitem exit_menu[] =
{
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

int main(void)
{
   wm_error error= wm_init();

   if (error)
   {
      puts("Unable to initialze FDOSTUI subsystem.");
      return error;
   }

   window *bar = new window(0, 0, _video_cols, 3);
   wm_register_window(bar);
   bar->set_attributes(window::EMPTY);
   
   menubar* bar_menus= new menubar(0, 0, _video_cols, 1);
   bar_menus->set_menu(menus_of_bar);
   bar->add(bar_menus);
   
   wm_draw(bar);

   wm_run();

   wm_deinit();

   return 0;
}
