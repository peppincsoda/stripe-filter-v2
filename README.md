
# stripe-filter-v2

Table of contents
=================

  * [Introduction](#introduction)
  * [Building on Linux](#building-on-linux)
    * [Installing as a service](#installing-as-a-service)
  * [Building on Windows](#building-on-windows)
  * [Usage](#usage)

## Introduction

This application measures the thickness of an object in a camera image using simple image processing algorithms.
The measurement results can then be sent to a PLC over the serial line using the Modbus RTU protocol.

The specific scenario for which this application was designed is the following:
An extrusion machine that applies plastic coating to metal pipes gives varying coating thickness along the pipe and thus
needs regular checking and tuning manually. We are working to automate this: A PLC can regulate the machine according to
thickness measurements from a sensor. This sensor is a Raspberry Pi connected to a Basler industrial camera and running
this application.

You can also run the application reading from different kinds of inputs and writing to different
kinds of outputs. For example, you can use a USB webcam and write the measurement to the console to play with
the filter parameters.

The project is cross-platform, next to Raspbian/GCC it was tested on MinGW32 and you should be able to build it on other
Linux platforms as well.
On Windows, there is no Pylon support for MinGW32, that is the reason `BaslerLib.dll` (the library encapsulating all
Basler Pylon SDK dependencies) has to be built with MSVC and the main application loads the DLL at runtime.

These are the processing steps incorporated into the filter:
+ Converting the input image to grayscale (if needed)
+ Median filtering
+ Gaussian filtering
+ Box filtering
+ Binary thresholding
+ Searching the borders of the object inside the ROI

You can turn on/off all filters and the binary thresholding in this pipeline when configuring and tuning.

### Why v2?

The original `stripe-filter` was an experimental project written in plain C and using GLib.
Every algorithm it contains was written from scratch (and some copy-pasted).
While it was useful for learning about GLib and image processing algorithms, there are much better options
to use for a real product.

That is why `stripe-filter-v2` was born: Using modern C++ and well tested libraries like Qt and OpenCV
resulted in an application that is easier to understand and modify. (Also the time needed for development
was much shorter.)
It also has new features:
+ UI to change all parameters on all platforms
+ Support for USB webcams

## Building on Linux

The following steps are for Raspbian Wheezy.

Install all dependencies: CMake, Qt5, OpenCV

```bash
~ $ sudo apt-get install cmake qt5-default libqt5serialport5-dev libopencv-dev
```

Install Basler Pylon SDK to `/opt`:

```bash
~ $ wget https://www.baslerweb.com/fp-1496746643/media/downloads/software/pylon_software/pylon-5.0.9.10389-armhf.tar.gz
~ $ tar xvvf pylon-5.0.9.10389-armhf.tar.gz
~ $ cd pylon-5.0.9.10389-armhf
~/pylon-5.0.9.10389-armhf $ sudo tar -C /opt -xzf pylonSDK-5.0.9.10389-armhf.tar.gz
```

Clone this repository:

```bash
~/pylon-5.0.9.10389-armhf $ cd
~ $ git clone https://github.com/peppincsoda/stripe-filter-v2
```

And build it with CMake:

```bash
~ $ mkdir build; cd build
~/build $ export PYLON_ROOT=/opt/pylon5
~/build $ cmake -DCMAKE_BUILD_TYPE=Debug ../stripe-filter-v2
~/build $ make
```

Run the application:

```bash
~/build $ bin/filterapp --gui
```

You can press Ctrl+C in the terminal or close the main window to exit.

If you do not have a Basler camera, you still can test the application by setting the `PYLON_CAMEMU` environment variable:

```bash
~/build $ export PYLON_CAMEMU=1
~/build $ bin/filterapp --gui
```

### Installing as a service

You can have the application started automatically every time the system is booted up:

```bash
~ $ sudo cp /home/pi/stripe-filter-v2/filterapp/filterapp.service /etc/systemd/system/filterapp.service
~ $ sudo chmod 644 /etc/systemd/system/filterapp.service
~ $ sudo systemctl daemon-reload
~ $ sudo systemctl enable filterapp.service
~ $ sudo systemctl start filterapp.service
```

Checking the health of the service:

```bash
~ $ systemctl status filterapp.service
```

And to view its log:

```bash
~ $ journalctl -u filterapp.service -f
```

Restarting if needed:

```bash
~ $ sudo systemctl restart filterapp.service
```

## Building on Windows

* Install Qt5 with MinGW32 support.
* Install CMake for Windows.
* Build OpenCV using the MinGW32 toolchain:
  * Download and extract `https://github.com/opencv/opencv/archive/2.4.13.4.zip` to `c:\work\src\opencv`.
  * Open a console and build the project using CMake:
  ```bat
  set PATH=c:\Qt\Tools\mingw530_32\bin;%PATH%
  mkdir c:\work\src\opencv_build; cd /d c:\work\src\opencv_build
  "c:/Program Files/CMake/bin/cmake.exe" -G "MinGW Makefiles" -DCMAKE_CONFIGURATION_TYPES=Debug;Release -DCMAKE_CXX_COMPILER="c:/Qt/Tools/mingw530_32/bin/g++.exe" -DCMAKE_C_COMPILER="c:/Qt/Tools/mingw530_32/bin/gcc.exe" -DCMAKE_Fortran_COMPILER="c:/Qt/Tools/mingw530_32/bin/gfortran.exe" -DCMAKE_INSTALL_PREFIX="c:/work/src/opencv_install" -DCMAKE_MAKE_PROGRAM="c:/Qt/Tools/mingw530_32/bin/mingw32-make.exe" ../opencv
  mingw32-make install
  ```
* To make Qt Creator to find OpenCV go to `Tools Menu/Options/Build & Run/Kits/<select kit>/CMake Configuration` and add
  the install path from above to `CMAKE_PREFIX_PATH` like this:
  ```
  CMAKE_PREFIX_PATH:STRING=%{Qt:QT_INSTALL_PREFIX};c:\work\src\opencv_install
  ```
* Clone this project from GitHub:
  ```bat
  git clone https://github.com/peppincsoda/stripe-filter-v2
  ```
* Now you can open the `CMakeLists.txt` file in Qt Creator and build the project.

If you also need Basler camera support you have to build `BaslerLib` in the `basler\vs_sln` subdirectory using VS2012:
* Download and install the Basler Pylon SDK for Windows.
* Build the solution in VS.

## Usage

Coming soon...

