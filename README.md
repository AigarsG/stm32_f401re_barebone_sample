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
