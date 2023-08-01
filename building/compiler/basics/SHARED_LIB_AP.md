# Dynamic Libraries on Mac OS X

When an app is linked with a library using a static linker, the code that the app uses is copied to the generated executable file. A *static linker* collects compiled source code, known as object code, and library code into one executable file that is loaded into memory in its entirety at runtime. The kind of library that becomes part of an app’s executable file is known as a *static library*.

> Static libraries are also known as static archive libraries and *static linked shared libraries*.

[Dynamic libraries](https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/DynamicLibraries/100-Articles/OverviewOfDynamicLibraries.html) are not statically linked into client apps; they don't become part of the executable file. Instead, dynamic libraries can be loaded (and linked) into an app either when the app is launched or as it runs.

> Dynamic libraries are also known as *dynamic shared libraries*, shared objects, or *dynamically linked libraries*.

When an app is launched, the OS X kernel loads the app’s code and data into the address space of a new process. The kernel also loads the dynamic loader (`/usr/lib/dyld`) into the process and passes control to it. The dynamic loader then loads the app’s dependent libraries. These are the dynamic libraries the app was linked with.

The static linker records the filenames of each of the dependent libraries at the time the app is linked. This filename is known as the dynamic library’s *install name*. The dynamic loader uses the app’s dependent libraries’ install names to locate them in the file system. If the dynamic loader doesn’t find all the app’s dependent libraries at launch time or if any of the libraries is not compatible with the app, the launch process is aborted.

```bash
(conan2) build % otool -L compressor
compressor:
	/usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 1500.65.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1319.100.3)
```
