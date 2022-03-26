# C++

C++ is the evolution of C, beginning with support for object-oriented
programming paradigm, through classes.

- C: procedural language
- C++: object-oriented language

## testing frameworks

- [gtest](gtest.md): Google test
- gmock: Google mock
- [Catch2](catch2.md)

Hint: implement a debug method for each class, where there is only one generic
class, where it prints the correct method.

```cpp
static class debug {
	void printinfo(int level, objeto) {
		objeto.printinfo(level);
	}
}

virtual class internaldebug {
	void printinfo(int level);
}

class parser : internaldebug {
	void printinfo(int level) {
		switch(level) {
			cout << "teste\n";
		}
	}
}

int main() {
	classe objeto;
	debug.printinfo(objeto);
}
```

## IO stream

- `cout` = c out
- `cin` = c in
- `cerr` = c error
- `<<` = put to operator
- `>>` = get from operator
- `getline(cin, string_variable)` = read from `cin` until return

```cpp
bool accept ( )
{
	cout << "Do you want to proceed (y or n) ?\n"; // write question
	char answer = 0;
	cin >> answer; // read answer
	if (answer == ' y ') return true;
	return false;
	cerr << s.example();
}
```

## namespace

C++ provides a mechanism for grouping related data, functions, etc., into separate namespaces.
A and B will not, but not C

```cpp
// A

#include <iostream>

int main()
{
	std::cout << "Hello, new world!\n";
}

// B

#include <iostream>
using namespace std;

int main()
{
	cout << "Hello, new world!\n";
}

// C

#include <iostream>

int main()
{
	cout << "Hello, new world!\n";
}
```

`namespace` usage example:

```cpp
namespace Stack { // interface
	void push (char);
	char pop ();
	class Overflow { }; // type representing overflow exceptions
}

void f()
{
	Stack::push ('c') ;
	if (Stack::pop() != 'c') error ("impossible");
}
```

The definition of the `Stack` could be provided in a separately-compiled part of
the program:

```cpp
namespace Stack { // implementation
	const int max_size = 200;
	char v[max_size];
	int top = 0;

	void push (char c)
	{
		/* check for overflow and push c */
	}

	char pop ()
	{
		/* check for underflow and pop */
	}

	class Overflow // type representing overflow exceptions
	{
		// ...
	};
}
```

## inline

Inline function are like preprocessor `#define` in the sense that they are
declared as a function, but they expand wherever they are called. This is useful
for short functions when the trouble of calling the function is greater than
the function itself.

## reference `&` operator

A reference is an alias for a variable. You always need to initialize it to a
variable.

```cpp
int data = 0;
int& mydata = data;
```

### return value from function by reference `&`

```cpp
void myfunc(int a, int& b);
```

The `b` parameter above is a variable passed by reference. This means that `b`
will be the same on `myfunc` as well as the calling function. This is like
passing the address of the variable:

```cpp
void myfunc(int a, int* b);
```

With the exception that the `int& b` can be used as a simple variable instead of
as a pointer.

### const keyword

#### on function

`const int foo(int y)` declares the return as a constant.

#### on method

A const method, denoted with the keyword const after a function declaration,
makes it a compiler error for **this method** to change a member variable of
the class. However, reading of a class variables is okay inside of the function,
but writing inside of this function will generate a compiler error.

- `int Foo::Bar(int random_arg)` -> `int Foo_Bar(Foo* this, int random_arg)`, is
  called `Foo f; f.Bar(4)` corresponds to `Foo f; Foo_Bar(&f, 4)`
- `int Foo::Bar(int random_arg) const` -> `int Foo_Bar(const Foo* this, int
  random_arg`).

To allow the "`const` method" to write to any variable of a class use the
`mutable` keyword (C++11).

#### on pointers

