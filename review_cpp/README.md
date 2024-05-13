## Local Variables and Function Arguments

### local variables
- local variables exist inside a scope
    - a scope is defined by a pair of braces {}
- a local variable comes into existence where it is defined
    - memory is automatically allocated for it on the program's stack
    - it will then be initialized
    - if we do not specify an initial value, it is "default initialized"
- it is destroyed at the end of its scope
```
{       // start of a scope
int i;  // allocate memory for i, default initialize it
...     // code that uses i
}       // end of scope - release memory used by i
// i is no longer "in scope"
```

### pass by value
- by default, a variable which is passed into a function is copied
- the copy is a local variable whose scope is the body of the function
- by default, the returned variable from a function is also copied
```
int func(int y) { // y is a local variable in func which is a copy of the caller's variable x
    return y;   // copy y into the function's return value
}

int x = 2;
int z = func(x);    // z will be a copy of func's return value
```

### pass by address
- with pass by value, any changes made in the function only affect the local variable
- if we want to change the original, we can use pass by address
```
void func(int *y) {     // y will be a pointer to the caller's variable x
    *y = 1
}

func(&x);   // x will now have the value 1
```

### pass by reference
- a refernce behaves like a pointer which is automatically dereferenced when used
```
void func(int& y) {     // y will be a reference to the caller's variable x
    y = 1;
}
func(x);    // x will now have the value 1
```

### pass by const reference
- for read-only access to class objects, passing by const reference is usually more efficient than passing by value
```
class MyClass {...};

void func(const MyClass& mc) { // m will be a reference to the caller's object
    ...     // can call const public member functions of mc
}

MyClass my_class;
func(my_class);     //Pass object to func
```

## Reference and Value Semantics

### Reference semantics
- some languages use reference semantics by default
- when an object is initialized from, another object, or passed as a function argument, a reference is used
- instead of allocating memory for a new object and copying the data into it, the new object shares the memory used by the original object
- the original variable cannot be destroyed until all the other objects which are using its memory have been destroyed
- to manage the lifetimes of all these objects, the language provides a "garbage collector"
- the program keeps track of all the objects in it
- it also keeps track of which objects are using another object's memory
- the program periodically stops executing and goes through all objects and decides which memory allocations it is safe to release

### languages with reference semantics
- avoids the overhead of copying an object's data
- adds overhead for garbage collection
    - the garbage collector does a lot processing and uses memory
    - the program cannot execute any instructions while the garbage collector is running
- memory not released immediately
    - the garbage collector must run and check there are no other references to the object
- cannot predict when, or in what order, objects are destroyed

### c++ semantics
- instead of reference semantics, c++ uses value semantics by default
    - arguments are passed by value
    - initialization creates an entirely new object
    - objects exist only within a scope
- if we want reference semantics, we can have them
    - arguments are passed by reference
    - initialization creates an alias to an existing object
    - heap-allocated objects can exist beyond the end of the scope
- "C++ doesn't have garbage collection because it does not produce much garbage"- Bjarne Stroustrup

### pros and cons of c++ value semantics
- copying objects creates overhead due to copying the object's data
    - mitigated in modern c++
- no garbage collection overhead
- local variables are destroyed deterministically and synchronistically
    - as soon as the program reaches the end of the scope
    - objects are destroyed in the reverse order to creation
    - the memory used by the object becomes available immediately
- allocated memory must be managed by the programmer
    - can be avoided in modern c++

## Declaration and Initialization

### Universal Initialization
- Brace initialization can be used with any type
```
int x{7};       //Equivalent to int x = 7;
string s{"Let us begin"};       // Equivalent to string s ("Let us begin");
```
- it can also be used to initialize containers
```
vector<int> vec{4, 2, 3, 5, 1};     // std::vector variable with elements 4, 2, 3, 5, 1
// instead of
// vector<int>vec;
// vec.push_back(4)
// vec.push_back(2)
//...
```

### advantages of universal initialization
- narrowing conversions are not allowed
```
int x = 7.7     // legal, although compilers may warn
int x{7.7}      // illegal
```

