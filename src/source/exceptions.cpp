#include "../include/exceptions.h"


BaseException::BaseException(const char* message) : message(message)
{

}

void BaseException::setMessage(const char* message)
{
    this->message = message;
}



const char* BaseException::getMessage() const
{
    return this->message;
}



Exception::Exception(const char* message) : BaseException(message)
{
    
}



OSError::OSError(const char* message) : Exception(message)
{

}



ConnectionError::ConnectionError(const char* message) : OSError(message)
{

}
