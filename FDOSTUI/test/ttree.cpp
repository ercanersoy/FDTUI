/*
 TTREE.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "fdostui.hpp"
#include <stdio.h>

tree*
tree_build()
{

    treeitem* root= new treeitem(
        reinterpret_cast<unsigned char const*>("Capital"));

    treeitem* state= new treeitem(reinterpret_cast<unsigned char const*>("NY"));
    state->add(reinterpret_cast<unsigned char const*>("Albany"));
    root->add(state);

    state= new treeitem(reinterpret_cast<unsigned char const*>("UT"));
    state->add(reinterpret_cast<unsigned char const*>("Salt Lake City"));
    root->add(state);

    state= new treeitem(reinterpret_cast<unsigned char const*>("NV"));
    state->add(reinterpret_cast<unsigned char const*>("Carson City"));
    root->add(state);

    tree* t= new tree(0, 0, 20, 10);
    t->attach(root);

    return t;
}

int
main()
{

    wm_error error= wm_init();

    if (error)
    {
        printf("Unable to initialze FDOSTUI subsystem\n");
        return error;
    }

    window* win= new window(0, 0, _video_cols, _video_rows);
    wm_register_window(win);
    tree* t= tree_build();
    win->add(t);
    wm_draw(win);
    wm_run();
    wm_deinit();

    return 0;
}
