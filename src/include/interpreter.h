#pragma once
#include <Python.h>
#include <vector>


namespace runtime_errors
{
    class ModuleNotImported
    {

    };


    class FunctionNotLoaded
    {

    };


    class ClassNotLoaded
    {

    };
}



class Interpreter
{
private:
    Interpreter(std::wstring& PYTHONHOME);
    Interpreter(const Interpreter &);

    class Logger* logger;

    std::vector<PyObject*> modules;
    std::vector<PyObject*> objects;

public:
    // -- Interpreter initialization --
    static Interpreter* init(std::wstring PYTHONHOME = L"");


    // -- Get the handle of python module --
    // * moduleName: the module name without file format.
    PyObject* initModule(const char* moduleName);

    PyObject* loadClass(const char* moduleName, const char* className);

    PyObject* loadClass(PyObject* moduleHandle, const char* className);

    PyObject* loadFunction(const char* moduleName, const char* functionName);

    PyObject* loadFunction(PyObject* moduleHandle, const char* functionName);

    PyObject* callObject(PyObject* callable_object, PyObject* args);


    void Python_traceback_toFile();

    void deleteObject(PyObject* object);


    void raise(const char* pythonExceptionName = "");

    ~Interpreter();
};
