## About
Simple LED light row as barebone example implemented on STM32F401re microcontroller.
Button press (PC13) changes direction of light row. Debug build outputs notification
about direction LED light row direction on UART1_TX (PA9) pin at 115200 baud rate, 1 stop bit, 8-bit word

## Build environment
<ol>
<li>./docker-build.sh to build and image </li>
<li>./docker-run.sh to run and log into container based on built image</li>
</ol>

## Building
### Native
#### Native build
make

#### Native debug build
make DEBUG=1

### Target (from inside Docker container)
#### Taget build
make ARCH=arm CROSS_COMPILE=arm-none-eabi-

#### Target debug build
make DEBUG=1 ARCH=arm CROSS_COMPILE=arm-none-eabi-

#### Programming device
./flash.sh

#### Debugging with gdb
./debug.sh
