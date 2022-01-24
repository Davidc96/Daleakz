import serial
import time
import argparse
import base64
import json
import random
import pyfiglet

MAX_TRANSFER_SIZE = 50

FILE_DIC = {}

class FileManager:

    def __init__(self, arduino):
        self.usb = USBController(arduino)

    # Write a file to the arduino
    def write(self, file_name, new_name):
        try:
            with open(file_name, "rb") as f:
                file_content = f.read()
                b64content = base64.b64encode(file_content).decode("utf-8")
                partial_content = ""
                offset = 0

                # Get all the slices to transfer to arduino
                for i in range(len(b64content)):
                    if (i % MAX_TRANSFER_SIZE) != 0 or i == 0:
                        partial_content += b64content[i]
                    else:
                        result = self.usb.send_file_usb(partial_content, offset, new_name) # Send to the arduino

                        if "write_ok" in result:
                            offset += len(partial_content)
                            partial_content = b64content[i]
                            
                            print("[+] Transfering file: " + str(int((offset / len(b64content)) * 100)) + 
                            " % Transfered: " + str(offset) + " bytes / " + str(len(b64content)) + " bytes", 
                            end="", flush=True)
                            print("\r", end='')
                        else:
                            print()
                            print("[-] Failed to transfer the file !")
                            print(result)
                            self.usb.close()
                            return False
                else:
                    if len(partial_content) > 0:
                        result = self.usb.send_file_usb(partial_content, offset, new_name) # Send to the arduino
                print()
                print("[+] Transfer successfully")
                self.usb.close()
                return True
        except Exception as e:
            print(e)

    # Read a file from arduino and stores it on the disk
    def read(self, file_name, file_name_ori):
        file_size = int(self.usb.get_file_size(file_name).split(' ')[1])
        read_size = MAX_TRANSFER_SIZE
        offset = 0
        data_content = ""

        while(offset <= file_size):
            result = self.usb.get_file_content_usb(file_name, offset, MAX_TRANSFER_SIZE)
            offset += MAX_TRANSFER_SIZE
            print("[+] Reading file: " + str(int((offset / file_size) * 100)) + 
            " % Transfered: " + str(offset) + " bytes / " + str(file_size) + " bytes", 
            end="", flush=True)
            print("\r", end='')
            content = result.split(' ')[1]
            data_content += content
        
        with open(file_name_ori, "wb") as f:
            f.write(base64.b64decode(data_content))
        
        self.usb.close()
    
    # List all the files in SD
    def list_files(self, file_database):
        result = json.loads(self.usb.get_file_list().split(' ')[1])

        print("----- Listing files ------")
        files = result["dir"]
        for f in files:
            for a in file_database.keys():
                if file_database[a] == f:
                    print(a)
                    break
            else:
                if f != "naan": 
                    print(f + " (Not found in DB you can extract it typing the name )")
        print("--------------------------")
        
        self.usb.close()
    
    # Remove a file in SD
    def remove_file(self, file_name):
        result = self.usb.remove_file(file_name)
        self.usb.close()
        return "remove_ok" in result


    # Generate a random name using OS system time as SEED
    def generate_random_name(self, str_len):
        word_list = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        name = ""
        for i in range(str_len):
            name += word_list[random.randint(0, 200) % len(word_list)]
        
        return name

class USBController:

    def __init__(self, arduino):
        self.arduino = arduino

    # Send the file write command to the arduino
    def send_file_usb(self, b64content, offset, file_name):
        command = ("write " + file_name + " " + str(offset) + " " + b64content + "\n").encode("utf-8")
        self.arduino.write(command)
        return self.arduino.readline().decode("utf-8")

    # Send the size command to arduino and returns its value
    def get_file_size(self, file_name):
        command = ("size " + file_name + "\n").encode("utf-8")
        self.arduino.write(command)
        return self.arduino.readline().decode("utf-8")

    # Send the read command to arduino and returns its content
    def get_file_content_usb(self, file_name, offset, read_size):
        command = ("read " + file_name + " " + str(offset) + " " + str(read_size) + "\n").encode("utf-8")
        self.arduino.write(command)
        return self.arduino.readline().decode("utf-8")
    
    # Send the remove command and returns its value
    def remove_file(self, file_name):
        command = ("remove " + file_name + "\n").encode("utf-8")
        self.arduino.write(command)
        return self.arduino.readline().decode("utf-8")

    # Close the arduino connection
    def close(self):
        self.arduino.close()
    
    def get_file_list(self):
        command = ("list\n").encode("utf-8")
        self.arduino.write(command)
        return self.arduino.readline().decode("utf-8")

class FileDatabase:
    # Read the File List JSON DB
    def read_json_file_db(self):
        with open("filedb.json", "r") as f:
            return json.loads(f.read())

    # Write any changes to the Filedb json
    def write_json_file_db(self, file_dic):
        with open("filedb.json", "w") as f:
            f.write(json.dumps(file_dic, indent=4))


banner = pyfiglet.figlet_format("DaLeakz: Arduino as USB")
print(banner)
print("Created by @Davidc96")
print()

pr_parser = argparse.ArgumentParser(description="Send or receive files to Arduino using COM Port")
pr_parser.add_argument("action",
                        metavar="read|write|list|remove",
                        type=str,
                        help="action you would like to use")
pr_parser.add_argument("-f",
                       metavar="--file",
                       type=str,
                       help="Name of the file (Not need it in list)")
pr_parser.add_argument("port",
                        metavar="port",
                        type=str,
                        help="Linux: /dev/USBX Windows: COMX")
args = pr_parser.parse_args()

try:
    arduino = serial.Serial(args.port, 9600)
    file_manager = FileManager(arduino)
    file_database = FileDatabase()
        
    FILE_DIC = file_database.read_json_file_db()

    if args.action == "read":
        if args.f:
            name_sd = FILE_DIC.get(args.f, args.f)
            file_manager.read(name_sd, args.f)
    
    if args.action == "write":
        if args.f:
            new_name = file_manager.generate_random_name(5)
            if file_manager.write(args.f, new_name):
                FILE_DIC[args.f] = new_name

    if args.action == "list":
        file_manager.list_files(FILE_DIC)
    
    if args.action == "remove":
        result = file_manager.remove_file(FILE_DIC.get(args.f, args.f))

        if result:
            print("[+] File removed from SD")
            if args.f in FILE_DIC.keys():
                FILE_DIC.pop(args.f)
                    
    file_database.write_json_file_db(FILE_DIC)
except Exception as e:
    file_database.write_json_file_db(FILE_DIC)
    print(e)
