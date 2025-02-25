#include "types_conversion.h"
#include "function.h"


template<typename keyType, typename valueType>
inline std::multimap<keyType, valueType> ReturnType::ToMultimap()
{
    PyObject* keys = PyDict_Keys(value);
    PyObject* values = PyDict_Values(value);

    void* keyDecoded = nullptr;
    void* valueDecoded = nullptr;

    std::multimap<keyType, valueType> dict;

    for (int i = 0; i < PyDict_Size(value); ++i)
    {
        ToType<keyType>(PyList_GetItem(keys, i), &keyDecoded);
        ToType<valueType>(PyList_GetItem(values, i), &valueDecoded);

        dict.insert(std::pair<keyType, valueType>(pointer_data<keyType>(keyDecoded), pointer_data<valueType>(valueDecoded)));
    }


    if (std::is_same<keyType, const char*>::value == false && std::is_same<keyType, void*>::value == false)
        free(keyDecoded);

    if (std::is_same<valueType, const char*>::value == false && std::is_same<valueType, void*>::value == false)
        free(valueDecoded);

    Py_CLEAR(keys);
    Py_CLEAR(values);


    return dict;
}



template<typename valueType>
inline std::vector<valueType> ReturnType::ToVector()
{
    std::vector<valueType> result;
    void* valueMemory = nullptr;

    for (int i = 0; i < PyList_Size(value); ++i)
    {
        ToType<valueType>(PyList_GetItem(value, i), &valueMemory);
        result.push_back((valueType)valueMemory);

        valueMemory = nullptr;
    }

    return result;
}
