
# stripe-filter-v2

Table of contents
=================

  * [Introduction](#introduction)
  * [Building on Linux](#building-on-linux)
    * [Updating manually](#updating-manually)
    * [Installing as a service](#installing-as-a-service)
    * [Start with X](#start-with-x)
  * [Building on Windows](#building-on-windows)
  * [Usage](#usage)
  * [Known bugs](#known-bugs)
  * [License](#license)

## Introduction

This application measures the thickness of an object in a camera image using simple image processing algorithms.
The measurement results can then be sent to a PLC over the serial line using the Modbus RTU protocol.

The specific scenario for which this application was designed is the following:
An extrusion machine that applies plastic coating to metal pipes gives varying coating thickness along the pipe and thus
needs regular manual checking and tuning. We want to automate this with a PLC that can regulate the machine according to
thickness measurements from an optical sensor. The sensor is a Raspberry Pi connected to a Basler industrial camera and running
this application.

You can also run the application reading from other kinds of inputs and writing to other
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

You can turn on/off any of the filters and the binary thresholding in this pipeline.

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
~/work $ wget https://www.baslerweb.com/fp-1496746643/media/downloads/software/pylon_software/pylon-5.0.9.10389-armhf.tar.gz
~/work $ tar xvvf pylon-5.0.9.10389-armhf.tar.gz
~/work $ cd pylon-5.0.9.10389-armhf
~/work/pylon-5.0.9.10389-armhf $ sudo tar -C /opt -xzf pylonSDK-5.0.9.10389-armhf.tar.gz
~/work/pylon-5.0.9.10389-armhf $ cd
```

Clone this repository:

```bash
~/work $ git clone https://github.com/peppincsoda/stripe-filter-v2
```

And build it with CMake:

```bash
~/work $ mkdir build; cd build
~/work/build $ export PYLON_ROOT=/opt/pylon5
~/work/build $ cmake -DCMAKE_BUILD_TYPE=Debug ../stripe-filter-v2
~/work/build $ make
```

Run the application:

```bash
~/work/build $ bin/filterapp --settings-file=~/work/settings.ini --gui
```

You can press Ctrl+C in the terminal or close the main window to exit.

If you do not have a Basler camera, you still can test the application by setting the `PYLON_CAMEMU` environment variable:

```bash
~/work/build $ export PYLON_CAMEMU=1
~/work/build $ bin/filterapp --settings-file=~/work/settings.ini --gui
```

### Updating manually

1. Stop the application or service.
1. Update the sources:
   ```
   ~ $ cd work/stripe-filter-v2
   ~/work/stripe-filter-v2 $ git pull
   ```
1. Build:
   ```
   ~/work/stripe-filter-v2 $ cd ../build
   ~/work/build $ make
   ```
1. Start the application or service.

### Installing as a service

You can have the application started automatically every time the system is booted up:

```bash
~ $ sudo cp /home/pi/work/stripe-filter-v2/filterapp/filterapp.service /etc/systemd/system/filterapp.service
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

### Start with X

To start the application automatically whenever the graphical desktop is launched you can create `~/.config/autostart/filterapp.desktop` with the following content:

```
[Desktop Entry]
Name=FilterApp
Exec=lxterminal -e /home/pi/work/build/bin/filterapp --settings-file=/home/pi/work/settings.ini --gui
Type=application
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

The application can get command line arguments: `--gui` to show the UI allowing to configure and fine tune the parameters
and `--settings-file` which receives the path to the file that will hold all parameter values.

### Settings on the UI:

+ *Filter*:
  + *Optimize ROI*: When turned on, the program will shrink the user-defined ROI to contain only pixels that
    actually affect the measurement to reduce the CPU load.
  + *Use Median*, *Kernel size*: Whether to apply median filtering and its kernel size.
  + *Use Gaussian*, *Kernel size*, *Sigma*: Whether to apply Gaussian filtering and its parameters.
  + *Use Box*, *Kernel size*: Whether to apply box filtering and its kernel size.
  + *Use Threshold*, *Threshold value*: Whether to apply binary thresholding and the value of the threshold.
  + *Black object*: When turned on, the search will look for the first black pixels from the left and right
    end of the centerline in the ROI, otherwise it will look for the first white pixels.
+ *Entropy*:
  + *Entropy samples*: How many values to compute the average over.
  + *Scale factor*: The average is multiplied with this factor before discretizing into a 16-bit value.
+ *Serial Port*: The usual serial port settings.
  + *Port name*: `COM1`, `COM2`, etc. on Windows and `ttyS0`, `ttyS1`, `ttyUSB0`, etc. on Linux.
+ *Modbus*:
  + *Slave address*: The address of the Modbus slave device (the PLC).
  + *Data address*: The address of the first register the program will send the results.
+ *Test*:
  + *Test Mode*: Turn test mode ON/OFF.
  + *Output status*: The status to send to the output.
  + *Min value*, *Max value*, *Value step*, *Time step*: Both the measurement and entropy values will
    increase from *Min value* to *Max value* by *Value step* steps on each *Time step*. The entropy value will be processed
    further.
  + *Reset timer*: Reset all test outputs to their initial values.

### Modbus output

If serial output mode is turned on, the program will set consecutive 16-bit registers starting with address *Data address* on the Modbus slave device. The outputs are the following:

Address Offset | Number of 16-bit registers | Value
--- | --- | ---
+0 | 1 | The measurement value.
+1 | 1 | The status of the sensor. (See below for status codes.)
+2 | 1 | The entropy of the histogram. (Used for contrast detection.)

Status codes:

Numeric value | Status string on the console | Meaning
--- | --- | ---
0 | OK | The measurement value is valid.
1 | ProcessingFailed | The object was not found in the input image.
2 | ProcessingInvalidParams | Invalid parameters were specified (like an empty ROI).
3 | InputFailed | Failed to read the input image. (E.g. connection to the camera is lost.)

## Known bugs

## License

MIT

If you have any questions, ideas, suggestions, contact me.