- it is consistent
```
vector<int> old_one(4);     // vector with elements 0, 0, 0, 0
vector<int> old_two(4, 2);   // vector with elements 2, 2, 2, 2
vector<int> uni{4};         // vector with element 4
vector<int> uni{4, 2};      // vector with elements 4, 2
```

- avoids ambiguity
```
Test test();            // Object creation or function declaration?
Test test{};            // Object creation
```

### nullptr
- nulltpr is a literal representing a null pointer
- it has a special type which is compatible with any pointer type, but cannot be converted to an integer
```
void func(int);
void func(int*);

func(nullptr);      // Calls func(int*) as expected
```

- the traditional NULL has value 0
- its type is implementation defined
```
func(NULL);     //Calls func(int*) with clang
                //Calls func(int) with VC++
                //Does not compile with gcc!
```

## Classes

- a class is a compund data structure
- classes can have member functions as well as data members
    - we can choose whcih member functions to have, what their names are, what they do and when they are called
    - these determine the behaviour of objects of the class
    - they are called to perform operations on an object
- by default, access to class members is "private"
    - only objects of the class can access a private member
- class members can be made "public"
    - a public member can be accessed by any code in the program


### Public and Private Members
- the public members provide the interface of the class
    - "What it does"
- the private members provide the implementation of the class
    - "How it does it"
- a struct is the same as a class, except all the members are public by default

### member function implementation
- member functions are implemented as global functions
- when a member function is called on an object, the object is passed by address in a hidden argument
```
Test test;
test.func(1, 2.0, "three");     //Is called as Test::func(&test, 1, 2.0, "three")
```
- the pointer to the object is available as "this" in the function
- dereferencing "this" gives access to members of the object
```
// In the body of Test::func()
this->i = 1;            // Or just i = 1
//"this" is equal to &test
```

## Special Member Functions
- some member functions are "special"
    - they concern the management of objects
    - their names are related to the class's name
    - the compiler will automatically insert calls to these member function when needed
    - in some cases, the compiler will create them for us
- the special member function in Traditional C++ are 
    - constructor
    - copy constructor
    - assignment operator
    - destructor
### Constructor
- has the same name as the class
- initializes a newly created object using its arguments
- performs any initial configuration required
```
// Initializes object from arguments
Test(int i, const string& str) : i(i), str(str) {
    // Allocate memory, connect to database, etc.
}
```

### Copy Constructor
- similar to the constructor, but uses another object for initialization
- always takes one argument
    - a reference to another object of the same class
    ```
    // Initialize object from another Test object
    Test(const Test& other) : i(other.i), str(other.str) {  // Initialize object
        // Configure object (if needed)
    }
    ```
### Assignment Operator
- assigns an exisiting object from another object
- always takes one argument
    - a reference to another object of the same class
- Returns a reference to the assigneed object
    - should be a non-const reference, for consistency with built-types
    ```
    // Assign object from another Test object
    Test& operator = (const Test& other) {
        i = other.i;
        str = other.str;
        return *this;
    }
    ```

### destructor
- called before the class members are released in memory
- performs a ny tidying-up required before the object is destroyed
```
// Prepare object to be released
~Test() {
    // Release allocated memory, close the connection to the database, etc.
}
```

## Pointers and Memory

### pointers
- a pointer is a variable whose value is an address in memory
    - this can be either on the stack or on the heap
- to create a pointer variable, we put a * after the type name
```
int *p;     // The type of p is pointer to int
```

- to initialize a pointer variable, we assign an address to it
```
int i{1};       //i is a stack variable
int *p1 = &i;   //p1 is a pointer to int. its value is the address of i

cout<<p1<<endl;     //displays the address of i
cout<<*p1<<endl;    //displays the value of i
```


### pointers and heap memory
- the new operator allocates memory on the heap and returns the address of the memory
```
int *p2 = new int;      //p2 points to memory allocated from the heap
```
- this will call the default constructor for the class
- for a built-in type, the data will be left uninitialized
- we can also get initialized memory
```
int *p3 = new int{36};      //p3 points to int with initial value 36 (C++11)
int *p3 = new int(36)       // older versions of C++
```

### heap allocated memory
- memory from the heap will remain allocated to the program until it is released
    - if the programmer does not explicitly release it, the memory will remain allocated until the program terminates
- the operating system restricts the amount of memory a program can allocate
    - if a program uses too much memory, the operating system may refuse to allocate any more memory

