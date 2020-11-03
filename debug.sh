#!/bin/bash

st-util &
gdb-multiarch -tui --eval-command="target extended-remote localhost:4242" main.elf
