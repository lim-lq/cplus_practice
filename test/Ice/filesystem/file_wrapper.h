#ifndef __FILE_WRAPPER_H__
#define __FILE_WRAPPER_H__

#include "fileInterface.h"

namespace blond{
class FileWrapper : public Blond::FileInterface{
public:
    FileWrapper(){}
    ~FileWrapper(){}

    ::std::string downloadfile(const ::std::string& file, const ::Ice::Current& current = ::Ice::Current());
    Blond::Files getfiles(const ::Ice::Current& current = ::Ice::Current());
    void uploadfile(const ::std::string& file, const ::Ice::Current& current = ::Ice::Current());
}; // end class FileWrapper
} // end namespace blond
#endif