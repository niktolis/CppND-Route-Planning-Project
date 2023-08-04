# Route Planning Project

This repo contains the **implemented** code for the Route Planning project.

![img][fixedmap]

## Cloning

When cloning this project, be sure to use the `--recurse-submodules` flag. Using HTTPS:

```bash
git clone https://github.com/niktolis/CppND-Route-Planning-Project.git --recurse-submodules
```

or with SSH:

```bash
git clone git@github.com:niktolis/CppND-Route-Planning-Project.git --recurse-submodules
```

## Dependencies for Running Locally

* cmake >= 3.11.3
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 7.4.0
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same instructions as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* IO2D
  * Installation instructions for all operating systems can be found [here](https://github.com/cpp-io2d/P0267_RefImpl/blob/master/BUILDING.md)
  * This library must be built in a place where CMake `find_package` will be able to find it
    * **NOTE** For MacOS users with MacOS SDK11.0+: The IO2D is not working out of the box with CoreGraphics backend. The reason is the difference that GraphicContext objects are treated from NSViews. It requires XQuartzCore library and a slight modification in the drawing procedure. You can refer to the required changes on my forked I02D repo [Modified IO2D Library for MacOS 11+][IO2DForkedRepo]

## Compiling and Running

### IO2D Prerequisites if macOS CoreeGraphics engine is used

Follow the steps in [building instructions][macOSCGIO2DBuild] to install the dependencies at the first **4** steps. If you have latest macOS(11.0+) the update of the libc++ can be skipped. The Xcode command line tools already include a newer version from the link. If you have a silicon Mac then stick to the one provided already by the toolchain.

Clone the repo [Modified IO2D Library for MacOS 11+][IO2DForkedRepo] inside the `thirdpary` directory

```bash
cd thirdparty
git clone --recurse-submodules https://github.com/niktolis/P0267_RefImpl.git
cd P0267_RefImpl
mkdir Debug
cd Debug
cmake -G "Xcode" -DIO2D_DEFAULT=COREGRAPHICS_MAC ../.
open io2d.xcodeproj
```

The CMake configuration on the forked repo has been modified and the library is created in the `./share` directory. Without choosing a specific folder the default one is `/usr/local/` where it requires root privileges and the `install` target fails by default. The best option instead of changing ownership of the `/usr/local` is to define a different path where Xcode can execute the installation.

You can use the target `install` and the library will be built and deployed in the shared folder.

As a next step in the root project `CMakeLists.txt` it is essential to add the following line before the project name definition in order during the configuration and compilation executed at the [Compiling](#compiling) step:

```cmake
# Add prefix path for the library derived by I02D
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/P0267_RefImpl/share" CACHE PATH "Prefix path")
```

### IO2D Prerequisites if X11 and CAIRO is used in macOS (without MacPorts)

X11 and CAIRO option for macOS can be used as backend installation for IO2D.

Follow the steps in [building instructions][macOSX11IO2DBuild] to install the dependencies at the first **5** steps. If you have latest macOS(11.0+) the update of the libc++ can be skipped. The Xcode command line tools already include a newer version from the link. If you have a silicon Mac then stick to the one provided already by the toolchain.

Download and install [XQuartz][xquartz.org]

install `Cairo` from brew:

```bash
brew install cairo
```

```bash
cd thirdparty
git clone --recurse-submodules https://github.com/niktolis/P0267_RefImpl.git
cd P0267_RefImpl
mkdir Debug
cd Debug
cmake -G "Xcode" ../.
open io2d.xcodeproj
```

In the `CMakeLists.txt` add the following lines on **both** the IO2D project and the root project folder before the definition of the project name:

```cmake
# Add necessary flags for IO2D lib using X11 config
SET(GXX_COVERAGE_COMPILE_FLAGS "-isystem/opt/homebrew/include -isystem/opt/local/include")
SET(GXX_COVERAGE_LINK_FLAGS    "-L/opt/homebrew/lib -L/opt/local/lib")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GXX_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GXX_COVERAGE_LINK_FLAGS}")
```

Additionaly add also the following lines in the root project `CMakeLists.txt`:

```cmake
# Add prefix path for the library derived by I02D
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/P0267_RefImpl/share" CACHE PATH "Prefix path")
```

### Compiling

To compile the project, first, create a `build` directory and change to that directory:

```bash
mkdir build && cd build
```

From within the `build` directory, then run `cmake` and `make` as follows:

```bash
cmake ..
make
```

### Running

The executable will be placed in the `build` directory. From within `build`, you can run the project as follows:

```bash
./OSM_A_star_search
```

Or to specify a map file:

```bash
./OSM_A_star_search -f ../<your_osm_file.osm>
```

## Testing

The testing executable is also placed in the `build` directory. From within `build`, you can run the unit tests as follows:

```bash
./test
```

## Retrospective

Generally the exercise was straightforward and fairly easy following all the steps in the TODO comments and combining it with the previous Lesson for the implementation of the A* Algorithm for finding the shortest path.

The code implemented is running with user input checking that the coordinates given are in the specified range (otherwise returns with an error).

For the given map and the given coordinates start(10,10) and end(90,90) the produced result is can be seen on the following screenshots:

![img][testrunwithCG]

The tests are also have been executed and passed:

![img][testResult]

## Troubleshooting

* Some students have reported issues in cmake to find io2d packages, make sure you have downloaded [this](https://github.com/cpp-io2d/P0267_RefImpl/blob/master/BUILDING.md#xcode-and-libc).
* For MAC Users cmake issues: Comment these lines from CMakeLists.txt under P0267_RefImpl

    ```cmake
    if( NOT DEFINED IO2D_WITHOUT_SAMPLES )
        add_subdirectory(P0267_RefImpl/Samples)
    endif()
    ```

    And then run "ALL_Build" and "install" in XCode.

    If any packages are missing try to install packages using

    ```bash
    brew install pkg-config
    ```

* For Ubuntu Linux IO2D installation errors, follow the given steps:

   ```bash
  sudo apt update
  sudo apt install build-essential
  sudo apt install cmake
  sudo apt install libcairo2-dev
  sudo apt install libgraphicsmagick1-dev
  sudo apt install libpng-dev

  git clone --recurse-submodules https://github.com/cpp-io2d/P0267_RefImpl
  cd P0267_RefImpl
  mkdir Debug
  cd Debug
  cmake --config Debug "-DCMAKE_BUILD_TYPE=Debug" ..
  cmake --build .
  sudo make install
   ```

* If you are working on windows and unable to install IO2D:
  * Enable WSL (Windows Subsystem for Linux) and use a distribution like [Ubuntu](https://ubuntu.com/wsl).(available from the windows store):
  * Install the required dependencies (compiler, cmake etc.) in the WSL(as mentioned above for ubuntu)
  * Configure CLion to use the WSL [toolchain](https://www.jetbrains.com/help/clion/how-to-use-wsl-development-environment-in-product.html#wsl-tooclhain)
  * Use the WSL toolchain to build the project
  * If you are still facing errors, visit this [link](https://github.com/udacity/CppND-Route-Planning-Project/issues/9).

* If you are facing errors with --config try to remove -- from the command.

[fixedmap]: screenshots/mapfixed.png
[macOSCGIO2DBuild]: https://github.com/niktolis/P0267_RefImpl/blob/master/BUILDING.md#coregraphicsmac-on-macos
[IO2DForkedRepo]: https://github.com/niktolis/P0267_RefImpl.git
[macOSX11IO2DBuild]: https://github.com/niktolis/P0267_RefImpl/blob/master/BUILDING.md#cairoxlib-on-macos
[xquartz.org]: https://www.xquartz.org
[testResult]: screenshots/testresult.png
[testrunwithCG]: screenshots/runwithCG.png
