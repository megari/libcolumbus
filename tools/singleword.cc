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
    GtkWidget *entry;
    GtkListStore *matchStore;
    GtkWidget *matchView;
};

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_main_quit();
    return TRUE;
}

static void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit ();
}

static void textChanged(GtkWidget *widget, gpointer data) {
    app_data *app = (app_data*) data;
    printf("%s\n", gtk_entry_get_text(GTK_ENTRY(app->entry)));
}

void build_gui(app_data &app) {
    GtkWidget *vbox;
    GtkWidget *scroller;
    GtkWidget *quitButton;
    GtkTreeViewColumn *textColumn;
    GtkTreeViewColumn *errorColumn;
    app.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect (app.window, "delete-event",
            G_CALLBACK (delete_event), NULL);
    g_signal_connect (app.window, "destroy",
            G_CALLBACK (destroy), NULL);
    gtk_window_set_default_size(GTK_WINDOW(app.window), 600, 700);
    gtk_window_set_title(GTK_WINDOW(app.window), "Columbus single word search test tool");
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(app.window), vbox);
    app.entry = gtk_entry_new();
    g_signal_connect(app.entry, "changed", G_CALLBACK(textChanged), &app);

    app.matchStore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
    app.matchView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(app.matchStore));
    textColumn = gtk_tree_view_column_new_with_attributes("Match",
            gtk_cell_renderer_text_new(), "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(app.matchView), textColumn);
    errorColumn = gtk_tree_view_column_new_with_attributes("Error",
            gtk_cell_renderer_text_new(), "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(app.matchView), errorColumn);
    scroller = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroller), app.matchView);

    quitButton = gtk_button_new_with_label("Quit");
    g_signal_connect(quitButton, "clicked", G_CALLBACK(destroy), NULL);

    gtk_box_pack_start(GTK_BOX(vbox), app.entry, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), scroller, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), quitButton, FALSE, TRUE, 0);
    gtk_widget_show_all(app.window);
}

void dummy(app_data &app) {
    GtkTreeIter iter;
    // Add a new row to the model
    gtk_list_store_append (app.matchStore, &iter);
    gtk_list_store_set (app.matchStore, &iter,
            0, "something",
            1, 35,
            -1);
}

int main(int argc, char **argv) {
    app_data app;
    gtk_init(&argc, &argv);

    build_gui(app);
    dummy(app);
    gtk_main();
    return 0;
}
