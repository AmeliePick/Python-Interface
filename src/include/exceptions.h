#pragma once


class BaseException
{
private:
    const char* message;

public:
    BaseException(const char* message = "");


    void setMessage(const char* message);

    const char* getMessage() const;
};



class Exception : public BaseException
{
public:
    Exception(const char* message = "");
};



class OSError : public Exception
{
public:
    OSError(const char* message = "");
};


class ConnectionError : public OSError
{
public:
    ConnectionError(const char* message = "");

};

