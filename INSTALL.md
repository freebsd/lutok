# Introduction

Lutok uses CMake as its build system. These instructions are for compiling
the library from source. If you want to install Lutok from a binary package,
you do not need to read this document.

For the impatient:

```shell
$ cmake -B build -S .
$ cmake --build build
$ ctest --test-dir build  # Run tests
$ sudo cmake --install build
```

Or alternatively, install as a regular user into your home directory:

```shell
$ cmake -B build -S . -DCMAKE_INSTALL_PREFIX=~/local
$ cmake --build build
$ ctest --test-dir build
$ cmake --install build
```

# Dependencies

To build and use Lutok successfully you need:

* CMake 4.2 or greater.
* A C++20 standards-compliant compiler.
* Lua 5.3 or greater.

Optionally, if you want to build and run the tests (recommended), you need:

* Kyua 0.5 or greater.
* ATF 0.15 or greater.

Optionally, if you want to build the API documentation, you need:

* Doxygen

# General build procedure

To build and install the source package, follow these steps:

1. **Configure** the build using CMake. This is done by running `cmake -B build -S .`
   from the source directory. You can customize the build with various options
   (see Configuration Options below).

2. **Build** the sources by running `cmake --build build`.

3. **Test** the build (optional but recommended) by running `ctest --test-dir build`
   or `cmake --build build --target test`.

4. **Install** the library by running `cmake --install build`. You may need to use
   `sudo cmake --install build` if installing to system directories.

# Configuration Options

Common CMake options:

- `-DCMAKE_INSTALL_PREFIX=<directory>`
    - **Default**: "/usr/local" (on FreeBSD and macOS)

      Specifies where the library will be installed.

- `-DCMAKE_BUILD_TYPE=<type>`
    - **Possible values**: "Debug", "Release", "RelWithDebInfo", "MinSizeRel"
    - **Default**: None (uses default compiler flags)

      Specifies the build type. Use "Debug" for development, "Release" for
      optimized builds.

Lutok-specific options:

- `-DDEVELOPER_MODE=<ON|OFF>`
    - **Default**: ON when building from git/jj repository, OFF otherwise

      Enables strict compiler warnings and treats warnings as errors. Useful
      for development. Auto-enabled when building from a VCS checkout.

- `-DBUILD_TESTING=<ON|OFF>`
    - **Default**: ON

      Enables building of test programs. Tests require ATF to be installed.
      Run tests with `ctest --test-dir build` or `cmake --build build --target test`.

- `-DBUILD_DOCS=<ON|OFF>`
    - **Default**: OFF

      Enables building of API documentation with Doxygen. Build docs with
      `cmake --build build --target docs`. Generated docs will be in
      `build/api-docs/html/`.

- `-DBUILD_EXAMPLES=<ON|OFF>`
    - **Default**: OFF

      Enables building of example programs.

# Examples

Build with tests enabled:

```shell
$ cmake -B build -S . -DBUILD_TESTING=ON
$ cmake --build build
$ ctest --test-dir build
```

Build with documentation:

```shell
$ cmake -B build -S . -DBUILD_DOCS=ON
$ cmake --build build
$ cmake --build build --target docs
$ open build/api-docs/html/index.html  # View docs
```

Build in debug mode with examples:

```shell
$ cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DBUILD_EXAMPLES=ON
$ cmake --build build
```

# Testing

After a successful installation (assuming you built with `-DBUILD_TESTING=ON`),
you can run the installed tests:

```shell
$ kyua test -k /usr/local/tests/lutok/Kyuafile
```

If you see any tests fail, please report them at:

    https://github.com/freebsd/lutok/issues/

Thank you!
