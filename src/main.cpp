#include <gtk/gtk.h>
#include <gdk.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "Manager.h"
//#include <ostream>

#include <ncurses.h>
#include <iostream>



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
    GtkWidget *image;
} gPointerData;

Utils manUtils;

static void on_file_selected(GtkFileChooser *chooser, gpointer data) {
    gchar *filename = gtk_file_chooser_get_filename(chooser);
    g_print("File selected: %s\n", filename);
    g_free(filename);
}

static void open_file_dialog(gpointer user_data) {
	gPointerData* data = (gPointerData*)user_data;

	GtkWidget *dialog = gtk_file_chooser_dialog_new("Copy Artwork Folders",
			GTK_WINDOW(data->window), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Quit",
			GTK_RESPONSE_CANCEL, "Copy", GTK_RESPONSE_ACCEPT,
			NULL);

	gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), TRUE);
	gtk_file_chooser_set_uri(GTK_FILE_CHOOSER(dialog),data->awManager->dirAw.c_str());

	g_signal_connect(dialog, "response", G_CALLBACK(on_file_selected), NULL);
	gtk_widget_show_all(dialog);
}

static gboolean showSplashScreen(gpointer user_data){
	gPointerData* data = (gPointerData*)user_data;
	GtkWidget *image;
	image = gtk_image_new_from_file(data->awManager->dirSplash.c_str());
	gtk_container_add(GTK_CONTAINER(data->window), image);
	gtk_widget_show_all(data->window);
	gtk_window_set_keep_above(GTK_WINDOW(data->window), TRUE);

	return FALSE;
}

static gboolean killAW(gpointer user_data){
	gPointerData* data = (gPointerData*)user_data;
	data->awManager->killPlayer();
}

static gboolean launchArtwork(gpointer user_data){
	gPointerData* data = (gPointerData*)user_data;

	std::cout << "launching: " << data->awManager->currentArtwork.awName << std::endl;
	data->awManager->launchPlayer(data->awManager->currentArtwork);
	data->awManager->is_changing = false;
//	g_timeout_add(guint(200), pollForUpdate, user_data);
	return FALSE;
}

static gboolean showArtworkInfo(gpointer user_data){
	gPointerData* data = (gPointerData*)user_data;

	std::cout << "showing splash: " << data->awManager->currentArtwork.awSplash << std::endl;
//	GdkPixbuf *pixBuf = gdk_pixbuf_new_from_file(data->awManager->currentArtwork.awSplash.c_str(), NULL);
	gtk_image_set_from_pixbuf(GTK_IMAGE(data->image), data->awManager->currentArtwork.splashPixBuf);
//	gtk_container_remove(GTK_CONTAINER(data->window), data->image);
//	gtk_container_add(GTK_CONTAINER(data->window), image);

	gtk_widget_show_all(data->window);
	gtk_window_set_keep_above(GTK_WINDOW(data->window), TRUE);
	killAW(user_data);

	//launch AW
	g_timeout_add(guint(2000), launchArtwork, user_data);

	return FALSE;
}


static gboolean pollForUpdate(gpointer user_data){
	gPointerData *data = (gPointerData*) user_data;

	if (!data->awManager->is_changing) {
		int buttonStat = manUtils.readGPIOfs();
		if (buttonStat == 0) {		//The button has been Pressed
			Manager::ArtworkInfo artwork = data->awManager->getNextAW();
			data->awManager->setAW(artwork);
			showArtworkInfo(user_data);
			data->awManager->is_changing = true;
//			return FALSE;
		}
	}

    return TRUE;
}

std::vector<GdkPixbuf*> buffs;
static void activate(GtkApplication* app, gpointer user_data){

	pollForUpdate(user_data);


    gPointerData* data =  (gPointerData*)user_data;


    //fullscreen
    data->window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (data->window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (data->window), 2160, 3840);
    gtk_window_fullscreen(GTK_WINDOW (data->window));

    //background
    GdkRGBA color;
	gdk_rgba_parse(&color, "black"); // Change "red" to any color you want
	gtk_widget_override_background_color(data->window, GTK_STATE_FLAG_NORMAL, &color);

	//image
	GtkWidget *image;
	image = gtk_image_new_from_file(data->awManager->dirSplash.c_str());
	gtk_container_add(GTK_CONTAINER(data->window), image);
	gtk_widget_show_all(data->window);
//	gtk_window_set_keep_above(GTK_WINDOW(data->window), TRUE);

	//launchAW
	data->awManager->setAW(data->awManager->getNextAW());
	data->awManager->is_changing = true;
	data->image = image;


	g_timeout_add(guint(1000), showArtworkInfo, user_data);
	g_timeout_add(guint(500), pollForUpdate, user_data);
}



int main (int argc,char **argv){
    GtkApplication *app;
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);

    GtkWidget *window = nullptr;

    Manager man;
    Manager *manager = &man;

    gPointerData *data = g_new(gPointerData, 1);
    data->window = window;
    data->awManager = manager;

    g_signal_connect(app, "activate", G_CALLBACK (activate), data);

    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}

