#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <stdint.h>

#include <string>
#include <memory>
#include <fstream>

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>

namespace wind
{
namespace common
{
// One log file can not exceed 128M
static const uint32_t MAX_LOG_FILE_SIZE = 128 * 1024 * 1024;

// there are at most 32 log files;
static const uint32_t MAX_LOG_FILE_NUM = 32;

static const char* LOG_FORMAT = "[%D][%p][%t]%m[%l]%n";

static const char* MODULE_NAME = "WIND";

//static const char* LOG_LEVEL_FILE = "/tmp/_log_level";

static inline void LoggerInitialize(const std::string& logFileName)
{
    log4cplus::SharedAppenderPtr append(new log4cplus::RollingFileAppender(logFileName, MAX_LOG_FILE_SIZE, MAX_LOG_FILE_NUM));
    std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(LOG_FORMAT));
    append->setLayout(layout);
    append->setName(MODULE_NAME);
    log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(MODULE_NAME));
    logger.addAppender(append);
    logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
}

} // end namespace common
} // end namespace wind

#define LOGGER ::log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(wind::common::MODULE_NAME))

#endif