#include "SDController.h"
#include "USBCommand.h"


USBCommand* usb;
String data = "";
String command = "";

char bt = '\0';


void setup() 
{
    Serial.begin(9600);
    usb = new USBCommand();
    Serial.println("All systems ready: Waiting for commands");

}

void loop()
{
    
    if(Serial.available() > 0)
    {
        // Get command char by char foreach loop
        bt = Serial.read();
        if(bt == '\n' || bt == '\r')
        {
            command = data;
            data = "";
        }
        if((' ' <= bt) && (bt <= '~'))
        {
            data += bt;
        }

        // Check if command is completed
        if(command.length() > 0)
        {
            Serial.print(usb->processCommand(command));
            command = ""; // Clean the command
        }
    }
}
