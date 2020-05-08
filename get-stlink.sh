#!/bin/bash

git clone https://github.com/stlink-org/stlink.git /opt/stlink

cd /opt/stlink && make release \
		&& cp /opt/stlink/etc/udev/rules.d/* /etc/udev/rules.d \
		&& udevadm control --reload-rules \
		&& udevadm trigger


ln -s /opt/stlink/build/Release/bin/st-util /usr/local/bin/st-util
ln -s /opt/stlink/build/Release/bin/st-flash /usr/local/bin/st-flash
