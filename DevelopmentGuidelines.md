# Introduction #

This page will detail the development guidelines and the coding rules applied to this project. Since the project is Ogre based, we will try to adopt the same coding rules, with some modification. Following detailes are taken from:

[OGRE Coding Standards](http://ogre.svn.sourceforge.net/viewvc/ogre/trunk/Docs/CodingStandards.html)


# Details #

## Wyverns' Assault Coding Standards ##

This document describes the coding standards all developers are expected to adhere to when writing code for the Wyverns' Assault project.

### Top-level organisation issues ###

All source files must begin with the standard copyright statement:
```
/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
```

All publically visible classes should be declared in their own header file using the .h extension, placed in the 'include' folder of the sub-project in question, and named after the class e.g. 'MyClass.h'. Only very tightly related classes should be declared in the same header file.
Implementations should be placed in a source file called the same name as the class but with an extension of .cpp.
Everything must be declared inside the namespace 'WyvernsAssault'.
Portablity

All code must be cross platform, ANSI C++. No dependencies on platform-specific headers and / or types are allowed (the only exception is when dealing with platform-specific features like windowing, which must be implemented for each platform separately).
If you serialise / deserialise any data, subclass from Serializer and use its methods, it will insulate you from endianness issues. If you need to serialise any types it doesn't already handle, make sure you deal with endianness issues in the same way Serializer does (ie use native endianness by default but detect the inverse on reading and perform flipping if required).

### C++ Standards compliance ###

  * Always prefer the STL over custom containers / algorithms.
  * Always prefer C++ techniques over C.
  * Avoid C-strings (char`*` and functions like sprintf, strcpy, use Ogre::String)
  * Avoid old I/O routines (fopen et al, use `<`iostream`>`)
  * Use abstract classes or templates not void`*`
  * Use overloaded methods not varargs.
  * Minimum C++ compiler level is MSVC 7.1 or gcc 3.1. Compilers which do not support things like partial template specialisation properly (such as older versions of MSVC) are not supported.
  * Use the PImpl idiom to reduce dependencies between classes.
  * Always use const-correctness. Methods taking non-primitive types as parameters should generally take them as const references, methods returning non-primitive types should generally return them as const references. Declare all methods that do not modify internal state 'const'. For lazy-update getter methods, declare the internal state which is lazy-updated 'mutable'.
  * Prefer 'protected' over 'private' to encourage specialisation where appropriate
  * Always declare destructors 'virtual' unless the class you are writing should not have any vtable (no other virtual methods).
  * Avoid non-const by-ref parameters unless you have no other option. We prefer not to have in/our parameters since they are less intuitive.

### Naming conventions & Documentation ###

  * Classes, types and structures must be title case (MyNewClass).
  * Methods and local variables must be camel case (myNewMethod).
  * Member variables should be prefixed with 'm' (mInstanceVar), static member variables should be prefixed 'ms' (msStaticMemberVar). Do not use any other prefixing such as Hungarian notation.
  * Preprocessor macros must be all upper case and prefixed with **WYVERNS`_`ASSAULT`_`**
  * Enums should be named in title case, enum values should be all upper case
  * All classes and methods must be fully documented in English using Doxygen-compatible comments. Use the @param and @returns directives to define inputs and  outputs clearly, and @note to indicate points of interest.
  * Use verbose, descriptive names for classes, methods, variables - everything except trival counters. Code should be self-describing, don't be obtuse.

### Memory Management ###

  * For Ogre realted code, follow Ogre Coding Standards, as stated in the top of the page
  * Be aware of allocator issues when using SharedPtr
  * When defining STL containers, instead of using std::vector or std::list etc, use the memory-manager enabled versions vector::type and list::type respectively (most containers have this equivalent). This defaults the memory manager to the General type, but you can override the last parameter to the template if you want an alternate type.

### Style issues ###

  * Insert a newline before an open brace (contentious I know!)
  * Use typedefs to declare template-based types that you use to avoid ugliness e.g. typedef std::list<MyType`*`> MyTypeList;
  * Always insert spaces in between operators and operands (x + y, not x+y)
  * Use parenthesis to make the operator precedence unambiguous, even when it is not required ((x `*` y) + 1, not x `*` y + 1)

### Error handling ###

  * Fatal errors should always be dealt with though exception handling. No return-value error reporting.
  * Whenever you make an assumption in your code, verify it with an assert().

### Design issues ###

  * Use existing design patterns and identify them by their well known names. A good starting reference is the 'Gang of Four' book.
  * Use strong encapsulation. Top-level interfaces should hide implementations and not require the user of the library to understand internals.
  * Avoid public attributes except in structs.
  * Don't use 'friend' if you can avoid it. Where classes need to collaborate on an internal implementation, prefix the methods they use to communicate with '`_`' (this is our demarcation for 'recommended for internal use only'). This can also be used to expose advanced functionality only intended for very skilled users.

### Final words ###

If in doubt, do as the existing code does!