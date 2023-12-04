// Maz_Protei_locator.cpp: определяет точку входа для приложения.
//

#include "Maz_Protei_locator.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>


using json = nlohmann::json;

int main(int argc, char** argv)
{
	try {
		Locator locator;
	}
	catch (const std::runtime_error& error) {
		std::cout << error.what() << std::endl;
		return 1;
	}

	while (true) {
		
	}
	return 0;
}
