# Non Static Data Members Initialization in C++

My short summary for non static data members initialization from modern C++. A very useful feature. Should we use it or not?

## Intro

Non-static data member initializers	| Paper [N2756](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2756.htm)
---------------------------------- |----------
Visual Studio |Since VS 2013
GCC |	Since GCC 4.7
Intel Compiler | Since version	14.0
Clang | Since Clang 3.0

Previously you could only initialize static, integral and const members of a class. Now it is extended to support non static members that do not need to be const and may have any type.

## Basic example

```cpp
class SimpleType
{
private:
	int a { 1 };    // << wow!
	int b { 1 };    // << wow2!
	string name { "string" }; // wow3!

public:
	SimpleType() {
		cout << "SimpleType::ctor, {" 
				  << a << ", " 
				  << b << ", \"" 
				  << name << "\"}" 
				  << endl;
	}
	~SimpleType() { 
		cout << "SimpleType::destructor" << endl; 
	}
};
```

If we create an object of type `SimpleType`:

    SimpleType obj;

On the output we will get:

    SimpleType::ctor, {1, 1, "string"}

All of member variables were properly initialized **before** our constructor was called. Note, that we did not initialize members in the constructor. Such approach is not only available for simple types like `int`, but also for a complicated type like `std::string`.

## Why useful

* Easier to write
* You are sure that each member is properly initialized.
 * you cannot forget to initialize a member like when having a complicated constructor. Initialization and declaration are in one place - not separated.
* Especially useful when we have several constructors.
 *  Previously we would have to duplicate initialization code for members.
 * Now, you can do a default initialization and constructors will only do its specific jobs...

You can play with the exmple here 

@[Simple Object]({"stubs": ["cpp_nsdmi_simple.cpp"],"command": "sh ./run.sh cpp_nsdmi_simple.cpp"})

## More details

Let's now make some more advanced example:

`SimpleType` with a new constructor

```cpp
class SimpleType
{
private:
    int a { 1 };    // << wow!
    int b { 1 };    // << wow2!
    string name { "string" }; // wow3!

public:
    SimpleType() { /* old code... */ }
    SimpleType(int aa, int bb) 
        : a(aa), b(bb) // << custom init!
    {
    std::cout << "SimpleType::ctor(aa, bb), {"  
        << a << ", " 
        << b << ", \"" 
        << name << "\"}" 
        << std::endl;
    }
    ~SimpleType() { 
        cout << "SimpleType::destructor" << endl; 
    }
};
```

And `AdvancedType`:

```cpp
class AdvancedType
{
private:
	SimpleType simple;
	
public:
	AdvancedType() {
		cout << "AdvancedType::ctor" << endl;
	}
	AdvancedType(int a) : simple(a, a) {
		cout << "AdvancedType::ctor(a)" << endl;
	}
	~AdvancedType() { 
		cout << "AdvancedType::destructor" << endl; 
	}
};
```

So now, `AdvancedType` uses `SimpleType` as a member. And we have two constructors here.

If we write:

    AdvancedType adv;

We will get:

    SimpleType::ctor, {1, 1, "string"}
    AdvancedType::ctor

`SimpleType::ctor` (default) was called before `AdvancedType::ctor`. Note that `AdvancedType::ctor` does nothing beside printing...

Then, if we write:

    AdvancedType advObj2(10);

We will get:

    SimpleType::ctor(aa, bb), {10, 10, "string"}
    AdvancedType::ctor(a)

So this time, the second constructor of `SimpleType` was called. 

**Note**: even if you have a default initialization for a member, you can easily **overwrite** it in a constructor. Only one initialization is performed.

As usual you can play with the code below:

@[Advanced Object]({"stubs": ["cpp_nsdmi_adv.cpp"],"command": "sh ./run.sh cpp_nsdmi_adv.cpp"})
	
## Any negative sides?

The feature that we discuss, although looks nice and easy, has some drawbacks as well.

* Performance: when you have performance critical data structures (for example a `Vector3D` class) you may want to have "empty" initialization code. You risk having uninitialized data members, but you will save several instructions.
* Making class non-aggregate: I was not aware of this issue, but Shafik Yaghmour noted that in the comments below the article.
 * In C++11 spec did not allowed aggregate types to have such initialization, but in C++14 this requirement was removed.
 * Link to [the StackOverflow question](http://stackoverflow.com/questions/27118535/c11-aggregate-initialization-for-classes-with-member-in-pace-initializers) with details

## Should you use it?
I do not think there are any drawbacks of using non static data members initialization. 

If your coding guideline contains a rule about initialization of every local variable in the code, then, in my opinion, non static data member initialization completes this approach.

BTW: If that puts any standard, this concept is  **not** forbidden in [Google C++ guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.html#C++11)

## Your turn

What do you think about Non static data member initialization? Do you use it in your code?

## Links
* [Bjarne Stroustrup C++ FAQ : In-class member initializers](http://www.stroustrup.com/C++11FAQ.html#member-init)
* [VC++ Blog: The Future of Non-Static Data Member Initialization](http://blogs.msdn.com/b/vcblog/archive/2014/08/19/the-future-of-non-static-data-member-initialization.aspx)	