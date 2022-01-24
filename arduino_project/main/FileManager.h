#ifndef H_FILEMANAGER
#define H_FILEMANAGER
#include "SDController.h"
#include "commonlibs.h"

class FileManager
{
    public:
        FileManager();
        ~FileManager();

        String readFile(String name, unsigned long offset, int read_size);
        String writeFile(String name, unsigned long offset, String data);
        String removeFile(String name);
        String listFiles();
        String getFileSize(String name);
    private:
        SDController * m_pSDController;

        
};








#endif
