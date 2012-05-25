/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * A simple GUI application to search from a list of single words.
 */

#include <gtk/gtk.h>
#include "LevenshteinIndex.hh"

struct app_data {
    LevenshteinIndex ind;
    GtkWidget *window;
};

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_main_quit();
    return TRUE;
}

static void destroy(GtkWidget *widget, gpointer data ) {
    gtk_main_quit ();
}

void build_gui(app_data &app) {
    app.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect (app.window, "delete-event",
            G_CALLBACK (delete_event), NULL);
    g_signal_connect (app.window, "destroy",
            G_CALLBACK (destroy), NULL);
    gtk_widget_show_all(app.window);
}

int main(int argc, char **argv) {
    app_data app;
    gtk_init(&argc, &argv);

    build_gui(app);
    gtk_main();
    return 0;
}
