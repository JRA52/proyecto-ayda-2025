# General info

This is a program for analyzing blood samples and detecting white blood cells.
Images must be in .jep format.
The objective lens magnification should be 40x.

## Installation
To install, you must meet the following requirements on your system

cmake, clang, SFML, zenity, Designar

To instal designar go to https://github.com/R3mmurd/DeSiGNAR


```bash
sudo apt install     libxrandr-dev     libxcursor-dev     libxi-dev     libudev-dev     libfreetype-dev     libflac-dev     libvorbis-dev     libgl1-mesa-dev     libegl1-mesa-dev     libfreetype-dev

sudo apt install cmake

sudo apt install clang

sudo apt install zenity

sudo apt install libx11-dev libsfml-dev
```

## Usage
Once the project has started, you must select the folder with the images in .jepg format and proceed to start the analysis.

```c++
make
./PQC_Analyzer
```