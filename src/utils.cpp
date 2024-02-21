#include "utils.h"

Utils::Utils() {

}

///home/william/Source/Manager/manqmake/untitled/fish.png


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
	    while (fgets(buffer, 32, fp) != NULL)
	        printf("%s", buffer);
	    	ret = buffer[0] - '0';
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
