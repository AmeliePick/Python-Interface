#include "../include/interpreter.h"
#include "../include/logger.h"
#include "../include/exceptions.h"
#include <frameobject.h>
#include <shlwapi.h>
#include <Windows.h>


Interpreter::Interpreter(std::wstring& PYTHONHOME)
{
    /*
     * PYTHONHOME is the path to python directory where are language runtime and language libraries.
     * By the default, this path gets from system enviroment variable(PATH or PYTHONPATH).
     *
     * If the application or library is using its own version of python you need specify the desired path
     * and put runtime dll to the folder with executable file.
     *
     *
     * applicationFolder/
     *     python/
     *     application.exe
     *     pythonVersion.dll

     * In this case you must indicate PYTHONHOME as L"python" and it will be use your python version.
    */


    this->logger = Logger::create();

    if (PYTHONHOME != L"") Py_SetPythonHome(_wcsdup(PYTHONHOME.data()));
    
    Py_Initialize();
}



Interpreter* Interpreter::init(std::wstring PYTHONHOME)
{
    static Interpreter* instance = new Interpreter(PYTHONHOME);
    return instance;

}



PyObject* Interpreter::initModule(const char* moduleName)
{
    PyObject* module = PyUnicode_FromString(moduleName);
    PyObject* moduleHandle = PyImport_Import(module);

    Python_traceback_toFile();

    if (moduleHandle == nullptr)
        throw runtime_errors::ModuleNotImported();

    modules.push_back(moduleHandle);
    Py_CLEAR(module);
    
    
    return moduleHandle;
}



PyObject* Interpreter::loadClass(const char* moduleName, const char* className)
{
    return this->loadClass(initModule(moduleName), className);
}



PyObject* Interpreter::loadClass(PyObject* moduleHandle, const char* className)
{
    if (moduleHandle == nullptr) return nullptr;
    
    PyObject* dict = PyModule_GetDict(moduleHandle);
    PyObject* python_class = PyDict_GetItemString(dict, className);

    Python_traceback_toFile();

    if (python_class == nullptr)
        throw runtime_errors::ClassNotLoaded();

    objects.push_back(python_class);
    Py_CLEAR(dict);

    
    return python_class;
}



PyObject* Interpreter::loadFunction(const char* moduleName, const char* functionName)
{
    return this->loadFunction(initModule(moduleName), functionName);
}



PyObject* Interpreter::loadFunction(PyObject* moduleHandle, const char* functionName)
{
    if (moduleHandle == nullptr) return nullptr;

    PyObject* function = PyObject_GetAttrString(moduleHandle, functionName);

    Python_traceback_toFile();

    if (function == nullptr)
        throw runtime_errors::FunctionNotLoaded();

    objects.push_back(function);


    return function;
}



PyObject* Interpreter::callObject(PyObject* callable_object, PyObject* args)
{
    PyObject* finalObject = PyObject_CallObject(callable_object, args);

    Python_traceback_toFile();

    if (finalObject != nullptr && find(objects.begin(), objects.end(), finalObject) == objects.end())
        objects.push_back(finalObject);

    return finalObject;
}



void Interpreter::Python_traceback_toFile()
{
    PyObject *ptype, *pvalue, *ptraceback;
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);

    if (pvalue && pvalue && ptraceback)
    {
        PyTracebackObject tb = *(PyTracebackObject*)ptraceback;
        //this->logger->writeLog(PyUnicode_AsUTF8(PyObject_Str((PyObject*)tb.tb_frame->f_back->f_code)));
        this->logger->writeLog(PyUnicode_AsUTF8(PyObject_Str((PyObject*)tb.tb_frame->f_code)));
        this->logger->writeLog(" ");
        this->logger->writeLog(PyUnicode_AsUTF8(PyObject_Str(pvalue)));
        this->logger->writeLog("\n");    
    }

    PyErr_Restore(ptype, pvalue, ptraceback);
}



void Interpreter::deleteObject(PyObject* object)
{
    if (object != nullptr)
    {
        auto obj = std::find(objects.begin(), objects.end(), object);

        if (obj != objects.end())
        {
            Py_CLEAR(objects[std::distance(objects.begin(), obj)]);
            objects.erase(obj);
        }
        else
        {
            obj = std::find(modules.begin(), modules.end(), object);

            Py_CLEAR(modules[std::distance(modules.begin(), obj)]);
            modules.erase(obj);
        }
    }
}



void Interpreter::raise(const char* pythonExceptionName)
{
    std::string exceptionName(pythonExceptionName);

    if (exceptionName == "")
    {
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);

        exceptionName = pvalue->ob_type->tp_name;

        Py_CLEAR(ptype);
        Py_CLEAR(pvalue);
        Py_CLEAR(ptraceback);
    }
    
    if (exceptionName == "BaseException") throw BaseException();
    else if (exceptionName == "Exception") throw Exception();
    else if (exceptionName == "ConnectionError") throw ConnectionError();
}



Interpreter::~Interpreter()
{
    /* Py_Finalize() isn't working.
     * Log: Assertion failed: _PyGCHead_REFS(gc) != 0, file ..\Modules\gcmodule.c, line 380
     *
     * API: https://docs.python.org/2/c-api/init.html#c.Py_Finalize
     */


    // Manualy delete objects instead Py_Finalize.
    for (size_t i = objects.size() - 1; i >= 0; --i)
    {
        Py_CLEAR(objects[i]);
    }
    for (size_t i = modules.size() - 1; i >= 0; --i)
    {
        Py_CLEAR(modules[i]);
    }

    delete logger;
    
    // Here should be Py_Finalize calling.
}