### memory leak
- failing to release memory when it is no longer needed causes a "memory leak"
```
void badfunc(){
    int *p4 = new{42};      // allocate memory in function
    ...
    return;     // Return without releasing memory
}               // Memory leak!
```

### releasing memory
- the delete operator releases memory that was allocated by new
```
delete p;
```
- this will call the destructor for the object(s) in the memory, then release the allocated memory
    - the p variable will still exist, but represents memory that is no longer accessible by the program
    - p is now a "dangling pointer"
    - attempting to access it will result in undefined behaviour
- for every new operation, there should be a matching delete operation

### array allocation
- we can also allocate a block of memory and access it as if it were an array
```
int *pa = new int[20];
for(int i = 0; i < 20; ++i){
    pa[i] = i;
}
```
- in this case, we have to use a special form of delete to release the memory
```
delete [] pa;
```
- using other form gives undefined behavior

## Array, String and Vector

### Array
- an indexed block of contiguous memory
- inherited from C
- an array can be allocated on the program's stack...
- but only if the number of elements is fixed and known at compile time
```
int arr[5];     //array of 5 ints allocated on the stack
int arr[nElements]; //not allowed in standard C++ for variable nElements
```

### Dynamic Array
- the array must be allocated on the heap if
    - we do not know the number of elements at compile time
    - or we want to be able to vary the number of elements
    ```
    int *pArr = new int[nElements];
    ```
- the array's memory has to be explicitly released when no longer needed
    ```
    delete [] pArr;
    ```

### C-Style String
- A C-style string is an array of const char
- each character of the string is stored in an element in the array
- the array has an null character at the end
- this is used to detect the end of the string
- string literals are C-style strings
```
const char *str = "Hello";      //Equivalent to const char str[] = {'H', 'e', 'l', 'l', 'o', '\0'}
```

### std::string
- std::string is a class
- it has a member which is a dynamic array
- it also has a member which stores the number of elements in the array
```
class std::string {
    char *data;     // Block of contiguous memory
    size_t n;       // Number of elements in the array
    ...
};
```

### using std::string
- std::string objects behave like a dynamic array, but are used like a local variable
    ```
    string hello{"Hello"};      // Allocates storage on heap and populates it
    ```
- contiguous block of memory allocated on heap in constructor
- released in destructor at end of scope
- correctly handles copying and assigning objects, by allocating a new block
- automatically reallocates the memory block when needed

### std::string interface
- subscript notation [] is supported
- elements are indexed, stating from 0
- size() member function returns number of elements

### std::vector
- std::vector is similar to std::string, but can store data of any single type
- the type of the data is a parameter of the class
```
vector<int> vec {4, 2, 3, 5, 1};        // vec is a vector of int
```
- the std::vector class has a member which is a dynamic array
```
class std::vector<int> {
    int *data;      // block of contiguous memory
    size_t n;       // number of elements in the array
    ...
};
```

## Numeric Types and Literals
### Integers
- the size of C++ types depends on the implementation
- char
    - 8 bits
- int 
    - at least 16 bits
- long
    - at least 32 bits. Must have at least as many as int
- long long
    - new in c++11
    - at least 64 bits. Must have at least as many as long

### Fixed-width integers
- introduced in C++11 in <cstdint>
    - int8_t
    - int16_t
    - int32_t
    - int64_t
- unsigned versions
    - uint8_t
    - uint16_t
    - uint32_t
    - uint64_t


### Floating point types 
- float
    - usually 6 digits precision
- double 
    - usually 15 digits precision
- long double 
    - usually 20 digits precision

### digit separator in numeric literals
- digit separator
```
// we can use ' in numberic literals to separate digits (C++14)
const int one_million = 1'000'000;
double pi = 3.141'593

//this can go anywhere inside the number (but not at the start or the end)
const int one_lakh = 1'00'000;
```

### default types
- floating-point literals are double by default
```
3.14159         // literal has type double
```
- integer literals are int by default
    - if too big for int, they are long
    - if too big for long, they are long long
- we can add a suffix to change the type of a literal
```
3.14159f        // literal has type float
1234567890ULL   // literal has type unsigned long long
```

## String literals

