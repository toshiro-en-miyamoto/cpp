# Building for multiple configurations: Static and Shared

We can change from static to shared linking by setting the `shared` option to `True` using the `--options` argument.

```bash
$ conan install . --output-folder=build --build=missing --options=zlib/1.2.13:shared=True
$ cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
$ ./compressor
Uncompressed size is: 233
Compressed size is: 147
ZLIB VERSION: 1.2.13
Release configuration!
```

The application executable needs to know where are the required shared libraries when it runs. On OSX, it will search in the directories declared in `DYLD_LIBRARY_PATH` as on Linux will use the `LD_LIBRARY_PATH`.
