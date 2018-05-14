/*********************************
 * FreeDOS TUI Shell Source File *
 *********************************/

#include <direct.h>
#include <mouse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fdostui.hpp>
#include "config.h"
#include "lang\en.h"
#include "lang\tr.h"

// Current Directory

char *current_directory = (char *)malloc(MAX_PATH_LENGTH);

// Bar

window *bar = new window(0, 0, _video_cols, 3);
menubar *bar_menus = new menubar(0, 0, _video_cols, 1);

// File Manager

window *file_manager = new window(5, 6, 68, 17);
menubar *file_manager_menus = new menubar(0, 0, _video_cols, 1);
label *current_directory_label = new label(1, 2, 65, 1);
listbox *drivers = new listbox(1, 3, 65, 3);
listbox *directories_and_files = new listbox(1, 6, 65, 9);

// Common Functions

void command_run(char const* command)
{
   mouse_deinit();
   system(command);
   mouse_init();
   mouse_show();
   wm_draw(0);
}

void directory_view(menuitem *, void *)
{
   struct dirent *element;

   directories_and_files->remove_all();

   DIR *directory = opendir(current_directory);

   while(element = readdir(directory))
   {
      if(!strcmp(element->d_name, ".") || !strcmp(element->d_name, ".."))
      {
         continue;
      }

      directories_and_files->add(reinterpret_cast<unsigned char const*>(element->d_name));
   }
}

// Bar Menu Functions

void application_file_manager(menuitem *, void *)
{
   if(false == file_manager->get_visible())
   {
      file_manager->set_visible();

      directory_view(0, 0);

      wm_draw(file_manager);
   }
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
   {reinterpret_cast<unsigned char const*>(STRING_FILE_MANAGER), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, application_file_manager, 0},
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
   {reinterpret_cast<unsigned char const*>(STRING_QUIT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, dosshell_quit, 0},
   {reinterpret_cast<unsigned char const*>(STRING_REBOOT_COMPUTER), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, reboot, 0},
   {reinterpret_cast<unsigned char const*>(STRING_POWEROFF_COMPUTER), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, poweroff, 0},
   {0}
};

struct menuitembar menus_of_bar[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_INTERNAL_APPLICATIONS), 0, SCAN_NONE, internal_applications_menu},
   {reinterpret_cast<unsigned char const*>(STRING_DOS_APPLICATIONS), 0, SCAN_NONE, dos_applications_menu},
   {reinterpret_cast<unsigned char const*>(STRING_EXIT), 0, SCAN_NONE, exit_menu},
   {0}
};

// Menu Function Menus

void file_manager_exit(menuitem *, void *)
{
   file_manager->set_hidden();
}

// File Manager Menus

struct menuitem file_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_OPEN), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_NEW_DIRECTORY), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_EXIT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, file_manager_exit, 0},
   {0}
};

struct menuitem edit_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_CUT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_COPY), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_PASTE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_DELETE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {0}
};

struct menuitem view_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_REFRESH), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, directory_view, 0},
   {reinterpret_cast<unsigned char const*>(STRING_SHOW_HIDDEN_FILES), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX | MENUITEM_SEPERATOR, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_SORT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE,  MENUITEM_SUBMENU, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_BY_NAME), MENUITEM_MNEMONIC_NONE, 0,  SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_BY_SIZE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_BY_TYPE), MENUITEM_MNEMONIC_NONE, 0,  SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_BY_DATE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {0},
   {0}
};

struct menuitem go_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_BACK), MENUITEM_MNEMONIC_NONE, 0,  SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_FOWARD), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {reinterpret_cast<unsigned char const*>(STRING_UP), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, 0, 0},
   {0}
};
struct menuitembar menus_of_file_manager[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_FILE), 0, SCAN_NONE, file_menu},
   {reinterpret_cast<unsigned char const*>(STRING_EDIT), 0, SCAN_NONE, edit_menu},
   {reinterpret_cast<unsigned char const*>(STRING_VIEW), 0, SCAN_NONE, view_menu},
   {reinterpret_cast<unsigned char const*>(STRING_GO), 0, SCAN_NONE, go_menu},
   {0}
};

// Main Function

int main(void)
{
   wm_error error= wm_init();

   if (error)
   {
      puts(STRING_UNABLE_TO_INITIALZE_FDOSTUI_SYSTEM);
      return error;
   }

   // Current Directory
   
   current_directory = (char *) malloc(PATH_MAX + 1);
   getcwd(current_directory, PATH_MAX + 1);

   // Bar

   wm_register_window(bar);

   bar_menus->set_menu(menus_of_bar);

   bar->set_attributes(window::EMPTY);
   bar->add(bar_menus);

   // File Manager

   wm_register_window(file_manager);

   file_manager_menus->set_menu(menus_of_file_manager);

   current_directory_label->set_text((unsigned char *)current_directory);

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

   return 0;
}
