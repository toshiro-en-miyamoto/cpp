# Building for multiple configurations: Static and Shared

We can change from static to shared linking by setting the `shared` option to `True` using the `--options` argument.

```bash
shared $ ~/source venv/conan2/bin/activate 
(conan2) shared $ conan install . \
  --output-folder=build --build=missing \
  --options=zlib/1.2.13:shared=True
```

The profile with the option works as well:

```bash
(conan2) shared $ cat conanfile.txt
[requires]
zlib/1.2.13

[generators]
CMakeDeps
CMakeToolchain

[options]
zlib/1.2.13:shared=True

(conan2) shared $ conan install . \
  --output-folder=build --build=missing
```

Doing this, Conan will install the Zlib shared libraries, generate the files to build with them and, also the necessary files to locate those dynamic libraries when running the application. Let’s build the application again after configuring it to link Zlib as a shared library:

```bash
(conan2) shared $ cd build
(conan2) build $ cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release
(conan2) build $ cmake --build .
```

Now, if you try to run the compiled executable you will see an error because the executable can’t find the shared libraries for Zlib that we just installed.

```bash
(conan2) shared $ deactivate

shared $ ./compressor
./compressor: error while loading shared libraries: libc++.so.1: cannot open shared object file: No such file or directory
```

This is because shared libraries are loaded at runtime. That means that the application executable needs to know where are the required shared libraries when it runs.

Conan provides a mechanism to define those variables and make it possible, for executables, to find and load these shared libraries. This mechanism is the `VirtualRunEnv` generator. If you check the output folder you will see that Conan generated a new file called `conanrun.sh`. This is the result of automatically invoking that `VirtualRunEnv` generator when we activated the shared option when doing the `conan install`. This generated script will set the `DYLD_LIBRARY_PATH` environment variables so that executables can find the shared libraries.

```bash
build $ source conanrun.sh

build $ env | grep LD_
DYLD_LIBRARY_PATH=/home/.../.conan2/p/b/zlib.../p/lib:
LD_LIBRARY_PATH=/usr/local/clang-16.0.0/lib/aarch64-linux-gnu

build $ ./compressor
Uncompressed size is: 233
Compressed size is: 147
ZLIB VERSION: 1.2.13
```

You can examine if the binary depends on the `zlib` library generated in the Conan build tree:

```bash
build $ ldd compressor
	linux-vdso.so.1 (0x...)
	libz.so.1 => /home/.../.conan2/p/b/zlib.../p/lib/libz.so.1 (0x...)
	libc++.so.1 => /usr/local/clang-16.0.0/lib/aarch64-linux-gnu/libc++.so.1 (0x...)
	libc++abi.so.1 => /usr/local/clang-16.0.0/lib/aarch64-linux-gnu/libc++abi.so.1 (0x...)
	libm.so.6 => /lib/aarch64-linux-gnu/libm.so.6 (0x...)
	libgcc_s.so.1 => /lib/aarch64-linux-gnu/libgcc_s.so.1 (0x...)
	libc.so.6 => /lib/aarch64-linux-gnu/libc.so.6 (0x...)
	libpthread.so.0 => /lib/aarch64-linux-gnu/libpthread.so.0 (0x...)
	librt.so.1 => /lib/aarch64-linux-gnu/librt.so.1 (0x...)
	libatomic.so.1 => /lib/aarch64-linux-gnu/libatomic.so.1 (0x...)
	/lib/ld-linux-aarch64.so.1 (0x...)

build $ source deactivate_conanrun.sh
Restoring environment

build $ env | grep LD_
build $

build $ ldd compressor
	linux-vdso.so.1 (0x...)
	libz.so.1 => /home/.../.conan2/p/b/zlib.../p/lib/libz.so.1 (0x...)
	libc++.so.1 => not found
	libc++abi.so.1 => not found
	libm.so.6 => /lib/aarch64-linux-gnu/libm.so.6 (0x...)
	libgcc_s.so.1 => /lib/aarch64-linux-gnu/libgcc_s.so.1 (0x...)
	libc.so.6 => /lib/aarch64-linux-gnu/libc.so.6 (0x...)
	/lib/ld-linux-aarch64.so.1 (0x...)
```
