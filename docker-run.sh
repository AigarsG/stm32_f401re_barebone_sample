#!/bin/bash


BASEDIR="$(readlink -f $(dirname $0))"


docker run -v $BASEDIR:/container/stm32_f401re_barebone_sample -it --entrypoint /bin/bash arm_build_env:v1
