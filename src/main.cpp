#include <gtk/gtk.h>
#include <gdk.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "Manager.h"
#include <filesystem>
#include "Networking/Networking.h"

#include <ncurses.h>
#include <iostream>

typedef struct {
    Manager *awManager;
    NetworkingMan *networkingMan;
    GtkWidget *window;
    GtkWidget *image;
} gPointerData;

typedef struct {
	gchar *message;
	GtkDialog *dialog;
} gMessageDialogue;

Utils manUtils;

static gboolean killAW(gpointer user_data){
	gPointerData* data = (gPointerData*)user_data;
	data->awManager->killPlayer();
	return FALSE;
}

static gboolean launchArtwork(gpointer user_data){
	gPointerData* data = (gPointerData*)user_data;
	if(!data->awManager->is_setup){
		std::cout << "launching: " << data->awManager->currentArtwork.awName << std::endl;
		data->awManager->launchPlayer(data->awManager->currentArtwork);
		data->awManager->is_changing = false;
		data->awManager->is_setup = false;
		return FALSE;
	}
	return FALSE;
}

static gboolean showArtworkInfo(gpointer user_data){
	gPointerData* data = (gPointerData*)user_data;

	std::cout << "showing splash: " << data->awManager->currentArtwork.awSplash << std::endl;
//	gtk_widget_show_all(data->window);
	gtk_image_set_from_pixbuf(GTK_IMAGE(data->image), data->awManager->currentArtwork.splashPixBuf);
	gtk_window_set_keep_above(GTK_WINDOW(data->window), TRUE);
	killAW(user_data);
//	gtk_window_set_keep_above(GTK_WINDOW(data->window), TRUE);
//	gtk_widget_show_all(data->window);

	//launch AW wait for kill
	g_timeout_add(guint(2000), launchArtwork, user_data);

	return FALSE;
}

static void open_message_dialog(gpointer user_data) {
	gMessageDialogue* data = (gMessageDialogue*)user_data;
	GtkWidget *message_dialog = gtk_message_dialog_new(GTK_WINDOW(data->dialog),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,data->message);
	gtk_dialog_run(GTK_DIALOG(message_dialog));
	gtk_widget_destroy (GTK_WIDGET(message_dialog));
}

//static gboolean startListenerThread(gpointer user_data){
//	gPointerData* data = (gPointerData*) user_data;
////	if(data->networkingMan->networkListenerThread && data->networkingMan->networkListenerThread->joinable()){
////		data->networkingMan->networkListenerThread->join();
////		data->networkListenerThread->reset(new std::thread(&Networking::udpListener, data->networkingMan));
////	}
//
//	return TRUE;
//}

static gboolean pollForUpdate(gpointer user_data){
	gPointerData *data = (gPointerData*) user_data;
	if(data->awManager->hasPaths){
		if (!data->awManager->is_changing && !data->awManager->is_setup) {
			if(data->networkingMan->isMessageReceived()){
				NetworkingMan::awInfo _awInfo;
				_awInfo = data->networkingMan->receivedMessage();
				std::cout << "GroupName: "<<_awInfo.groupName << std::endl;
				std::cout << "AWPath: "<<_awInfo.awPath << std::endl;
				Manager::ArtworkInfo artwork = data->awManager->getAW(_awInfo.awPath);
				data->awManager->setAW(artwork);
				showArtworkInfo(user_data);
				data->awManager->is_changing = true;
				data->networkingMan->startListening();
			}
			if (manUtils.readGPIOfs() == 0) {		//The button has been Pressed
				Manager::ArtworkInfo artwork = data->awManager->getNextAW();
				data->awManager->setAW(artwork);
				showArtworkInfo(user_data);
				data->awManager->is_changing = true;
			}
		}

	}
    return TRUE;
}

