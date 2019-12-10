#!/bin/bash

# create tap interface
sudo tunctl -t tapvpn

# set address, mask and run
ifconfig tapvpn 192.168.255.1 netmast 255.255.255.0 up
