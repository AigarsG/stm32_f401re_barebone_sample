Native build
make

Native debug build
make DEBUG=1

Target build
make ARCH=arm CROSS_COMPILE=arm-none-eabi-

Target debug build
make DEBUG=1 ARCH=arm CROSS_COMPILE=arm-none-eabi-


Programming device
./get-stlink.sh (run only once)
./flash.sh


Debugging with gdb
./debug.sh
