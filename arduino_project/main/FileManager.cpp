#include "FileManager.h"

FileManager::FileManager() 
{
    m_pSDController = new SDController();
    if(!m_pSDController->initSD())
    {
      Serial.println("Cannot Init SD Card");
    }
    else
    {
      Serial.println("Init SD OK");
    }
}

FileManager::~FileManager() {}

String FileManager::readFile(String name, unsigned long offset, int read_size)
{
    if(m_pSDController->fileExists(name))
    {
        return "read_ok " + m_pSDController->readFile(name, offset, read_size) + " \n";
    }
    else
    {
        return "read_err file_not_found \n";
    }
}

String FileManager::writeFile(String name, unsigned long offset, String data)
{
    m_pSDController->writeFile(name, data, offset);

    // TODO: Check more error cases
    if(!m_pSDController->fileExists(name))
    {
        return "write_error file_not_writed \n";
    }

    return "write_ok \n";
}

String FileManager::removeFile(String name)
{
    if(m_pSDController->fileExists(name))
    {
        m_pSDController->deleteFile(name);
        
        return "remove_ok \n";
    }

    return "remove_error file_cannot_be_deleted \n";
}

String FileManager::listFiles()
{
    File root = m_pSDController->getRootPath();
    String data_list = "{\"dir\":[";

    while(true)
    {
      File entry = root.openNextFile();
      if(!entry) 
      {
        break;
      }

      if(!entry.isDirectory())
        data_list += "\"" + String(entry.name()) + "\",";
      entry.close();
    }

    data_list += "\"naan\"]}";
    return "list_ok " + data_list + " \n";
 
}

String FileManager::getFileSize(String name)
{
    if(m_pSDController->fileExists(name))
    {
        return "size_ok " + String(m_pSDController->getFileSize(name)) + " \n";
    }

    return "size_error file_not_found"; 
}
