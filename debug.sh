#!/bin/bash

st-util &
gdb-multiarch -tui --eval-command="target remote localhost:4242" main.elf
