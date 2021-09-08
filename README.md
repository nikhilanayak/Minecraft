# Minecraft

ugh ill make this better later sorry :(


## Setup
* Run on ubuntu
* apt install libgl-dev pkg-config xorg-dev libglfw3-dev libglfw3 git cmake gcc python3 python3-pip
* pip3 install conan
* git clone https://github.com/IndianBoy6/Minecraft.git
* cd Minecraft

* pushd /tmp
* git clone https://github.com/recp/cglm.git
* cd cglm
* mkdir build
* cd build
* cmake ..
* make
* sudo make install
* popd

* install glad (add headers to path, glad.c is already added)

* cd build
* conan install ..
* cmake ..
* cd ..
* cmake --build build
* bin/Minecraft (to run program)
