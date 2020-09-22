# About Python Interface
It provides Python code into C++ while preserving the structure of the code.

# Features
- Calling functions.
- Calling classes.
- Calling class methods.
- Calling class static methods.
- Types conversion from Python to C++ and backwards.

# Supporting types
Function arguments can be represented by _bool, char, int, double(also pointers of those types), and also const char*_.

Function return value can be converted to types which described above(except pointers) and also in _std::multimap_ if python method returning dictionary value.


# Usage Example

Python file:
```Python
class A:
    def doSomething(self, value: int) -> int:
        return value
```

And it needs to be run in C++ code:

```Cpp
class A : private Class
{
public:
    int doSomething(int value);
}


int A::doSomething(int value)
{
    Function::Arguments arg(value);
    return this->callMethod("doSomething", arg).ToLong();
}


int main()
{
    A* instance = new A();
    std::cout << instance->doSomething(777) << std::endl; //It will print 777.
}

```
