#include "USBCommand.h"

USBCommand::USBCommand() 
{
    m_pFileManager = new FileManager();
}

USBCommand::~USBCommand() {}

String USBCommand::processCommand(String command)
{
    String header = getValue(command, ' ', 0);
    
    // Request Packet Struct: <header> <file_name> <offset> <read_size> \n
    // Response Packet Struct: <read ok> <data> <length data> <total_file_size> \n
    if(header.startsWith("read"))
    {
        String file_name = getValue(command, ' ', 1);
        unsigned long offset = strtoul(getValue(command, ' ', 2).c_str(), 0, 10);
        int read_size = getValue(command, ' ', 3).toInt();

        return m_pFileManager->readFile(file_name, offset, read_size);
    }

    // Request Packet Struct: <header> <file_name> <offset> <data> \n
    // Response packet Struct: <write ok> \n
    if(header.startsWith("write"))
    {
        String file_name = getValue(command, ' ', 1);
        unsigned long offset = strtoul(getValue(command, ' ', 2).c_str(), 0, 10);
        String data = getValue(command, ' ', 3);

        return m_pFileManager->writeFile(file_name, offset, data);
    }

    // Request Packet Struct: <header> <file_name> \n
    // Response Packet Struct: <remove ok> \n
    if(header.startsWith("remove"))
    {
        String file_name = getValue(command, ' ', 1);
        return m_pFileManager->removeFile(file_name);
    }

    // Request Packet Struct: <header> \n
    // Response Packet Struct: <list ok> {"dir":["file_name1", "file_name2",....,"naan"]} \n
    if(header.startsWith("list"))
    {
        return m_pFileManager->listFiles();
    }

    // Request Packet Struct: <header> <file_name>
    // Response Packet: <size ok> file_size \n
    if(header.startsWith("size"))
    {   
        String file_name = getValue(command, ' ', 1);
        return m_pFileManager->getFileSize(file_name);
    }
}

String USBCommand::getValue(String command, char separator, int pos)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = command.length() - 1;

    for(int i = 0; i <= maxIndex && found <= pos; i++)
    {
        if(command.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }

    return (found > pos ? command.substring(strIndex[0], strIndex[1]) : "");
}
