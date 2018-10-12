/*
 *  common.cpp - Common functions source file
 *  Written by Ercan Ersoy and Mark Olesen.
 */

#include <common.h>
#include <dosshell.h>

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
