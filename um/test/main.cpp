#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdint>

#include "math_module.h"

int main() {
    MathModule math;
	
    int result = math.add(10, 5);
    std::cout << "10 + 5 = " << result << std::endl;
	
    result = math.subtract(10, 5);
    std::cout << "10 - 5 = " << result << std::endl;
	
    result = math.multiply(10, 5);
    std::cout << "10 * 5 = " << result << std::endl;
	
    double divisionResult = math.divide(10.0, 5.0);
    std::cout << "10 / 5 = " << divisionResult << std::endl;


    int var = 0;
	uint64_t* var_virtaddr = reinterpret_cast<uint64_t*>(&var);
	std::ofstream outFile("addr.txt");
	if (!outFile.is_open()) {
		std::cerr << "Error: Could not open the file for writing." << std::endl;
		return 1;
	}
	outFile << var_virtaddr;
	outFile.close();
	
    while (true) {
        int random_increment = std::rand() % 100; // Generate a random number between 0 and 99
        std::cout << "before inc value=" << std::dec << var << "@" << std::hex << var_virtaddr << std::endl;
        var += random_increment;
        std::cout << "after inc value=" << std::dec << var << "@" << std::hex << var_virtaddr << std::endl;
        std::cin.get(); // Wait for Enter key press
    }

    return 0;
}
//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
