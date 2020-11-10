#include "../include/logger.h"


Logger::Logger()
{
    this->fileWrite = std::ofstream("interpreter_log.txt", std::ios_base::app | std::ios_base::binary);
    this->fileWrite.close();
}



Logger* Logger::create()
{
    static Logger* instance = new Logger();
    return instance;
}



void Logger::writeLog(const char* data)
{
    if(this->fileWrite.is_open() == false)
        this->fileWrite.open("interpreter_log.txt", std::ios_base::app | std::ios_base::binary);

    this->fileWrite << data;

    fileWrite.close();
}
