# How to install the latest `cmake`

Visit [github:Kitware/CMake](https://github.com/Kitware/CMake/releases) to check the latest release:

```bash
$ cd ~/work
$ wget -O cmake.sh https://github.com/Kitware/CMake/releases/download/v3.23.4/cmake-3.23.4-linux-aarch64.sh

$ cd /usr/local
$ sudo bash ~/work/cmake.sh
$ sudo mv cmake-3.23.4-linux-aarch64 cmake-3.23.4
```

Edit `~/.profile` to add the path to `cmake`:

```bash
# set PATH to cmake
PATH="/usr/local/cmake-3.23.4/bin:$PATH"
```

Log out or run `'. .profile'`, then make sure the location and version of `cmake`:

```bash
$ which cmake
/usr/local/cmake-3.23.4/bin/cmake

$ cmake --version
cmake version 3.23.4
```

You may now remove `~/work/cmake.sh`.
