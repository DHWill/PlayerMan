#include <gtk/gtk.h>
#include <gdk.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "Manager.h"

#include <ncurses.h>
#include <iostream>


///home/william/Source/Manager/manqmake/untitled/fish.png

/*      POA
 * Get all dirs in /homr/root/player/AW
 * Thread GPIO for change, if change g_timeot_add(10 seconds)??
 * OR if GPIO change OR receive UDP Message trigger PNG, kill prvious ./launcher + dirVid + dirJson
 *
 *
 * Main(){
 * vector AWDirs = getDirsIn(AW)
 * int position = 0
 * whileTrue{
 *  if(GPIO or UDP){
 *      if(UDP){
 *          for n in AWDirs:
 *              if(n.name == UDP){
 *                  showSplashScreen(dir + splash.png)
 *                  exe player (dir + video) (dir + json)
 *              }
 *      }
 *
 *      else{
 *          showSplashScreen(dir + splash.png)
 *          exe player (dir + video) (dir + json)
 *      }
 *  usleep5
 *  }
 *
 */

typedef struct {
    Manager *awManager;
    GtkWidget *window;
} gPointerData;

Utils manUtils;
static gboolean pollForUpdate(gpointer user_data){

	int buttonStat = manUtils.readGPIOfs();
	std::cout << "buttonStat: " << buttonStat << std::endl;

	if(buttonStat == 0){
		gPointerData* data = (gPointerData*)user_data;

		GtkWidget *image;
		image = gtk_image_new_from_file(data->awManager->artworks[0].awSplash.c_str());
		image = gtk_image_new_from_file("/home/root/player/sig.png");

		GdkRGBA color;
		gdk_rgba_parse(&color, "black"); // Change "red" to any color you want
		gtk_widget_override_background_color(data->window, GTK_STATE_FLAG_NORMAL, &color);

		gtk_container_add(GTK_CONTAINER(data->window), image);
		gtk_widget_show_all(data->window);

	}
    //os.launch the video player
    // usleep(2000);
    // gtk_widget_hide(data->window);

    return TRUE;
}

static void activate(GtkApplication* app, gpointer user_data){

    gPointerData* data =  (gPointerData*)user_data;
    data->window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (data->window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (data->window), 2160, 3840);
    gtk_window_fullscreen(GTK_WINDOW (data->window));
    g_timeout_add(guint(200), pollForUpdate, user_data);
}



int main (int argc,char **argv){
    GtkApplication *app;
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);


    GtkWidget *window = nullptr;
    Manager man;
    man.findAWPaths();
    Manager *manager = &man;

    gPointerData *data = g_new(gPointerData, 1);

    data->window = window;
    data->awManager = manager;

    g_signal_connect(app, "activate", G_CALLBACK (activate), data);

    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}
