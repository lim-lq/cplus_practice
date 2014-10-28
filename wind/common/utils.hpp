#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <errno.h>

#include "log.hpp"
inline void print_error(const std::string& msg)
{
    LOG4CPLUS_INFO(LOGGER, msg << "failure, error code is[" << errno << "]");
}
#endif