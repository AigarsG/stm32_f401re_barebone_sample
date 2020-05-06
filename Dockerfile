FROM debian:buster

# Install build tools
RUN apt-get update && apt-get install -y	binutils-arm-none-eabi \
						gcc-arm-none-eabi \
						gdb-multiarch \
						build-essential