### C-Style String Literal
- the traditional string literal is an array of const char, terminated by a null character
```
const char *cca = "Hello, world!";      //C-style string - null-terminated array of const char
```
- very limited range of operations
- only compatible with arrays of the same length

### String literals
- from C++14, we can create std::string literals
- we add 's' after the closing double quote
```
using namespace std::literals;      //C++14 literals have their own namespace

string str = "Hello, string!"s;     // std::string
```
- supports all std::string operations
```
cout<<"Hello"s +", world!"s;    //can perform std::string operations
```
- can be used anywhere that expects an std::string object

### String literals and escape characters
- in a string literal, certain characters can be "escaped"
    - this gives them a different meaning
- this is done by putting a backslash \ in front of them
    \n - newline character in string literal (not character 'n')
    \" - double quote in string literal (not string terminator)
- consider the following

    ```<a href="file">C:\"Program Files"\<\a>\n```
- to write this as a string literal, we need to escape the " and \ character
```
//String literal with escaped characters
string url = "<a href=\"file\">C:\\\"Program Files\"\\</a>\\n";
```

### Raw String Literals
- C++11's raw string literals avoid "backslashitis"
- we put the string literal inside R"(...)"
```
// Raw string literal with unescaped characters
string url = R"(<a href="file">C:\Program Files\</a>\n)";
```
- this prevents characters such as " and \ form being processed
- if the string contains )", we need to add a delimiter
```
// Raw string literal with delimiter x
string delimited_url = R"x(<a href="file">C:\"Program Files (x86)"\</a>\n)x"
```

## Casting
- a cast performs an explicit conversion
- casting can serve several different purposes
    - convert an expression to a different type
    - convert a const expression to the non-const equivalent
    - convert a data in a buffer to untyped binary data
    - convert pointer to base class object to pointer to derived
- casting is rarely necessary in well-written C++ code
- it is often a sign that something "suspicious" is going on

### C-style cast
- the C-style cast puts the type in parentheses
```
int c = 'A';
cout<<c<<endl;      // Prints out 65
cout<<(char)c<<endl;    // Prints out 'A'
```
- this is easy to overlook when reading through code
- it fails to distinguish between the different types of cast

### C++98 casts
- C++98 provided four new ways of casting an expression
    - one for each use
- these have a very distinctive syntax
    - (Type)expr
    - Becomes xyz_cast<Type>(expr)
- "An ugly operation should have an ugly syntactice form"

### static_cast
- static_cast is used to convert an expression to a different type
```
cout<<static_cast<char>(c)<<endl;
```

### const_cast
- const_cast is used to convert a const expression to the non-const equivalent
- may be needed to call functions that aren't const-correct (old or badly written code)
```
void print(char *str) {     //Doesn't mutate str - should be const char *
    cout <<str<<endl;
}

const char *msg="Hello, world!";
print(msg);                     //Error - invalid conversion
print(const_cast<char*>(msg));  //Cast to non-const
```
- undefined behavior if print() does mutate str!

### reinterpret_cast
- reinterpret_cast is used to convert data in a buffer to untyped binary data
- mainly used in low level work (communication with hardware, operating system, binary files) 

### dynamic_cast
- dynamic_cast is used to convert a pointer to a base class object to a pointer to a derived class object
    - also applies to references
- unlike the other types, it is done at runtime

## Iterator Introduction
### loop termination
- when we use a pointer to loop over all elements in an array, the loop terminates after incrementing the pointer when it points to the last element
- the final value of the pointer is not the address of any element in the array
    - using this pointer results in undefined behaviour
    - the pointer represents an imaginary "element after the last element"

### std::string iteration
- with std::string, we can use an iterator in the same way
```
string str{"Hello"};

string::iterator it = /* Iterator to first element */;

while(it != /* Iterator to last element plus one */) {
    cout << *it <<","       //Dereference to get the current element
    ++it;                   //Increment to move to next element
}
```

### begin() and end()
- std::string has two member function which return iterators
- begin()
    - returns an iterator to the first element
- end()
    - returns an iterator corresponding to "the element after the last element"
    - this is an invalid iterator and must not be dereferenced
- these iterators are specific to the object
    - they must not be mixed with iterators to other objects