```cpp
int a = 1, b = 2;
const int* p = &a; // constant value
cout << "*p = " << *p << ", p = " << p << endl;
p = &b;
cout << "*p = " << *p << ", p = " << p << endl;

int* const pp = &a; // constant reference
cout << "*pp = " << *pp << ", pp = " << pp << endl;
*pp = 9;
cout << "*pp = " << *pp << ", pp = " << pp << endl;

const int* const ppp = &a; // constant value & reference
cout << "*ppp = " << *ppp << ", ppp = " << ppp << endl;

/*
*p = 1, p = 0x7ffda2303970
*p = 2, p = 0x7ffda2303974
*pp = 1, pp = 0x7ffda2303970
*pp = 9, pp = 0x7ffda2303970
Invalid:
  *p = 6;
  pp = &b;
  ppp = &b;
  *ppp = 6;
*/
```

One way to think about this is to read it from right to left. E.g.:

- `int const*`: pointer to `const int`
- `int *const`: **const pointer** to `int`
- `int const* const`: **const pointer** to `const int`
- `int ** const`: **const pointer** to **pointer** to an `int`
- `int * const *`: **pointer** to `const` **pointer** to an `int`
- `int const **`: **pointer** to a **pointer** to a `const int`
- `int * const * const`: **const pointer** to a **const pointer** to an `int`

#### on reference

Useful for making an alias which cannot change the variable, but still having a
changeable variable:

```cpp
int data = 0;
const int& aliasData = data;
data = 1; // allowed
aliasData = 2; // not allowed
```

## testing for type

Without any libraries, the following will work.

```cpp
#define IS_VAR_MY_TYPE(var) (typeid(var).name() == typeid(my_type_t).name())
```

## allocating and deallocating

Do not use `malloc` or `free`. Use instead:

```cpp
int *ptr;
ptr = new int;
delete ptr;

myClass *ptr;
ptr = new mytype[max];
delete[] ptr;

myClass *dynamic = new myClass();
delete dynamic;
```

The `delete` operator needs to be applied to the base address of the variable.

Typically, `new` will throw a `std::bad_alloc` exception, which can be handled
as:

```cpp
#include <iostream>
using namespace std;

int main()
{
	try
	{
		// Request lots of memory space
		int* pAge = new int [536870911];

		// Use the allocated memory

		delete[] pAge;
	}
	catch (bad_alloc)
	{
		cout << "Memory allocation failed. Ending program" << endl;
	}
	return 0;
}
```

However, if you prefer the `NULL` style of exception handling, use the
`new(nothrow)` variant:

```cpp
#include <iostream>
using namespace std;

int main()
{
	// Request lots of memory space, use nothrow version
	int* pAge = new(nothrow) int [0x1fffffff];

	if (pAge) // check pAge != NULL
	{
		// Use the allocated memory
		delete[] pAge;
	}
	else
		cout << "Memory allocation failed. Ending program" << endl;

	return 0;
}
```


## class

Classes are essentially the same as structs, differing only in the default
access type (structs default to public, whereas class to private). Example:

```cpp
class complex {
// defaults to private
	double re, im;
public:

	complex(double r, double i) { re=r; im=i; } // alternative A

	/*complex(double r, double i) // alternative B
		: re (r), im (i) {}; */

	complex(double r) { re=r; im=0; }
	complex() { re=im=0; } // a constructor
	~complex() { delete[] re; delete[] im }; // a deconstructor
	complex operator+(complex, complex) {...};
	// ...
}
```

Functions declared inside a class are also called methods.
Static methods can only access static fields.

## static keyword uses

`static` means different things depending on the context, but it always has to
do with having a single thing from it. Static elements have their lifetime until
the end of the program, being deallocated after `main()` is complete.
Static are "file defined", meaning that a local copy in each file is made: two
files using the same static element name will use a separate static element
copy.

- as a local variable: the description above.
- as a function: defines the scope of the function to only the file itself
  (reuse name in multiple files)
- as a class variable: variable is shared across all objects instances
  (literally the same variable for every object)
