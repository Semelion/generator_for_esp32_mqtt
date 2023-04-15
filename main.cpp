#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <json/value.h>

int pins[11] = {23, 22, 21, 19, 18, 5, 17, 16, 4, 2, 15};

int main () {
	std::ifstream people_file("config.json", std::ifstream::binary);
	Json::Value config_json;
	people_file >> config_json;

	std::cout<<config_json;
	std::cout << "\n________________________\n"

	//OPEN FILES//
	std::ofstream esp_code;
	esp_code.open ("smart_house_mqtt.ino");

	std::ifstream start_file;
	start_file.open("start_of_code.txt");

	std::ifstream config;
	config.open("config.txt");

	std::ifstream functions;
	functions.open("function.txt");

	//WRITE COMMENTS AND NOTES IN TOP OF CODE FILE//
	if (esp_code.is_open() && start_file.is_open()){
		std::cout << "start_file is opened\n";
		std::string line;
		while ( std::getline (start_file,line) ){
			// std::cout << line << "\n";
			esp_code << line << '\n';
		}
	}
	esp_code << "\n";


	//PARCING FROM CONFIG AND WRITE DEFINES//
	std::vector<std::string> names_of_relay;
	std::vector<std::string> names_of_buttons;

	if (esp_code.is_open() && config.is_open()){
		std::cout << "config is opened\n";
		std::string line;
		while ( std::getline(config,line) ){
			std::string type;
			std::string value;

			int i = 0;
			while(line[i] != ' ' && line[i] != ':'){
				if (i >= line.size()){
					std::cout << "i error\n";
					break;
				}
				type += line[i];
				i++;
			}

			while(line[i] != '\"')
				i++;
			
			i++;
			while(line[i] != '\"'){
				value += line[i];
				i++;
			}

			// std::cout << type << "\t" << value << "\n";
			if(type == "relay" || type == "button"){
				if(type == "relay"){
					names_of_relay.push_back(value);
				}else{
					names_of_buttons.push_back(value);
				}
			}else{
				//std::cout << "#define\t" << type << "\t" << value << "\n";
				esp_code << "#define\t" << type << "\t\"" << value << "\"\n";
			}
		}

		// std::cout << "relays\n";
		// for(int i = 0; i < names_of_relay.size(); i++){
		// 	std::cout << i << "\t" << names_of_relay[i] << "\n";
		// }
		// std::cout << "buttons\n";
		// for(int i = 0; i < names_of_buttons.size(); i++){
		// 	std::cout << i << "\t" << names_of_buttons[i] << "\n";
		// }

		esp_code << "\nWiFiClient espClient;\nPubSubClient client(espClient);\n\n";

		for(int i = 0; i < names_of_relay.size(); i++){
			esp_code << "#define\t" << names_of_relay[i] << "\t" << pins[i] << "\n";
		}

		for(int i = 0; i < names_of_buttons.size(); i++){
			esp_code << "#define\t" << names_of_buttons[i] << "\t" << pins[i+names_of_relay.size()] << "\n";
		}

	}

	//WRITE SETUP//
	esp_code << "\n";
	esp_code << "void setup(){" << '\n';
	for(int i = 0; i < names_of_relay.size(); i++){
		esp_code << "\tpinMode(" << names_of_relay[i] << ", OUTPUT);\n";
	}

	for(int i = 0; i < names_of_buttons.size(); i++){
		esp_code << "\tpinMode(" << names_of_buttons[i] << ", INPUT);\n";
	}
	esp_code << "\n\tSerial.begin(115200); connect();\n}\n\n";

	esp_code << "void loop(){}\n\n";

	//voids//
	if (esp_code.is_open() && functions.is_open()){
		std::cout << "function is opened\n";
		std::string line;
		while ( std::getline (functions,line) ){
			// std::cout << line << "\n";
			esp_code << line << '\n';
		}
	}

	//CLOSE FILES//
	config.close();
	start_file.close();
	esp_code.close();
	return 0;
}