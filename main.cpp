#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "includes/simdjson.cpp"

int pins[11] = {23, 22, 21, 19, 18, 5, 17, 16, 4, 2, 15};

int main () {
    // std::cout << "wifi:" << tweets["wifi"]<< std::endl;
    // std::cout << "mqtt_broker:" << tweets["mqtt_broker"]<< std::endl;

    // auto arr = tweets["button"].get_array();
    // int a = arr.count_elements();
    // std::cout << "button:" << a << std::endl;
    // // std::cout << tweets["button"].at(0)["name"] << "\n";
    // // std::cout << tweets["button"].at(1)["name"] << "\n";
    // // std::cout << tweets["button"].at(2)["name"] << "\n";
    // // std::cout << tweets["button"].at(3)["name"] << "\n";
    // // std::cout << tweets["button"].at(4)["name"] << "\n";
    // // std::cout << tweets["button"].at(5)["name"] << "\n";
    // for(int i = 0; i < a; i++){
    // 	std::cout << i << " ";
    // 	std::cout << tweets["button"].at(5)["name"] << "\n";
    // }
   
	// std::cout << "\n________________________\n";

	//OPEN FILES//
	std::ofstream esp_code;
	esp_code.open ("smart_house_mqtt.ino");

	std::ifstream start_file;
	start_file.open("code_parts/start_of_code.txt");

	// std::ifstream config;
	// config.open("code_parts/config.txt");

	std::ifstream functions;
	functions.open("code_parts/function.txt");

	simdjson::ondemand::parser parser;
   	simdjson::padded_string json = simdjson::padded_string::load("code_parts/config.json");
    simdjson::ondemand::document tweets = parser.iterate(json);

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

	// std::cout << "wifi:" << tweets["wifi"]<< std::endl;
    // std::cout << "mqtt_broker:" << tweets["mqtt_broker"]<< std::endl;

    esp_code << "#define WIFI_SSID\t" << tweets["wifi"]["ssid"] << std::endl;
	esp_code << "#define WIFI_PW\t" << tweets["wifi"]["pasword"] << std::endl;
    esp_code << "#define MQTT_BROKER\t" << tweets["mqtt_broker"]["MQTT_BROKER"] << std::endl;
    esp_code << "#define MQTT_PORT\t" << tweets["mqtt_broker"]["MQTT_PORT"] << std::endl;
    esp_code << "#define MQTT_NAME_OF_DEVICE\t" << tweets["mqtt_broker"]["MQTT_NAME_OF_DEVICE"] << std::endl;
    esp_code << "#define MQTT_LOGIN\t" << tweets["mqtt_broker"]["MQTT_LOGIN"] << std::endl;
    esp_code << "#define MQTT_PW\t" << tweets["mqtt_broker"]["MQTT_PW"] << "\n" << std::endl;

    auto arr = tweets["button"].get_array();
 	std::cout << arr.count_elements() << "\n";
    for(int i = 0; i < arr.count_elements(); i++){
    	std::cout << i << "\n";
    	esp_code << "#define " << tweets["button"].at(i)["name"] << "\t" << pins[i] << "\n";
    }

	// std::vector<std::string> names_of_relay;
	// std::vector<std::string> names_of_buttons;

	// if (esp_code.is_open() && config.is_open()){
	// 	std::cout << "config is opened\n";
	// 	std::string line;
	// 	while ( std::getline(config,line) ){
	// 		std::string type;
	// 		std::string value;

	// 		int i = 0;
	// 		while(line[i] != ' ' && line[i] != ':'){
	// 			if (i >= line.size()){
	// 				std::cout << "i error\n";
	// 				break;
	// 			}
	// 			type += line[i];
	// 			i++;
	// 		}

	// 		while(line[i] != '\"')
	// 			i++;
			
	// 		i++;
	// 		while(line[i] != '\"'){
	// 			value += line[i];
	// 			i++;
	// 		}

	// 		// std::cout << type << "\t" << value << "\n";
	// 		if(type == "relay" || type == "button"){
	// 			if(type == "relay"){
	// 				names_of_relay.push_back(value);
	// 			}else{
	// 				names_of_buttons.push_back(value);
	// 			}
	// 		}else{
	// 			//std::cout << "#define\t" << type << "\t" << value << "\n";
	// 			esp_code << "#define\t" << type << "\t\"" << value << "\"\n";
	// 		}
	// 	}

	// 	// std::cout << "relays\n";
	// 	// for(int i = 0; i < names_of_relay.size(); i++){
	// 	// 	std::cout << i << "\t" << names_of_relay[i] << "\n";
	// 	// }
	// 	// std::cout << "buttons\n";
	// 	// for(int i = 0; i < names_of_buttons.size(); i++){
	// 	// 	std::cout << i << "\t" << names_of_buttons[i] << "\n";
	// 	// }

	// 	esp_code << "\nWiFiClient espClient;\nPubSubClient client(espClient);\n\n";

	// 	for(int i = 0; i < names_of_relay.size(); i++){
	// 		esp_code << "#define\t" << names_of_relay[i] << "\t" << pins[i] << "\n";
	// 	}

	// 	for(int i = 0; i < names_of_buttons.size(); i++){
	// 		esp_code << "#define\t" << names_of_buttons[i] << "\t" << pins[i+names_of_relay.size()] << "\n";
	// 	}

	// }

	// //WRITE SETUP//
	// esp_code << "\n";
	// esp_code << "void setup(){" << '\n';
	// for(int i = 0; i < names_of_relay.size(); i++){
	// 	esp_code << "\tpinMode(" << names_of_relay[i] << ", OUTPUT);\n";
	// }

	// for(int i = 0; i < names_of_buttons.size(); i++){
	// 	esp_code << "\tpinMode(" << names_of_buttons[i] << ", INPUT);\n";
	// }
	// esp_code << "\n\tSerial.begin(115200); connect();\n}\n\n";

	// esp_code << "void loop(){}\n\n";

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
	start_file.close();
	esp_code.close();
	return 0;
}