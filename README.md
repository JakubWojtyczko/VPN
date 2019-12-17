# VPN
VPN is a client-server software for protecting and encrypting your network connection via public Internet.

## Requirements
You need to have installed following libraries to compile source.
 * [OpenSSL](https://www.openssl.org)
 * Pcap Library ([Npcap](https://nmap.org/npcap) on Windows and [libPcap](https://www.tcpdump.org) on Linux)
 * g++ compiler on Linux and ming32 on Windows.
## Building a project

### Linux
Install required libraires by typing:
```
# apt install libssl-dev
# apt install libpcap-dev
```
Compile program:
```
$ make
```

### Windows
Download the last stable Npcap SDK from [here](https://nmap.org/npcap)

Donwload OpenSSL static libraries from [here](http://www.wittfella.com/openssl)

Compile program typing:
```
> mingw32-make.exe
```
