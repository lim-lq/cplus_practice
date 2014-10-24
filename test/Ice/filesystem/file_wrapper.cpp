#include "file_wrapper.h"
#include <iostream>

namespace blond
{
::std::string FileWrapper::downloadfile(const ::std::string& file, const ::Ice::Current& current)
{
    std::cout << "I'm in downloadfile" << std::endl;
    std::string name = "server";
    return name;
}

void FileWrapper::uploadfile(const std::string& file, const ::Ice::Current& current)
{
    m_queue.put(file);
    std::cout << "I'm in uploadfile" << std::endl;
}

::Blond::Files FileWrapper::getfiles(const ::Ice::Current& current)
{
    ::Blond::Files files;
    files.push_back("yuwen");
    files.push_back("shuxue");
    files.push_back("daishu");

    return files;
}

void FileWrapper::startConsumerThread()
{
    m_consumer = new Consumer(&m_queue);
    m_consumer->start();
}
}