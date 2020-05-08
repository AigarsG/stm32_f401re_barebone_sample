FROM debian:buster

# Install build tools
RUN apt-get update && apt-get install -y	binutils-arm-none-eabi \
						gcc-arm-none-eabi \
						gdb-multiarch \
						build-essential \
						git \
						cmake \
						pkg-config \
						libusb-1.0 \
						libusb-1.0-0-dev \
						udev

ADD get-stlink.sh /
RUN /get-stlink.sh