### iterating over string
- we can finalize our loop
```
string::iterator it = str.begin();      //Start of string

while(it != str.end()) {    // Gone past last element?
    cout<<*it<<","  // Dereference to get the current element
    ++it;           // Increment to move to next element
}
```

- if the container is empty, begin(), and end() return the same value and the loop body is not executed

### auto keyword
- "auto" was originally used in C to specify that a variable should be created on the stack
- in modern C++, it is used to indicate that the compiler should deduce the type from the variable's initial value
```
auto i{42};             //Equivalent to int i{42}
auto str1 = "Hello";    //Equivalent to const char str1[] = "Hello"
auto str2 = "Hello"s;   //Equivalent to std::string str2{"Hello"s}
```

### using auto to simplify your code
- auto is useful for simplifying complex types
```
//write out iterator type in full
vector<int>::iterator it = vec.begin();

//let the compiler deduce the type
auto it = vec.begin();
```

- sometimes it is not easy to work out the type
    - and to type it accurately!
- in modern C++, there are even situations in which it is impossible to know what the type is!

### auto with qualifiers
- auto will only give the underlying type
- const, reference, etc are ignored
```
const int& x{6};
auto y = x;     //Equivalent to int y = x

++y;        //Legal
```
- if we need them, we must add them ourselves
```
const auto& y = x;      //Equivalent to const int& y = x
```

### auto with function return value
- we can also use auto to capture the value returned by a function
```
int func(){         //Function returning reference to const int
    return 5;
}

auto x = func();    //x has type int
```

### when to use auto
- use auto:
- when the type does not matter
- when the type does not provide useful information
- when the code is clearer without the type
```
auto it = vec.begin();      //Clearer than vector<int>::iterator it = vec.begin();
```
- when the type is difficult to discover
    - template metaprogramming
- when the type is unknowable
    - compiler-generated class

### when not ot use auto
- do not use auto:
- if you want a particular type
- if it makes the code less clear
```
auto h = xyz();         //What does h represent??
```

## Loops and Iterators
### const iterator
- if we want to prevent the loop from modifying the string, we can use a const_iterator
```
string::const_iterator cit;

for (cit = str.begin(); cit != str.end(); ++cit)
    cout<<*cit<<",";
```

### reverse iterator
- we can also use a reverse iterator to iterate backwards from the last element
```
string::reverse_iterator rit;
```
- rbegin() returns a reverse iterator to the last element
- rend() returns a reverse iterator corresponding to "the element before the first element"
```
for(string::reverse_iterator rit = str.rbegin(); rit != str.rend(); ++rit)
    cout<<*rit<<",";
```

### const forms of begin() and end()
- modern C++ has const forms of begin() and end()
    - these make it easier to use auto to iterator over containers
- cbegin() and cend() return const iterators
```
string str{"Hello"};
for(auto it = str.cbegin(); it != str.cend(); ++it)     //Deduced as string::const_iterator
    cout<<*it<<",";
```
- crbegin() and crend() return const reverse iterators
```
for(auto it = str.crbegin(); it != str.crend(); ++it)   //Deduced as const_reverse_iterator
    cout<<*it<<",";
```

### non-member begin() amd end()
- C++11 provides begin() and end() global functions
- these are more readable than the member functions
```
for (auto it = begin(str); it != end(str); ++it)
    cout<<*it<<",";
```
- they also work with built-in arrays
```
int arr[] = {1, 2, 3, 4, 5};
for (auto it begin(arr); it != end(arr); ++it)
    cout<<*it<<",";
```
- C++14 adds the other forms
```
for (auto it = cbegin(arr); it != cend(arr); ++it)
for (auto it = rbegin(arr); it != rend(arr); ++it)
for (auto it = crbegin(arr); it != crend(arr); ++it)
```

### range for loops
- special concise syntax for iterating over containers
```
for(auto el : vec)
    cout<<el<<",";          //Prints out each element of vec
```
- this is equivalent to 
```
for(auto it = begin(vec); it != end(vec); ++it) {
    int el = *it;       //el is a copy of the current element
    cout<<el<<",";
}
```
- any changes which are made to "el" do not affect the container elements

### range for loops with modification
- to modify elements, we need to use a reference
```
for(auto& el : vec)
    el += 2;            //add 2 to each element of vec
```