- as a class method: makes the function independent of a single object
- as object: description above.

For both class variable and class method, element is independent of the
existence of an instantiation and may be access through the scope operator
(`::`).

```cpp
using namespace std;

class Box {
private:
	double length, breadth, height;

public:
	static int objectCount;

	Box(double l = 2.0, double b = 2.0, double h = 2.0) {
		cout <<"Constructor called." << endl;
		length = l;
		breadth = b;
		height = h;

		objectCount++;
	}
	double Volume() {
		return length * breadth * height;
	}
	static int getCount() {
		return objectCount;
	}
};

// Initialize static member of class Box
int Box::objectCount = 0;

int main(void) {
	cout << "Inital Stage Count: " << Box::getCount() << endl;

	Box Box1(3.3, 1.2, 1.5);
	Box Box2(8.5, 6.0, 2.0);

	cout << "Final Stage Count: " << Box::getCount() << endl;

	return 0;
}

/*
 * prints:
 * Inital Stage Count: 0
 * Constructor called.
 * Constructor called.
 * Final Stage Count: 2
 */
```

## constructor \& deconstructor

```cpp
class myClass {
	myClass(); // default constructor
	myClass(int a, int b = 1); /* overloaded constructor, with default
		paramenter */
	myClass(const myClass& CopySource); // copy constructor
	myClass(myClass&& MoveSource); // move constructor (C+11)
	~myClass(); // deconstructor
}
```

- The default constructor is the one which does not need any input, meaning it
  can be one with *optional* parameters
- If you have a constructor with parameters and no default constructor, you are
  forcing the class user to use the constructor with parameters
- the copy constructor is used to instruct how a function makes a local
  copy
- copy constructor is useful for ensuring that copies will not point to the same
  address location
- if `&` was not in the copy construct, the problem would be recursive
- copy constructors cannot be `virtual`, create an emulated function instead

### move constructor

```cpp
class MyString {
	char* Buffer;
	// copy constructor
	GetString();
	// something
};

MyString Copy(MyString& Source)
{
	MyString CopyForReturn(Source.GetString());  // create copy
	return CopyForReturn;  // return by value invokes copy constructor
}

int main()
{
	MyString sayHello ("Hello World of C++");
	MyString sayHelloAgain(Copy(sayHello)); // invokes 2x copy constructor
	return 0;
}
```

This creates two copies of Buffer. This is avoidable with the move constructor
(C++11):

```cpp
class MyString {
	// some stuff
	// move constructor
	MyString(MyString&& MoveSource)
	{
		if(MoveSource.Buffer != NULL)
		{
			Buffer = MoveSource.Buffer; // take ownership i.e.  ‘move’
			MoveSource.Buffer = NULL;   // set the move source to NULL i.e. free it
		}
	}
}

// Copy function

int main() {
	MyString sayHelloAgain(Copy(sayHello)); /* invokes 1x copy constructor, 1x
		move constructor */
	return 0;
}
```

## inhibit object copy

If you need to ensure that you can only have one copy of a class, make the copy
constructor and the `=` operator private.

```cpp
class President
{
private:
	President(const President&); // private copy constructor
	President& operator= (const President&); // private copy assignment operator
	// stuff
}
```

But this does not disallows for instantiations as such:

```cpp
President one, two, three;
```

For this, you have to use **singletons**:

```cpp
class President
{
private:
	// private default constructor (prohibits creation from outside)
	President() {};
	//  private copy constructor (prohibits copy creation)
	President(const President&);

	// private assignment operator (prohibits assignment)
	const President& operator=(const President&);

	// member data: Presidents name
	string Name;

public:
	// controlled instantiation
	static President& GetInstance()
	{
		// static objects are constructed only once
		static President OnlyInstance;

		return OnlyInstance;
	}

	// public methods
	string GetName()
	{
		return Name;
	}

	void SetName(string InputName)
	{
		Name = InputName;
	}
};

int main()
{
	President& OnlyPresident = President::GetInstance();
	OnlyPresident.SetName(“Abraham Lincoln”);

	// President Second; // cannot access constructor
	// President* Third= new President(); // cannot access constructor
	// President Fourth = OnlyPresident; // cannot access copy constructor
	// OnlyPresident = President::GetInstance(); // cannot access operator=
	return 0;
}
```

