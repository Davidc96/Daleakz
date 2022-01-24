# DaLeakz: CLI Software

In this folder there is the CLI program to communicate with the Arduino in order to transfer files. The script is called daleakz.py and is written in
Python3.

# Requirements

- Python 3
- PySerial
- pyfiglet

# Installation

To get the script working, you need to install Python3 and Pyserial with:

```
python3 -m pip install pyserial pyfiglet
python3 -h daleakz.py
```

# How to
```
 ____        _               _
|  _ \  __ _| |    ___  __ _| | __ _____
| | | |/ _` | |   / _ \/ _` | |/ /|_  (_)
| |_| | (_| | |__|  __/ (_| |   <  / / _
|____/ \__,_|_____\___|\__,_|_|\_\/___(_)

    _            _       _                           _   _ ____  ____
   / \   _ __ __| |_   _(_)_ __   ___     __ _ ___  | | | / ___|| __ )
  / _ \ | '__/ _` | | | | | '_ \ / _ \   / _` / __| | | | \___ \|  _ \
 / ___ \| | | (_| | |_| | | | | | (_) | | (_| \__ \ | |_| |___) | |_) |
/_/   \_\_|  \__,_|\__,_|_|_| |_|\___/   \__,_|___/  \___/|____/|____/


Created by @Davidc96

usage: daleakz.py [-h] [-f --file] read|write|list port

Send or receive files to Arduino using COM Port

positional arguments:
  read|write|list  action you would like to use
  port             Linux: /dev/USBX Windows: COMX

optional arguments:
  -h, --help       show this help message and exit
  -f --file        Name of the file (Not need it in list)
```

# Technical Details

Due to the limitation that Arduino has when creating variables, due to memory, it is necessary to send and receive the content
with multiple read or write commands. The maximum transfer for it to work is 50 bytes per command.

To avoid any problem with NULL Bytes, what is done is to convert a file to base64 to display printable characters in exchange for the file size increasing.

Another difficulty that Arduino has is when creating the names of the files where they cannot contain depending on their length or what special character, so what is done is to generate a random name and then the original name is mapped with the new name. That mapping is stored in filedb.json

On the MicroSD, all files will be stored in the root with no ability to create folders or anything.