- this is equivalent to 
```
for(auto it = begin(vec); it != end(vec); ++it) {
    int& el = *it;      //el is a reference to the current element
    el += 2;
}
```

### When to use range for loops
- range for loops are only suitable if we wish to visit each element once, in order, without adding or removing elements
- otherwise, we need to use a traditional loop

### Iterator Arithmetic
- we can perform arithmetic on iterators
    - similar to pointers
- adding to an iterator moves it towards the back of the container
    ```
    auto second = begin(str)+1;         //Iterator to second element
    ```
- subtracting from an iterator moves it towards the front of the container
    ```
    auto last = end(str) - 1;           //Iterator to last element
    ```
- end() - begin() gives the number of elements
    ```
    auto mid = begin(str) + (end(str) - begin(str))/2;  //Iterator to middle element
    ```
### Iterator arithmetic functions
- next() takes an iterator and returns the following iterator
    ```
    auto second = next(begin(str));     //Returns iterator to second element
    ```
- prev() takes an iterator and returns the previous iterator
    ```
    auto last = prev(end(str));         //Returns iterator to last element
    ```
- distance() returns the number of steps needed to go from its first argument to its second argument
    ```
    distance(begin(str), end(str));     //Returns number of elements
    ```
- advance() moves an iterator by its second argument
    ```
    auto mid = begin(str);
    advance(mid, distance(begin(str), end(str))/2); //mid is iterator to middle
    ```

### Half-open range
- Imagine we have a loop with an integer loop countewr
    ```
    for(int i = 0; i < 10; ++i)
        ...
    ```
- In the loop body, 'i' has an allowed range of values
    - 0, 1, 2, ..., 9 but not 10
- This is known as a "half-open" range
- it is written [0, 10)
    - i >=0 must be true and i < 10 must be true
    - i == 10 is not allowed

### Iterator Ranges
- the iterators returned by begin() and end() define an interator range
- if we have a loop with an iterator as loop counter
    ```
    for(auto it = begin(str); it != end(str); ++it)
        ...
    ```
- in the loop body, 'i has an allowed range of values
    - begin(), begin()+1, begin()+2, ..., but not end()
- this is a "half-open" range
    - it >= begin() must be true and it < end() must be true
    - it == end() is not allowed


## If Statements and Switch in C++17

### Initializer in for Statement
    ```
    int i;          // i is in the enclosing scope
    for(i=0; i < size; ++i){
        ...
    }

    for(int i = 0; i < size; ++i) { //C++98
        ...                         //i is local to the loop
    }
    ```
- Encapsulation
    - Loop counter is an "implementation detail" of the loop
    - Loop counter cannot be used outside the loop's scope (avoid's potential bugs)
    - with multiple loops in the same scope, each loop counter can have the same name
- Code is clearer and more concise

### Initializer in if Statement
    ```
    auto iter = begin(vec);
    if(iter != end(vec))            //Check container is not empty
        ...
    ```
- C++17 allows an initializer in an if statement
    ```
    if(auto iter = begin(vec); iter != end(vec))    //Check container is not empty
        ...
    ```
- This has similar advantages to the for loop initializer
- 'iter' is local to the if statement
    - this includes the "else" block

### Initializer in Switch Statement
    ```
    const char c = get_next_char();
    switch(c){
        ...
    }
    ```
- C++17 also allows initializers in a switch statement
    ```
    switch (const char c = get_next_char(); c) {
        ...
    }
    ```
- Again, the 'c' variable is local to the switch statement

### Falling through case labels deliberately
- if there is no break at the end of a case label, the program flow "falls through" to the nest label
- often this is useful
    ```
    switch(c) {
        case '':        // c is space character
                        // Fall through to next case
        case '\t':      // c is tab character
                        // Fall through to next case
        case '\n':      // c is newline character
            ++ws_count; // Increment whitespace counter
            break;
    }
    ```

### Falling through case labels accidentally
- however, it can easily be done by mistake
```
switch (employee_location) {
    case HEAD_OFFICE:
        vehicle_type = Mercedes;
                                    // No break - fall through to next case
    case SALES_OFFICE:
        vehicle_type = Peugeot;
                                    // No break - fall through to next case
    case WAREHOUSE:
        vehicle_type = Truck;       // All employees have this vehicle type!
}
```

