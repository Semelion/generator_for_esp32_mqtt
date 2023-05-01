#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include <nlohmann/json.hpp>

int pins[11] = {23, 22, 21, 19, 18, 5, 17, 16, 4, 2, 15};
int count_used_pins = 0;

int main () {
	//OPEN FILES//
	std::ofstream esp_code;
	esp_code.open ("smart_house_mqtt.ino");

	std::ifstream start_file;
	start_file.open("code_parts/start_of_code.txt");

	std::ifstream functions;
	functions.open("code_parts/function.txt");

	std::ifstream config_file("code_parts/config.json");
	nlohmann::json config = nlohmann::json::parse(config_file);

	//WRITE COMMENTS AND NOTES IN TOP OF CODE FILE//
	if (esp_code.is_open() && start_file.is_open()){
		//std::cout << "start_file is opened\n";
		std::string line;
		while ( std::getline (start_file,line) ){
			// std::cout << line << "\n";
			esp_code << line << '\n';
		}
	}
	esp_code << "\n";


	//PARCING FROM CONFIG AND WRITE DEFINES//

    esp_code << "#define WIFI_SSID\t" << config["wifi"]["ssid"] << std::endl;
	esp_code << "#define WIFI_PW\t" << config["wifi"]["pasword"] << std::endl;
    esp_code << "#define MQTT_BROKER\t" << config["mqtt_broker"]["MQTT_BROKER"] << std::endl;
    esp_code << "#define MQTT_PORT\t" << config["mqtt_broker"]["MQTT_PORT"] << std::endl;
    esp_code << "#define MQTT_NAME_OF_DEVICE\t" << config["mqtt_broker"]["MQTT_NAME_OF_DEVICE"] << std::endl;
    esp_code << "#define MQTT_LOGIN\t" << config["mqtt_broker"]["MQTT_LOGIN"] << std::endl;
    esp_code << "#define MQTT_PW\t" << config["mqtt_broker"]["MQTT_PW"] << "\n" << std::endl;


    //std::cout << config["button"].size() << "\n";
	for(int i = 0; i < config["button"].size(); i++){
		esp_code << "#define " << config["button"].at(i)["name"] << "\t" << pins[i] << "\n";
		count_used_pins++;
	}

	for(int i = 0; i < config["relay"].size(); i++){
		esp_code << "#define " << config["relay"].at(i)["name"] << "\t" << pins[i + count_used_pins] << "\n";
	}

	esp_code << "WiFiClient espClient;\n";
	esp_code << "PubSubClient client(espClient);\n\n";

	///WRITE SETUP////
	esp_code << "void setup() {\n";
	esp_code << "\tSerial.begin(9600);\n\n";

	for(int i = 0; i < config["button"].size(); i++){
		esp_code << "\tpinMode(" << config["button"].at(i)["name"] << ", INPUT);\n";
	}

	for(int i = 0; i < config["relay"].size(); i++){
		esp_code << "\tpinMode(" << config["relay"].at(i)["name"] << ", OUTPUT);\n";
	}
	esp_code << "}\n";

	// //voids//
	// if (esp_code.is_open() && functions.is_open()){
	// 	std::cout << "function is opened\n";
	// 	std::string line;
	// 	while ( std::getline (functions,line) ){
	// 		// std::cout << line << "\n";
	// 		esp_code << line << '\n';
	// 	}
	// }

	//CLOSE FILES//
	// config.close();
	config_file.close();
	start_file.close();
	esp_code.close();
	return 0;
}