static void on_file_selected(GtkFileChooser *chooser, gint response_id, gpointer user_data) {
	gPointerData *data = (gPointerData*) user_data;
	gchar *filename = gtk_file_chooser_get_filename(chooser);

	gMessageDialogue *messageDialogue = g_new(gMessageDialogue, 1);
	messageDialogue->dialog = GTK_DIALOG(chooser);
	std::string _message = "";

	//This is copy folder
	if (response_id == GTK_RESPONSE_ACCEPT) {
		if (data->awManager->copyFiles(filename)){
			_message = "Copied " + std::string(filename) + " to: " + data->awManager->dirAw;
//			data->awManager->makeFolderExecutable(std::string(filename));
		} else {
			_message = "Error in Copying " + std::string(filename) + " to: " + data->awManager->dirAw + "cleaning up.. ";
			std::string awCopyPath = std::string(filename);
			data->awManager->cleanUpFalseCopy(awCopyPath);
		}
		data->awManager->findAWPaths();
		messageDialogue->message = (gchar*) _message.c_str();
		open_message_dialog(messageDialogue);
	}

	//This is delete folder
	else if (response_id == GTK_RESPONSE_CANCEL) {
		if (data->awManager->removeFiles(filename)) {
			_message = "Removed " + std::string(filename);
		} else {
			_message = "Could NOT Remove " + std::string(filename);
		}
		messageDialogue->message = (gchar*) _message.c_str();
		open_message_dialog(messageDialogue);
	}

	//This is close file dialogue
	else if (response_id == GTK_RESPONSE_DELETE_EVENT) {;
		if(data->awManager->findAWPaths()){
			_message = "Closing Artwork Copier, Launching Player..";
			messageDialogue->message = (gchar*) _message.c_str();
			open_message_dialog(messageDialogue);
			data->awManager->setAW(data->awManager->getNextAW());
			data->awManager->is_changing = true;
			g_timeout_add(guint(1000), showArtworkInfo, user_data);	//Launch AW on close
		}
		else{
			_message = "No Artworks in AW Directory.. Press Shift + S";
			messageDialogue->message = (gchar*) _message.c_str();
			open_message_dialog(messageDialogue);
		}
		data->awManager->is_setup = false;
		gtk_widget_show_all(data->window);	//to kepp keybind open
	}
	g_free(filename);
}

static void open_file_dialog(gpointer user_data) {
	gPointerData* data = (gPointerData*)user_data;
	GtkWidget *dialog = gtk_file_chooser_dialog_new("Copy or Remove Artwork Folders",
			GTK_WINDOW(data->window), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Delete",
			GTK_RESPONSE_CANCEL, "Copy", GTK_RESPONSE_ACCEPT,
			NULL);
	gtk_file_chooser_remove_shortcut_folder(GTK_FILE_CHOOSER(dialog), "/home/root/", NULL);
	gtk_file_chooser_remove_shortcut_folder(GTK_FILE_CHOOSER(dialog), "recent", NULL);
	gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(dialog), data->awManager->dirAw.c_str(), NULL);
	gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(dialog), data->awManager->dirAwMSata.c_str(), NULL);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),data->awManager->dirAw.c_str());

	g_signal_connect(dialog, "response", G_CALLBACK(on_file_selected), user_data);
	gtk_widget_show_all(dialog);
}

void key_press_event(  GtkEventControllerKey* self,guint keyval,guint keycode,GdkModifierType* state,gpointer user_data){
	gPointerData* data = (gPointerData*)user_data;
	if((keyval == GDK_KEY_S) && (!data->awManager->is_setup)){
		killAW(user_data);
		open_file_dialog(user_data);
		data->awManager->is_setup = true;
	}
}

static void activate(GtkApplication* app, gpointer user_data){

	pollForUpdate(user_data);
    gPointerData* data =  (gPointerData*)user_data;

    //Fullscreen
    data->window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (data->window), "AW");
    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()),&workarea);
    gtk_window_set_default_size (GTK_WINDOW (data->window), workarea.width, workarea.height);
//    gtk_window_set_default_size (GTK_WINDOW (data->window), 1280, 1024);
    gtk_window_fullscreen(GTK_WINDOW (data->window));

    //Background
    GdkRGBA color;
	gdk_rgba_parse(&color, "black");
	gtk_widget_override_background_color(data->window, GTK_STATE_FLAG_NORMAL, &color);

	//Keybind
	GtkEventController *event_controller;
	event_controller = gtk_event_controller_key_new(data->window);
	g_signal_connect(event_controller, "key-pressed",G_CALLBACK(key_press_event), data);

	//Create Image for PixBuff
	GtkWidget *image;
	image = gtk_image_new_from_file(data->awManager->dirSplash.c_str());
	gtk_container_add(GTK_CONTAINER(data->window), image);
	data->awManager->tools.runSystem("gpioset 2 8=0");	//Turn on Backlight
	data->image = image;

	if(data->awManager->hasPaths && (!data->awManager->is_setup)){
		data->awManager->setAW(data->awManager->getNextAW());
		data->awManager->is_changing = true;
		g_timeout_add(guint(1000), showArtworkInfo, user_data);
		gtk_widget_show_all(data->window);
		data->networkingMan->startListening();
	}
	else{
		open_file_dialog(user_data);
	}
	g_timeout_add(guint(500), pollForUpdate, user_data);
}



int main (int argc,char **argv){
    GtkApplication *app;
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);

    GtkWidget *window = nullptr;

    Manager man;
    Manager *manager = &man;

    NetworkingMan networking;
//    networking.startListening();

    gPointerData *data = g_new(gPointerData, 1);


    NetworkingMan *netManager = &networking;
    data->networkingMan = netManager;
    data->window = window;
    data->awManager = manager;
//	data->networkingMan->startListening();


    g_signal_connect(app, "activate", G_CALLBACK (activate), data);

    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}

