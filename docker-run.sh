#!/bin/bash


BASEDIR="$(readlink -f $(dirname $0))"


docker run -it --privileged -p 4242:4242 -v $BASEDIR:/container/stm32_f401re_barebone_sample -v /dev/bus/usb:/dev/bus/usb --entrypoint /bin/bash arm_build_env:v1
