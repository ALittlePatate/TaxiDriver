#include "memory.hpp"

std::string target_proc_name{"test_app"};
std::string target_mod_name{"libmath_module.so"};

int main() {
    //run_overlay();
    if (!open_device())
		return -1;
    std::cout << "[Main]device opened" << std::endl;
	
    int pid = get_pid(target_proc_name.c_str());
	std::cout << "pid of " << target_proc_name << "=" << std::dec << pid << std::endl;
	if (!open_process(pid))
		return -1;
	
    uint64_t addr;
    std::ifstream inFile("test/addr.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open the file for reading." << std::endl;
        return 1;
    }
	inFile >> std::hex;
    if (inFile >> addr) {
        std::cout << "Read from file var_virtaddr=" << std::hex << addr << std::endl;
    } else {
        std::cerr << "Error: Failed to read value from file." << std::endl;
        return 1;
    }
    inFile.close();
	
#if KERNEL_UPSTREAM==1
	uint64_t mod_primary_addr = get_module(target_proc_name.c_str());
#else
	uint64_t mod_primary_addr = get_pid_module(pid, target_proc_name.c_str());
#endif
	std::cout << "module of " << target_proc_name << "=" << std::hex << mod_primary_addr << std::endl;

#if KERNEL_UPSTREAM==1
	uint64_t modaddr_libmath_addr = get_module(target_mod_name.c_str());
#else
	uint64_t modaddr_libmath_addr = get_pid_module(pid, target_mod_name.c_str());
#endif
	std::cout << "module of " << target_mod_name << "=" << std::hex << modaddr_libmath_addr << std::endl;
	
	//mod_primary_addr = 0; // primary_module not required
    int value_read = RPM<int>(0 + addr);
	std::cout << "Value before write=" << std::dec << value_read << std::endl;
	
    WPM<int>(0 + addr, 1337);
    value_read = RPM<int>(0 + addr);
	std::cout << "Value after write=" << std::dec << value_read << std::endl;

#if KERNEL_UPSTREAM==0 && TESTING==1
	uint64_t physaddr = VIRT_TO_PHYS(0x7fff2a2cb7a4);
	std::cout << "phys addr=" << std::hex << physaddr << std::endl;
	uint64_t phys2virtaddr = PHYS_TO_VIRT(0x7fff2a2cb7a4);
	std::cout << "phys2virtaddr=" << std::hex << phys2virtaddr << std::endl;
	
	//attempt to read relative to proc primary module
	int value_read_primarymod = RPM<int>(mod_primary_addr + 0x2A917D6757A4);
	std::cout << "value_read_primarymod=" << std::dec << value_read_primarymod << std::endl;
#endif
    close_device();
    return 0;
}
//pmap -x $(pidof test_app)
