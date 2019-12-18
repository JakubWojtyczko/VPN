#!/bin/bash

# Interface name and address must be
# compatibile with file config.conf

tap=tapvpn
addr="192.168.255.1"
mask="255.255.255.0"
broadcast="192.168.255.255"


# create tap interface
ip tuntap add mode tap $tap

# link up
ip link set dev $tap up

# set address, mask and run
ifconfig $tap $addr netmast $mask broadcast $broadcast up

# set tap interface as your default gateway
route add default gw $addr dev $tap metric 1

