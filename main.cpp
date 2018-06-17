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

char *current_directory = (char *)malloc(MAX_PATH_LENGTH + 1);
bool show_hidden_file = false;
char *clipboard_item_name = (char *)calloc(256, 1);
char *clipboard_item_path = (char *)calloc(MAX_PATH_LENGTH, 1);
unsigned char clipboard_status = 0;  // 0 for nothing, 1 for copying, 2 for cutting.

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

   DIR *directory = opendir(getcwd(current_directory, PATH_MAX + 1));

   current_directory_label->set_text((unsigned char *)current_directory);

   while(element = readdir(directory))
   {
      if(!strcmp(element->d_name, "."))
      {
         continue;
      }
      else if(element->d_attr == _A_HIDDEN && !show_hidden_file)
      {
         continue;
      }

      directories_and_files->add(reinterpret_cast<unsigned char const*>(element->d_name));
   }
}

void copy_file(char *file, char *target)
{
   FILE *source_file, *target_file;
   char buffer = 0;

   source_file = fopen(file, "rb");
   target_file = fopen(target, "wb");

   while((buffer = fgetc(source_file)) != 255 && !feof(source_file))
   {
      fputc(buffer, target_file);
   }

   fclose(target_file);
   fclose(source_file);
}

void copy_directory(char *directory, char *target)
{
   DIR *element;
   struct dirent *item_properties;
   char *item_name = (char *) malloc(256);
   char *item_path = (char *) malloc(MAX_PATH_LENGTH + 1);
   char *target_item_path = (char *) malloc(MAX_PATH_LENGTH + 1);
   struct stat status;

   mkdir(target);

   element = opendir(directory);
   item_properties = readdir(element);
   item_name = item_properties->d_name;

   while(item_name[0] != 1)
   {
      if(strcmp(item_name, ".") && strcmp(item_name, "..") && (strlen(item_path) + strlen(target_item_path) <= (MAX_PATH_LENGTH + 1) * 2))
      {
         strcpy(item_path, directory);
         strcat(item_path, "\\");
         strcat(item_path, item_name);

         strcpy(target_item_path, target);
         strcat(target_item_path, "\\");
         strcat(target_item_path, item_name);

         stat(item_path, &status);
         if(S_ISDIR(status.st_mode))
         {
            copy_directory(item_path, target_item_path);
         }
         else
         {
            copy_file(item_path, target_item_path);
         }
      }
      
      item_properties = readdir(element);
      item_name = item_properties->d_name;
      
      *item_path = '\0';
      *target_item_path = '\0';
   }

   closedir(element);
   free(item_name);
   free(item_path);
   free(target_item_path);
}

char remove_item(char *removing_item_path)
{
   DIR *element;
   struct dirent *item_properties;
   char *item_name = (char *) malloc(256);
   char *item_path = (char *) malloc(260);

   element = opendir(removing_item_path);
   item_properties = readdir(element);
   item_name = item_properties->d_name;

   while(item_name[0] != 1)
   {
      if(strcmp(item_name, ".") && strcmp(item_name, "..") && (strlen(item_path) + strlen(removing_item_path) <= 258))
      {
         strcpy(item_path, removing_item_path);
         strcat(item_path, "\\");
         strcat(item_path, item_name);
         if(remove(item_path))
         {
            remove_item(item_path);
         }
      }
      
      item_properties = readdir(element);
      item_name = item_properties->d_name;
      
      *item_path = '\0';
   }
   
   closedir(element);
   free(item_name);
   free(item_path);
   
   return rmdir(removing_item_path);
}

// Bar Menu Functions

void application_file_manager(menuitem *, void *)
{
   if(!file_manager->get_visible())
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

void item_open(menuitem *, void *)
{
   char *item_name = (char *)directories_and_files->get_item(directories_and_files->get_selected_first());
   struct stat status;

   stat(item_name, &status);
   if(S_ISDIR(status.st_mode))
   {
      chdir(item_name);

      directory_view(0, 0);
   }
   else
   {
      command_run(item_name);
   }
}

void new_directory(menuitem *, void *)
{
   mkdir((char *)popup_input(reinterpret_cast<unsigned char const*>(STRING_NEW_DIRECTORY), reinterpret_cast<unsigned char const*>(STRING_DIRECTORY_NAME), reinterpret_cast<unsigned char const*>("")));

   directory_view(0, 0);
}

void cut_item(menuitem *, void *)
{
   strcat(clipboard_item_name, (char *)directories_and_files->get_item(directories_and_files->get_selected_first()));
   strcat(clipboard_item_path, current_directory);
   strcat(clipboard_item_path, "\\");
   strcat(clipboard_item_path, clipboard_item_name);
   clipboard_status = 2;
}

void copy_item(menuitem *, void *)
{
   strcat(clipboard_item_name, (char *)directories_and_files->get_item(directories_and_files->get_selected_first()));
   strcat(clipboard_item_path, current_directory);
   strcat(clipboard_item_path, "\\");
   strcat(clipboard_item_path, clipboard_item_name);
   clipboard_status = 1;
}

void paste_item(menuitem *, void *)
{
   char *target_path = current_directory;
   struct stat status;

   strcat(target_path, "\\");
   strcat(target_path, clipboard_item_name);

   if(clipboard_status == 1)
   {
      stat(clipboard_item_path, &status);
      if(S_ISDIR(status.st_mode))
      {
         copy_directory(clipboard_item_path, target_path);
      }
      else
      {
         copy_file(clipboard_item_path, target_path);
      }
      
   }
   else if(clipboard_status == 2)
   {
      rename(clipboard_item_path, target_path);
   }

   clipboard_status = 0;
   *clipboard_item_name = '\0';
   *clipboard_item_path = '\0';
   
   directory_view(0, 0);
}

void delete_item(menuitem *, void *)
{
   char *item = (char *)directories_and_files->get_item(directories_and_files->get_selected_first());

   if(remove(item))
   {
      remove_item(item);
   }
   
   directory_view(0, 0);
}

void file_manager_exit(menuitem *, void *)
{
   file_manager->set_hidden();
}

void showing_hidden_files(struct menuitem *menu_item, void *)
{
   if(menu_item->m_flags & MENUITEM_VALUE)
   {
      show_hidden_file = true;
   }
   else
   {
      show_hidden_file = false;
   }
   
   directory_view(0, 0);
}

// File Manager Menus

struct menuitem file_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_OPEN), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, item_open, 0},
   {reinterpret_cast<unsigned char const*>(STRING_NEW_DIRECTORY), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, new_directory, 0},
   {reinterpret_cast<unsigned char const*>(STRING_EXIT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, file_manager_exit, 0},
   {0}
};

struct menuitem edit_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_CUT), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, cut_item, 0},
   {reinterpret_cast<unsigned char const*>(STRING_COPY), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, copy_item, 0},
   {reinterpret_cast<unsigned char const*>(STRING_PASTE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, paste_item, 0},
   {reinterpret_cast<unsigned char const*>(STRING_DELETE), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, 0, delete_item, 0},
   {0}
};

struct menuitem view_menu[] =
{
   {reinterpret_cast<unsigned char const*>(STRING_REFRESH), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_SEPERATOR, directory_view, 0},
   {reinterpret_cast<unsigned char const*>(STRING_SHOW_HIDDEN_FILES), MENUITEM_MNEMONIC_NONE, 0, SCAN_NONE, MENUITEM_CHECKBOX | MENUITEM_SEPERATOR, showing_hidden_files, 0},
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
   free(clipboard_item_name);
   free(clipboard_item_path);

   return 0;
}
