#ifndef H_USBCOMMAND
#define H_USBCOMMAND

#include "commonlibs.h"
#include "FileManager.h"

class USBCommand
{
    public:
        USBCommand();
        ~USBCommand();
        String processCommand(String command);

        String onReadCommand(String command);
        String onWriteCommand(String command);

    private:
       String getValue(String command, char separator, int pos);
       FileManager * m_pFileManager;
};

#endif