### Fallthrough Attribute
- C++17 provides a [[fallthrough]] attribute
- This is used with an empty statement
- it indicates that the break statement is intentionally omitted
- the compiler will not give a warning
    ```
    switch(c){
        case '':
            [[fallthrough]];        //Fall through to next case - not a mistake!
        case '\t':
            [[fallthrough]];        //Fall through to next case - not a mistake!
        case '\n':
            ++ws_count;
            break;
    }
    ```

## Template Overview

### Why are templates useful?
- we often need to write code which is functionally the same, but operates on different types of data
    - vector of int
    - vector of double
    - vector of string
- C++ templates allow us to write code that works with any type of data
- this is known as "generic programming"

### Template Instantiation
- when we use the template with data of a particular type, the compiler will generate the code needed for that type
    - e.g. vector<int> will cause the compiler to define a class that is a vector of ints
    - the compiler will insert the source code for the class definition into the translation unit
    - this code will then be compiled as part of the program
- this is called an "instantiation" of the template
- it happens automatically when the compiler sees vector<int>
- for this to be possible, the compiler must be able to see the full definition of the vector template class in the translation unit

### Writing a template
- When we write a template, we use a "dummy" type to show the compiler what the code looks like
- this dummy type is called "the template parameter"
- a template can be either a function template or a class template
    ```
    // Function template for finding maximum of two values
    template <class T>          //T is the parameter type
    T Max(const T& t1, const T& t2) {   //The arguments and return value have this type
        if(t1 > t2)
            return t1;
        return t2;
    }
    ```

### Using a template
- When we call Max(), the compiler will instantiate the function from the template code, with T replaced by the type of the arguments
    ```
    cout<<Max(7.1, 2.6)<<endl;
    ```
- The compiler deduces that the argument type needs to be double
- The compiler will generate a definition of the Max function, in which T is replaced by double
    ```
    double Max(const double& t1, const double& t2){ //Replace "T" with "double"
        if(t1 > t2)
            return t1;
        return t2;
    }
    ```

### Templates and Code Organization
- with a regular function, the compiler only needs to be able to see its declaration when it is called
    - the compiler has to check that the types are used correctly in the call
- for a template function, the compiler must be able to see the full definition when it is called
    - the compiler has to generate the instantiated code for that call
- most programmers write the template definition in the header file, so it is included automatically
- some programmers put all their templates in a separate ".inc" file and include that file separately

### Class Template
- we can write classes that work with data of any type
    ```
    template<class T>
    class Test {
        T data;
        Test(const T& data) : data(data) {}
    };
    ```
- To create an instance of this class, we put thw type in angle brackets
    ```
    Test<string>test{"Hello"};

    class Test_xcajkjha {           // Instantiated with unique name
        string data;
        Test(const string& data) : data(data) {}
    };
    ```

### Constructor Argument Deduction in C++17
- when we create an object of templated class, the C++17 compiler can deduce the parameter type
    ```
    vector<int> vec{1, 2, 3};       //C++11 - declared as vector<int>
    vector vec{1, 2, 3};            //C++17 - deduced as vector<int>
    ```

- This is known as "CTAD"
    - Constructor Template Argument Deduction
    - Similar to calling a templated function
    - The declaration must be initialized
    - The compiler will deduce the type from the initializer

- CTAD makes many declarations simpler

### typename
- originally, the class keyword had to be used for a template parameter
    ```
    template<class T>
    ```
- the typename keyword was added in C++98
    ```
    template<typename T>
    ```
- the "class" keyword was felt to be confusing as templates can be instantiated for built-in types as well as classes
- however, many programmers still continue to use it

### Namespace
- a namespace is used to group together logically related symbols
    - Typically done within libraries
- the C++ standard library defines teh std namespace
- this groups together the names of all the functions, types and variables defined by it.


### Why Namespaces?
- large programs often include several libraries
    - this can lead to name conflicts
    - e.g. two libraries could define a class Test
- one solution is to add a library-specific prefix to the name
    - class abc_Test;       // In the Alpha Beta Corporation's library
    - class xyz_Test;       // In the Xylum Yield Testing library
