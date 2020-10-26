

WebKrunchers' InformationKruncher
An http web server that is designed to be simple, standard, and easy to use.

Intended Audience:
Software developers who wish to use C++ to build a simple web server.

Introduction:
This server is pure C++.  There are many features that modern
web servers offer, but may never be part of the basic InformationKruncher
server.  This keeps the project free of any pollution, and give the user
the leverage to add any features desired.

This is designed to be a starting point, not a complete service.  The bare
bones service does not provide any database interactions, or access to 
persisent data of any kind.  All transasctions are stateless, and serve
static data.

This project is probably very useful for anyone who needs to create a
simple service without any COTs packages.  There are many libraries that
can be used.  This project offers everything needed to create a basic service,
and a few tools to deal with hypertext.  But, the entire project is only
a about 2K lines of source code, and the main source file is very small,
and not difficult to follow.

The configuration is hard coded.  This is done to keep the source code as
simple as possible, and to make it easier to integrate into existing solutions.



