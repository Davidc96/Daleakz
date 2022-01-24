#include "SDController.h"

SDController::SDController(){}
SDController::~SDController(){}

bool SDController::initSD()
{
    return SD.begin(4);
}

bool SDController::fileExists(String name)
{
    return SD.exists(name);
}


unsigned long SDController::getFileSize(String name)
{
    File dataFile = SD.open(name, FILE_READ);
    unsigned long size = dataFile.size();
    dataFile.close();
    return size;
}

String SDController::readFile(String name, unsigned long pos, int read_size)
{
    File dataFile = SD.open(name, FILE_READ);
    char fileContent[read_size];
    memset(fileContent, '\0', sizeof(fileContent));
    
    dataFile.seek(pos);
    dataFile.read(fileContent, read_size);
    fileContent[read_size] = '\0';
    dataFile.close();
    return String(fileContent);
}

void SDController::writeFile(String name, String data, unsigned long pos)
{
    File dataFile = SD.open(name, FILE_WRITE);
    dataFile.seek(pos);
    dataFile.print(data);
    dataFile.close();
}

File SDController::getRootPath()
{
    return SD.open("/");
}

void SDController::deleteFile(String name)
{
  SD.remove(name);
}