## forcing object instantiation on heap (not stack)

The key is to declare the deconstructor as private.

```cpp
class MonsterDB
{
private:
	~MonsterDB(); // private deconstructor
public:
	static void DestroyInstance(MonsterDB* pInstance)
	{
		// static member can access private destructor
		delete pInstance;
	}
}

int main()
{
	// MonsterDB myDatabase; // compile error
	MosnterDB* myDatabase = new MonsterDB(); // no error

	// delete pMyDatabase; // private destructor cannot be invoked
	MonsterDB::DestroyInstance(pMyDatabase);

	return 0;
}
```

## static methods

Normally, the keyword `this` is implied. However, the `static` keyword applied
to methods means that it is not connected to a single instance, but to all. You
have to specify it by writing.

## class inheritance

```cpp
class A
{
	public:
		int x;
	protected:
		int y;
	private:
		int z;
};

class B : public A
{
	// x is public
	// y is protected
	// z is not accessible from B
};

class C : protected A
{
	// x is protected
	// y is protected
	// z is not accessible from C
};

class D : private A    // 'private' is default for classes
{
	// x is private
	// y is private
	// z is not accessible from D
};
```

[CMake: public vs private vs
interface](https://leimao.github.io/blog/CMake-Public-Private-Interface/)

### access specifiers

access specifier|description
-|-
public|Members are accessible from outside the class
protected|Members cannot be accessed from outside the class. However, they can be accessed in inherited classes
private|Members cannot be accessed from outside the class. However, they can be accessed in inherited classes

access specifier|same class|derived class|outside class
-|-|-|-
public|yes|yes|yes
protected|yes|yes|no
private|yes|no|no

inheritance type| description
-|-
public|public of base class become public of derived; protected of base class become protected members of derived class; a base class's private are never accessible
protected|public and protected members of base class become protected members of the derived
private|public and protected members of base class become private members of the derived

Inheritance Type|base: public|base: protected|base: private
-|-|-|-
public|derived: public|derived: protected|-
protected|derived: protected|derived: protected|-
private|derived: private|derived: private|-

## method overriding

```cpp
class Fish
{
private:
	bool FreshWaterFish;

public:
	// Fish constructor
	Fish(bool IsFreshWater) : FreshWaterFish(IsFreshWater){}

	void Swim()
	{
		if (FreshWaterFish)
			cout << “Swims in lake” << endl;
		else
			cout << “Swims in sea” << endl;
	}
};

class Tuna: public Fish
{
public:
	Tuna(): Fish(false) {}

	void Swim()
	{
		cout << “Tuna swims real fast” << endl;
	}
};

class Carp: public Fish
{
public:
	Carp(): Fish(true) {}

	void Swim()
	{
		cout << “Carp swims real slow” << endl;
		Fish::Swim();
	}
};

int main()
{
	Carp myLunch;
	Tuna myDinner;

	myLunch.Swim(); // method provided by Carp class
	myDinner.Swim(); // method provided by Tuna class
	myDinner.Fish::Swim(); // method provided by Fish base class

	return 0;
}
```

Be careful with overriding inherited methods. If in your base class you have an
overloaded method and this method is overridden in the derived class, every
instance of the method from the base class is overridden. Use something like
this:

```cpp
class Tuna: public Fish
{
public:
	void Swim(bool FreshWaterFish)
	{
		Fish::Swim(FreshWaterFish);
	}
	void Swim()
	{
		cout << “Tuna swims real fast” << endl;
	}
}
```

Or simply:

```cpp
class Tuna: public Fish
{
public:
	using Fish::Swim;

	void Swim()
	{
		cout << “Tuna swims real fast” << endl;
	}
}
```

## virtual methods

Use of keyword virtual means that the compiler ensures that any overriding
variant of the requested base class method is invoked.

```cpp
class Fish
{
public:
	void Swim()
	{
		cout << "Fish swims!" << endl;
	}
	virtual void Hello()
	{
		cout << "Fish says: hello" << endl;
	}
};

class Tuna:public Fish
{
public:
	// override Fish::Swim
	void Swim()
	{
		cout << "Tuna swims!" << endl;
	}
	// override virtual Fish::Hello
	void Hello()
	{
		cout << "Tuna says: hello" << endl;
	}
};

void MakeFishSwim(Fish& InputFish)
{
	// calling Fish::Swim
	InputFish.Swim();
	// calling Tuna::Hello
	InputFish.Hello();
}

int main()
{
	Tuna myDinner;

	myDinner.Swim();
	MakeFishSwim(myDinner);

	return 0;
}
```

Returns:

```
Fish swims!
Tuna says: hello
```

**You should always apply the `virtual` keyword to the destructor:**

```cpp
class Fish
{
public:
	virtual ~Fish()   // virtual destructor!
	{
		cout << “Destroyed Fish” << endl;
	}
};

class Tuna:public Fish
{
public:
	~Tuna()
	{
		cout << “Destroyed Tuna” << endl;
	}
};

void DeleteFishMemory(Fish* pFish)
{
	delete pFish;
}

int main()
{
	cout << “Allocating a Tuna on the free store:” << endl;
	Tuna* pTuna = new Tuna;
	cout << “Deleting the Tuna: “ << endl;
	DeleteFishMemory(pTuna);

	cout << “Instantiating a Tuna on the stack:” << endl;
	Tuna myDinner;
	cout << “Automatic destruction as it goes out of scope: “ << endl;

	return 0;
}
```

Returns:

```
Allocating a Tuna on the free store:
Deleting the Tuna:
Destroyed Tuna
Destroyed Fish
Instantiating a Tuna on the stack:
Automatic destruction as it goes out of scope:
Destroyed Tuna
Destroyed Fish
```

Without `virtual`, "Destroyed Tuna" would not be in the output.

If you want to force for a derive class to implement a particular function, but
not implement it on the base, use the pure virtual function:

```cpp
virtual void Swim() = 0;
```

This creates a so called "Abstract Base Class" (ABC).

## Pure virtual methods

Normally, if you have a class and base class implementing the same method, and
you call the method from the derived class, it will access the pointer class
method. The virtual keyword tell to invert this behavior, by using the pointed class method.

```c++
#include <iostream>
using namespace std;

class Base {
public:
    void NonVirtual() {
        cout << "Base NonVirtual called.\n";
    }
    virtual void Virtual() {
        cout << "Base Virtual called.\n";
    }
};
class Derived : public Base {
public:
    void NonVirtual() {
        cout << "Derived NonVirtual called.\n";
    }
    void Virtual() {
        cout << "Derived Virtual called.\n";
    }
};

int main() {
    Base* bBase = new Base();
    Base* bDerived = new Derived();

    bBase->NonVirtual();
    bBase->Virtual();
    bDerived->NonVirtual();
    bDerived->Virtual();
}
```

Output:

```
Base NonVirtual called.
Base Virtual called.
Base NonVirtual called.
Derived Virtual called.
```

So a `virtual` function specialization actually overwrites the one in the base class.
If we added `= 0` (aka purity), `class Base` would not be instantiable.

## overwrite

[virtual, final and
override](https://www.fluentcpp.com/2020/02/21/virtual-final-and-override-in-cpp/)

`override` is about expressing your intentions to the compiler. If you tag a method , the compiler
will make sure that the method exists in the base class and prevent the program from compiling
otherwise. It is particularly useful for `const` methods.

```c++
class Base
{
public:
    virtual void f()
    {
        std::cout << "Base class default behaviour\n";
    }
};

class Derived : public Base
{
public:
    void f() const override
    {
        std::cout << "Derived class overridden behaviour\n";
    }
};
```
`void f()` and `void f() const` are two different prototypes, and the derived class no longer
overrides the method of the base class. Indeed, the overriding of a method in C++ is based on
prototype (`void f()` = name + signature) and not just on the name of the method (`f()`).
`override` prevents the above code from compiling in the first place.

Adding `virtual` creates a new virtual method that could be overridden in the derived classes of
`Derived` itself. It doesn't check that `f` in `Derived` overrides `f` in `Base`.

## final

You can tag your `virtual` member function with `final`.

```c++
class Base
{
public:
    virtual void f()
    {
        std::cout << "Base class default behaviour\n";
    }
};

class Derived : public Base
{
public:
    void f() final
    {
        std::cout << "Derived class overridden behaviour\n";
    }
};
```

This prevents any derived class of Derived (`MoreDerived : public Derived`) to override the member
function `f`.
It can also apply to types:

```c++
class X final;
```

This prevent the type to be inherited from.

## slicing

Then you also have the possibility of doing something like this:

```cpp
Circle MyCircle;
Shape MyShape = MyCircle;
```

This would copy only the shared members.

## private and protected inheritance

If you want everything (including the public) from base class to be **X** in
the derived class, use:

```cpp
class Circle : **X** Shape {
	/* stuff here */
}
```

**X** may be either `private` or `protected`. This means that everything the
base class will be considered as a member with access type **X**.

Remember:

- `privite` and `protected` inheritance means "has-a"; whereas
- `public` inheritance means "is-a"

## multiple inheritance

```cpp
class Derived: access-specifier Base1, access-specifier Base2 {
	/* some stuff */
}
```

## friend keyword

The `friend` keyword associated with function, allows them to have access to
private and protected members while not belonging to the class.

```cpp
class Box {
public:
  friend void printWidth( Box box );
  void setWidth( double wid );
};

// Member function definition
void Box::setWidth( double wid ) {
	...
}

// printWidth() is not a member function of any class
void printWidth( Box box ) {
   /* Because printWidth() is a friend of Box, it can
   directly access any member of this class */
   cout << "Width of box : " << box.width <<endl;
}

// Main function for the program
int main() {
   Box box;

   // set box width without member function
   box.setWidth(10.0);

   // Use friend function to print the wdith.
   printWidth( box );

   return 0;
}
```

Note: to make every method within a class as a friend, just use friend keyword
in front of class declaration.

## containers, template

Important: a template is a template, it is not the object it is describing.
Declaring the template object in a header file and then implement in the source
will cause [compilation
issues](https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file).

Uses the `template` and `typename` keywords. For example:

```cpp
#include <iostream>
using namespace std;

// One function works for all data types.  This would work
// even for user defined types if operator '>' is overloaded
template <typename T>
T myMax(T x, T y)
{
   return (x > y)? x: y;
}

int main()
{
  cout << myMax<int>(3, 7) << endl;  // Call myMax for int
  cout << myMax<double>(3.0, 7.0) << endl; // call myMax for double
  cout << myMax<char>('g', 'e') << endl;   // call myMax for char

  return 0;
}
```

Notes:

- if `typename` is a class, then it should state `template <class T>`
- templates may also contain normal types, eg: `template <typename T, int
  max_size>`

When using template inside a class:

```cpp
class test {
	template<typename A>
	int<A> f() {}
}

test Test;
test.template f<int>();
```

## exception handling

Exception handling in C++ consist of three keywords: try, throw and catch:
- try statement allows you to define a block of code to be tested for errors while it is being executed
- throw keyword throws an exception when a problem is detected, which lets us create a custom error
- catch statement allows you to define a block of code to be executed, if an error occurs in the try block

```cpp
try {
	// block of code to try
	int age = 15;
	if (age > 18) {
		cout << "Access granted - you are old enough.";
	} else {
		// throw an exception when a problem arise
		throw (age);
	}
}
catch (int myNum) {
	// block of code to handle errors
	cout << "Access denied - You must be at least 18 years old.\n";
	cout << "Age is: " << myNum;
}
```

Another example:

```cpp
MyData md;
try {
	// Code that could throw an exception
	md = GetNetworkResource();
}
catch (const networkIOException& e) {
	// Code that executes when an exception of type
	// networkIOException is thrown in the try block
	// ...
	// Log error message in the exception object
	cerr << e.what();
}
catch (const myDataFormatException& e) {
	// Code that handles another exception type
	// ...
	cerr << e.what();
}

// The following syntax shows a throw expression
MyData GetNetworkResource()
{
	// ...
	if (IOSuccess == false)
	throw networkIOException("Unable to connect");
	// ...
	if (readError)
	throw myDataFormatException("Format error");
	// ...
}
```

You can also use the throw keyword to output a reference number, like a custom
error number/code for organizing purposes:

```cpp
try {
	int age = 15;
	if (age > 18) {
		cout << "Access granted - you are old enough.";
	} else {
		throw 505;
	}
}
catch (int myNum) {
	cout << "Access denied - You must be at least 18 years old.\n";
	cout << "Error number: " << myNum;
}
```

## operator overloading

The basic syntax is:

```cpp
return_type operator (options)
```

You can also overload with types, e.g.:

```cpp
operator const char*()
```

## type casting

- [Cast
  comparison](https://newbedev.com/regular-cast-vs-static-cast-vs-dynamic-cast)
- [Understanding C++
  casts](https://codeburst.io/understanding-c-casts-ef1f36e54240)
- [Type Casting](https://www.cplusplus.com/doc/oldtutorial/typecasting/)
- [C++ Type Casting](https://www.w3adda.com/cplusplus-tutorial/cpp-type-casting)

```cpp
int a;
short b;

b = a; // C implicit casting
b = (short) a; // C explicit casting
```

The functionality of these explicit conversion operators is enough for most
needs with fundamental data types. However, these operators can be applied
indiscriminately on classes and pointers to classes, which can lead to code that
while being syntactically correct can cause runtime errors.

### const_cast

`const_cast<>()` is used to add/remove const(ness) (or volatile-ness) of a variable.

### static_cast

`static_cast<>()` is used to cast between the integer types. e.g.
`char`->`long`, `int`->`short` etc. Static cast is also used to cast pointers to
related types, for example casting `void*` to the appropriate type.

### dynamic_cast

Used to convert pointers and references at run-time, generally for the purpose
of casting a pointer or reference up or down an inheritance chain (inheritance
hierarchy).

The target type must be a pointer or reference type, and the expression must
evaluate to a pointer or reference. Dynamic cast works only when the type of
object to which the expression refers is compatible with the target type and
the base class has at least one virtual member function. If not, and the type of
expression being cast is a pointer, `NULL` is returned, if a dynamic cast on a
reference fails, a `bad_cast` exception is thrown. When it doesn't fail, dynamic
cast returns a pointer or reference of the target type to the object to which
expression referred.

### reinterpret_cast

Reinterpret cast simply casts one type bitwise to another. Any pointer or
integral type can be casted to any other with reinterpret cast, easily allowing
for misuse. For instance, with reinterpret cast one might, unsafely, cast an
integer pointer to a string pointer.

## explicit keyword

```cpp
class Complex
{
private:
    double real;
    double imag;

public:
    // Default constructor
    explicit Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // A method to compare two Complex numbers
    bool operator== (Complex rhs) {
       return (real == rhs.real && imag == rhs.imag)? true : false;
    }
};

int main()
{
    // a Complex object
    Complex com1(3.0, 0.0);

    if (com1 == 3.0)
       cout << "Same";
    else
       cout << "Not Same";
     return 0;
}
```

Output:

- with `explicit`: Compiler error
- without `explicit`: Same

Reason:

If there exists a constructor that can be called with only one argument, then it
acts as a _conversion constructor_. `explicit` tell the compiler not to consider
it as such.

## POCO library

The POrtable COmponents (POCO) C++ Libraries are computer software, a set of
class libraries for developing computer network-centric, portable applications in
the programming language C++. The libraries cover functions such as threads,
thread synchronizing, file system access, streams, shared libraries and class
loading, Internet sockets, and network communications protocols (HTTP, FTP, SMTP,
etc.), and include an HTTP server, and an XML parser with SAX2 and DOM interfaces
and SQL database access. The modular and efficient design and implementation
makes the libraries well suited for embedded system development.

## low level type definitions

You have to add the `<cstdint>` library, which corresponds to `stdint.h` in C.

## execv without warnings

```c++
char const* args[] = {
    "/usr/bin/ls",
    "-l",
    NULL
};
execv(args[0], const_cast<char**>(args));
```

## clang tools

- clang-format: [configurator](https://zed0.co.uk/clang-format-configurator/)

## typedef method pointer

[https://newbedev.com/calling-c-class-methods-via-a-function-pointer](Calling methods via a function
pointer)

Although you can use a `Dog*` in the place of an `Animal*` thanks to polymorphism, the type of a
function pointer does not follow the lookup rules of class hierarchy. So an `Animal` method pointer
is not compatible with a `Dog` method pointer, in other words you can't assign a `Dog* (*)()` to a
variable of type `Animal* (*)()`.

```c++
struct kkk {
	typedef int(kkk::*Bar)(const char*) const;
};

struct Foo : public kkk {
	typedef int(Foo::*Bar)(const char*) const;

	int print(const char* str) const
	{
		std::cout << str << std::endl;
		return 0;
	}
};

int main(int argc, char* argv[])
{
	Foo foo;
	Foo::Bar bar = &Foo::print;
	(foo.*bar)("test1");

	kkk::Bar heh = reinterpret_cast<kkk::Bar>(&Foo::print);
	(foo.*heh)("test2");

	return 0;
}
```

## Variadic template

- [Variadic
  template](http://www.vishalchovatiya.com/variadic-template-cpp-implementing-unsophisticated-tuple/)

Variadic template means to add as many templates as one wishes. This may be used to implement a
crude tuple:

```c++
template<
            typename T,
            typename... Rest    // Template parameter pack
        >
struct Tuple<T, Rest...> {      // Class parameter pack
    T first;
    Tuple<Rest...> rest;        // Parameter pack expansion
    Tuple(const T& f, const Rest& ... r)
        : first(f)
        , rest(r...) {
    }
};
```

## lambda functions

```
[ capture clause ] (parameters) -> return-type
{
   definition of method
}
```

Capture clause:

- `[&]`: by reference
- `[=]`: by value
- `[a,&b]`: a by value, b by reference
- `[]`: access only those variable which are local to it

```c++
vector<int> v1 = {3, 1, 7, 9};
vector<int> v2 = {10, 2, 7, 16, 9};

auto pushinto = [&] (int m) // -> void (this is optional)
{
    v1.push_back(m);
    v2.push_back(m);
};

pushinto(20);

auto square = [](int i)
{
    return i * i;
};

cout << "Square of 5 is : " << square(5) << endl;
```

Standard functions:

- `for_each`
- `find_if`
- `sort`
- `count_if`
- `unique`
- `accumulate`

Notes:
- According to Paragraph 5.1.2/3 of the C++11 Standard, "type of the lambda-expression is a unique,
  unnamed nonunion class type — called the closure type." This means, you should better stick to
  `auto`.
- Function return type is not the same as function pointer type.
