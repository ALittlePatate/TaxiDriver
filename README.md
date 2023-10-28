# TaxiDriver
Linux Kernel Module for RPM/WPM and usermode driver with examples.

## Features
* Get the base address of any process/loaded .so file
* RPM
* WPM

## Compiling
### Prerequises :
```
sudo pacman -S git linux-headers OpenGL cmake
yay -S sdl3-git
```
### Upstream Linux Kernel
To compile the driver you'll need to be at least on kernel version v6.5 (i used the [arch kernel](https://github.com/archlinux/linux/releases/tag/v6.5.8-arch1)).<br>
### legacy Linux Kernel
To build for an older Linux Kernel ex. 5.15 change KERNEL_UPSTREAM preprocessor definition
in TaxiDriver.h:
`#define KERNEL_UPSTREAM 0`
or if you use cmake only change required in CmakeLists.txt:
`add_compile_definitions(TaxiDriver PRIVATE KERNEL_UPSTREAM=0)`

### Compiling and running :
```
git clone --recursive https://github.com/ALittlePatate/TaxiDriver
```
#### Kernel Module
choose and write a MAJOR number ex. 506 in install.sh and validate for uniqueness `grep "506" /proc/devices`
```
# sudo mknod /dev/TaxiDriver c <choose Major Number> 0
# cmake is still incomplete
cd km && ./install.sh
```
system might still choose to set a different one, so read the number that is in use, after TaxiDriver is loaded,
with `grep "TaxiDriver" /proc/devices` and adjust in mknod and reload TaxiDriver.
After loading the Driver, you should find a line saying that TaxiDriver was loaded with X major code.
```
sudo dmesg | grep TaxiDriver
```
Unload the driver :
```
# sudo make unload
sudo rmmod TaxiDriver
sudo rm /dev/TaxiDriver
```

#### Usermode Target/Test Application
```
cd um/test && make
./test_app
```

#### Usermode Driver Application
```
cd um && make
./main
```


### Known Issues
- Fails to open device:
MAJOR number has to match in mknod to open device! Check with `grep "TaxiDriver" /proc/devices`

If you find anything not listed above. Please open up Issues on the Repo
and provide the build log showing the issue with `LANG=C make` with all of your steps and envoirment info to reproduce.

### Missing Features in development
- ImGui Overlay
- expand km mem functions
- expand um driver example and test_app
 - Findpattern
 - write/inject shellcode with asm mid function hook
- X11 Input Event Handling

## Contact
If you have a problem regarding the code, open an issue or make a pull request, i'll be happy to add your contribution !<br>
If you need to contact me for any other reasons, message me at `_.patate` on discord.
