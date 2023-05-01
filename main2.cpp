#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "includes/json.cpp"

int main(){
	std::ifstream f("code_parts/config.json");
	nlohmann::json data = nlohmann::json::parse(f);


	std::cout << data["button"].size() << "\n";
	for(int i = 0; i < data["button"].size(); i++){
		std::cout << data["button"].at(i)["name"] << "\n";
	}
}