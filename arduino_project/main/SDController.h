#ifndef H_SDCONTROLLER
#define H_SDCONTROLLER

#include "commonlibs.h"
class SDController 
{
    public:
        SDController();
        ~SDController();
        bool initSD();
        void writeFile(String name, String data, unsigned long pos);
        void deleteFile(String name);
        unsigned long getFileSize(String name);
        bool fileExists(String name);
        String readFile(String name, unsigned long pos, int size);
        File getRootPath();
            
    };

#endif
