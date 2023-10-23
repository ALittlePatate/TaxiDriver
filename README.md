# TaxiDriver
W/RPM Driver and usermode for Linux.<br>
Some things in the client are still not finished :
* X11 overlay

# Features
* Get the base address of any process/loaded .so file
* RPM
* WPM

# Compiling
To compile the driver you'll need to be at least on kernel version v6.5.8 (i used the [arch kernel](https://github.com/archlinux/linux/releases/tag/v6.5.8-arch1)).<br>

## Prerequises :
You'll need git, linux-headers, SDL3 and OpenGL :
```
sudo pacman -S git linux-headers OpenGL
yay -S sdl3-git
```

## Compiling and running :
Clone the repo, prepare imgui (still in active dev) :
```
git clone https://github.com/ALittlePatate/TaxiDriver --recursive
chmod +x prepare_imgui.sh
./prepare_imgui.sh
```
Compile :
```
make
```
Run the driver :
```
sudo make load
sudo dmesg | grep TaxiDriver
```
You should find a line saying that TaxiDriver was loaded with X major code.
```
sudo mknod /dev/TaxiDriver c X 0
```
replace X with your major code.<br>
Run the client :
```
sudo ./Revird
```
Unload the driver :
```
sudo make unload
```

# Contact
If you have a problem regarding the code, open an issue or make a pull request, i'll be happy to add your contribution !<br>
If you need to contact me for any other reasons, message me at @_.patate on discord.
