#include "utils.h"

Utils::Utils() {

}

///home/william/Source/Manager/manqmake/untitled/fish.png
/*
 * 	   killall helloworld-debug &
	   killall helloworld &
 *
 * 	   $SPLASH_APP $folder"/"${directories[INDEX]}"/splash.jpg" 1 &
	   $PLAYER_APP $folder"/"${directories[INDEX]}"/Video.mp4" $folder"/"${directories[INDEX]}"/video_states.json" & >/dev/null
	   PLAYER_APP="/home/root/player/launcher.sh"

	    //Kill cursor (Just unplug mouse)
//    GdkCursor* Cursor = gdk_cursor_new(GDK_BLANK_CURSOR);
//	GdkWindow* win = gtk_widget_get_window((data->window));
//	gdk_window_set_cursor((win),Cursor);
 *
 */


#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

//int Utils::dirStatus() {
//    std::string path = "path_to_your_file_or_directory";
//
//    // Check if the path exists
//    if (fs::exists(path)) {
//        // Check if it's a regular file
//        if (fs::is_regular_file(path)) {
//            std::cout << "The path refers to a regular file." << std::endl;
//        }
//        // Check if it's a directory
//        else if (fs::is_directory(path)) {
//            std::cout << "The path refers to a directory." << std::endl;
//        }
//        // If it's neither a regular file nor a directory
//        else {
//            std::cout << "The path exists but is neither a regular file nor a directory." << std::endl;
//        }
//    } else {
//        std::cout << "The path does not exist." << std::endl;
//    }
//
//    return 0;
//}

//void Utils::chechDirStatus(const std::filesystem::path& p, std::filesystem::file_status s){
//    std::cout << p;
//    // alternative: switch(s.type()) { case fs::file_type::regular: ...}
//    if (std::filesystem::is_regular_file(s)){
//        std::cout << " is a regular file\n";
//    }
//    if (std::filesystem::is_directory(s)){
//        std::cout << " is a directory\n";
//    }
//    if (std::filesystem::is_block_file(s)){
//        std::cout << " is a block device\n";
//    }
//    if (std::filesystem::is_character_file(s))
//    {
//        std::cout << " is a character device\n";
//    }
//    if (std::filesystem::is_fifo(s)){
//        std::cout << " is a named IPC pipe\n";
//    }
//    if (std::filesystem::is_socket(s)){
//        std::cout << " is a named IPC socket\n";
//    }
//    if (std::filesystem::is_symlink(s)){
//        std::cout << " is a symlink\n";
//    }
//    if (!std::filesystem::exists(s)){
//        std::cout << " does not exist\n";
//    }
//}


bool Utils::dirStatus(const char* _pathname) {
	bool status;
//	const char* pathname = _pathname.c_str();
	struct stat info;

	if (stat(_pathname, &info) != 0) {
		printf("cannot access %s\n", _pathname);
		status = false;

	} else if (info.st_mode & S_IFDIR) {
		printf("%s is a directory\n", _pathname);
		status = true;
	} else {
		printf("%s is no directory\n", _pathname);
		status = false;
	}
	return status;
}




void Utils::pollGPIO(){
    const std::string GPIO_PATH = "/sys/class/gpio/gpioXX";  // Replace XX with the GPIO
    // Export the GPIO pin (if not already exported)
    std::ofstream exportFile("/sys/class/gpio/export");
    exportFile << "XX";  // Replace XX with the GPIO number
    exportFile.close();

    // Set the GPIO direction to input
    std::ofstream directionFile(GPIO_PATH + "/direction");
    directionFile << "in";
    directionFile.close();

    // Open the value file for reading
    std::ifstream valueFile(GPIO_PATH + "/value");

    // Set up poll structure
    struct pollfd pollFd;
    pollFd.fd = valueFile.rdstate();
    pollFd.events = POLLPRI;  // Trigger on rising edge

    // Read initial value
    char currentValue;
    valueFile >> currentValue;

    std::cout << "Initial value: " << currentValue << std::endl;

    while (true) {
        // Wait for an event (edge detection)
        int ret = poll(&pollFd, 1, -1);

        if (ret > 0) {
            // Event occurred, read new value
            valueFile.seekg(0);
            valueFile >> currentValue;

            // Handle the GPIO change
            std::cout << "GPIO value changed: " << currentValue << std::endl;
        } else {
            std::cerr << "Error in poll()" << std::endl;
            break;
        }

        // You can add additional logic or break conditions as needed
    }

    // Cleanup (unexport the GPIO pin)
    std::ofstream unexportFile("/sys/class/gpio/unexport");
    unexportFile << "XX";  // Replace XX with the GPIO number
    unexportFile.close();
}

int Utils::readGPIOfs(){
	FILE *fp;
	char buffer[32];
	int ret = -1;
	fp = popen("gpioget 2 8", "r");
	if (fp != NULL)
	{
	    while (fgets(buffer, 32, fp) != NULL){
//	        printf("%s", buffer);
	    	ret = buffer[0] - '0';
	    }
	    pclose(fp);
	}
	return ret;
}

int Utils::readADC_MV(){
	FILE *fp;
	char buffer[32];
	int ret = -1;
	fp = popen("cat /dev/apalis-adc0", "r");
	if (fp != NULL)
	{
	    while (fgets(buffer, 32, fp) != NULL){
//	        printf("%s", buffer);
	    	ret = buffer[0] - '0';
	    }
	    pclose(fp);
	}
	return ret;

}


void Utils::readFile(std::string file){
    // Create a text string, which is used to output the text file
    std::string myText;

    // Read from the text file
    std::ifstream MyReadFile(file);

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
        // Output the text from the file
        std::cout << myText;
    }

    // Close the file
    MyReadFile.close();
}

std::string Utils::runCmd(const std::string& command)
{
    char psBuffer[512];
    FILE*   pPipe;
    std::string result;

    if ((pPipe = popen(command.c_str(), "r")) == NULL) {
        return "not here";
    }
    while (fgets(psBuffer, sizeof(psBuffer), pPipe) != NULL)
        result += psBuffer;

    if(!feof(pPipe)){
        std::cout << "non" << std::endl;
    }

    pclose(pPipe);
    return result;
}

int Utils::runSystem(std::string command)
{
    return system(command.c_str());
}

std::vector<std::string> Utils::getDirsInPath(const char* path){
    // const char* PATH = "/home/william/";
    std::vector<std::string> paths;
    GDir *dir;
    GError *error;
    const gchar *filename;

    dir = g_dir_open(path, 0, &error);
    while ((filename = g_dir_read_name(dir))){
        paths.push_back(std::string(filename));
        std::cout << std::string(filename) << std::endl;
    }
    return paths;
}

bool Utils::doesFileExist(const std::string& name){
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}
