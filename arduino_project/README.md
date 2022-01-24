# Daleakz: Arduino software

In this folder there is the code that must be uploaded to the Arduino.

## Requirements

- BADUSB 3212 ATmega32u4 with SD included
- Arduino IDE
- MicroSD con FAT32

![ARDUINO_USB](img/atmega32u3.jpg)

## How to

- Open the file main.ino with the Arduino IDE
- Connect the Arduino to a USB port
- Select as Arduino Leonardo board and the port where the Arduino is connected
- Upload the sketch

Once uploaded, it can be used with the Python program

## Technical Details

In order to transfer files from the computer, the Serial port is used for communications.
To do this, a protocol has been implemented at the application level to facilitate the upload of files.

Currently, this project has 5 commands that are structured in this way:

```
<header> <params> \n
```

Each command ends with a line break \n. The possible commands currently implemented are:
- read: Read a file from the SD
- write: Write a file to the SD
- list: List all files on the SD
- size: Get the size of a file inside the SD

### Read Command

The read command is used to read a file into the SD. The parameters are the following:
- file_name: Name of the file you want to read
- offset: Position you want to read
- read_size: How much data do you want to read

If the command has been successful it returns:
- read_ok
- content: Returns the content of the SD

If the command fails it returns read_error and these reasons:
- file_not_found: File inside the SD not found.

### Write Command

The write command is used to write a file to the SD. The parameters are the following:
- file_name: Name of the file you want to write (If it does not exist, a new one is created)
- offset: Position where you want to start writing
- data: The content of the file

If the command was successful, it returns a write_ok

### List Command

The list command is used to display the list of files on the SD. If the command is successful, it returns a JSON with all the files contained in the SD.
It will always return a file named naan.

### Remove Command

The remove command removes a file from the SD. The parameters are the following:
- file_name: The name of the file you want to delete

If the command was successful it returns a remove_ok
If the command fails it returns remove_error and these reasons:
- file_not_found: File inside the SD not found.