- putting in these prefixes is tedious and error-prone
- C++ addresses this by using namespaces


### Namespace example
- we create a namespacex using the namespace keyword, followed by the name we wish to use for the namespace
    ```
    //Namespace for the Alpha Beta Corporation's library
    namespace abc{...}
    ```
- we put the symbols we want to declare in this namespace inside the braces
- every symbol declared inside the namespace will have the namespace's name automatically prefixed to it by the compiler
    ```
    namespace abc {
        clads Test;         // This defines the symbol abc::Test
    }
    ```
- if we want to use a symbol outside the namespace, we put abc:: in front of it
    ```
    // namespace for the Alpha Beta Corporation's library
    / all symbols defined in here will have abc:: in front of them
    namespace abc {
        class Test {...};       // This defines class abc::Test
    }

    abc::Test alphaTest;        // Create an object of Alpha's class Test
    ```

### Global namespace
- if a name is not in any namesapce, it is said to be in the "global namespace"
    - if we define a symbol which is not in any namespace, the compiler will assme it is in the global namespace
- the global namespace has no name
- if we want to specify that we are referring to the global namespace we use :: on its own before the name
    ```
    class Test {...};       //Another definition of class Test (not in a namespace)
    ::Test test;            //Create an object of the global class Test
    ```

### Namespace Splitting
- namespaces can be split over different parts of the code, and even over different files
    ```
    //In Alpha Beta Corporation's Test.h:
    namespace abc {
        class Test {...};           //Definition of class Test in header
    }

    //In Alpha Beta Corporation's Test.cc
    namespace abc {
        in Test::do_test(int value) const {...}     //Member function definition
    }
    ```

### Name Hiding
- when a symbol is defined in a namespace, it "hides" any symbols outside the namespace with the same name
    ```
    int x = 23;         // x defined in the global namespace

    namespace abc {
        int x = 47;     // x defined in the namespace abc - hides global x
        void func() {
            cout << "x = " <<x<<endl;   //Will use abc's x => 47
            cout <<"::x ="<<::x<<endl;  //Will use global x => 23
        }
    }
    ```

### Using Declarations
- A "using" declaration will bring a particular version of class Test into the global namespace
    ```
    using xyz::Test;        //Now "Test" will refer to Xylum's class Test
    ```
- this will make xyz::Test available as "Test"
    ```
    Test            //Xylum's class Test
    abc::Test       //Alpha's class Test
    ::Test          //Global class Test
    ```
- this takes effect from the using declaration until the end of the scope

### Using Directive
- a "using" directive will bring everything from a namespace into the global namespace
- not good practice as it contradicts the point of having namespaces
    ```
    using namespace std;        //Bring in all names from std namespace
    //Now we can refer to std::cout as cout, and so on
    cout<<"Look, no qualifiers!"<<endl;
    ```

### Function Pointer
- a function's executable code is stored in memory
- we can get a pointer whose address is the start of this code
    ```
    void func(int, int);
    auto func_ptr = &func;
    //void(*func_ptr)(int, int) = &func;
    ```
- func_ptr is a pointer to the function
- we can use a type alias for a function pointer's type
    ```
    using a pfunc = void(*)(int, int);
    ```

### Callable Object
- a function pointer is a "callable object"
    - behaves like a variable
    - can be called like a function
- we can call the function by dereferencing the pointer
    - (*func_ptr)(1, 2);
- a function pointer is a "first class object"
    - we can pass a function pointer as argument to another function
    - we can returna function pointer from a call to another function

### Passing a function pointer
- we can pass a function pointer as an argument to another function
    ```
    void some_func(int, int, pfunc);
    ```
- the function can call the pointed-to function in its body
    ```
    void some_func(int x, int y, pfunc func_ptr) {
        (*func_ptr)(x, y);
    }
    ```
- we can return a function pointer from a function call
    ```
    pfunc other_func(...);
    ```

### Pros and Cons of Function Pointers
- Function pointers were inherited from C
- Useful for writing callbacks
    - Operating systems, GUI's, event driven code
    - Provide a function that will be called when "something" happens
- "Raw" pointer
    - Can be overwritten, null or uninitialized
- Ugly syntax















