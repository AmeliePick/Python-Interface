#include "../include/class.h"
#include "../include/interpreter.h"
#include <string>



Class::Class(const char* moduleName, const char* className, Function::Arguments&& args)
{
    loadedMethods = std::multimap<std::string, Function>();
    PyObject* _class = Interpreter::init()->loadClass(moduleName, className);

    if (_class != nullptr)
    {
        pyClass = PyObject_CallObject(_class, args.get());
        Py_CLEAR(_class);
    }   
}
 


ReturnType Class::callStaticMethod(const char* moduleName, const char* className, const char* methodName, Function::Arguments& args)
{
    // Storages all static methods from all classes.
    static std::multimap<std::string, Function> loadedStaticMethods = std::multimap<std::string, Function>();


    std::string strMethodString(methodName);
    auto elem = loadedStaticMethods.find(strMethodString);
    if (elem == loadedStaticMethods.end())
    {
        PyObject* _className = Interpreter::init()->loadClass(moduleName, className);
        auto func = loadedStaticMethods.insert(std::pair<std::string, Function>(strMethodString, Function(_className, methodName)));

        if (func != loadedStaticMethods.end())
            return func->second.call(args);
        else return ReturnType(nullptr);
    }
    else
        return elem->second.call(args);
}



ReturnType Class::callMethod(const char* methodName, Function::Arguments& args)
{
    std::string stringMethod(methodName);
    auto elem = loadedMethods.find(stringMethod);
    if (elem == loadedMethods.end())
    {
        auto func = loadedMethods.insert(std::pair<std::string, Function>(stringMethod, Function(pyClass, methodName)));

        if (func != loadedMethods.end())
            return func->second.call(args);
        else return ReturnType(nullptr);
    }
    else
        return elem->second.call(args);
}



Class::~Class()
{
    PyObject_CallMethod(pyClass, "__del__", nullptr);
    loadedMethods.clear();

    Interpreter::init()->deleteObject(pyClass);
}
