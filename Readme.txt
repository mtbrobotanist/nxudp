nxudp version 1.0.0
--------------------
A sample client/server application demonstrating the use of UDP sockets.

This project was build using asio version 1.12.0 (https://github.com/chriskohlhoff/asio.git)


Supported Operating Systems
---------------------------
* Linux Ubuntu 16.04
* Windows 10

Although this program is likely to work on older Operating Systems (Ubuntu 14.04, Windows 7, etc.), it has not been tested.


Understanding This Guide
-----------------------

The text above is a headline. it represents a section of information.
This guide is split into two major parts:

  * UBUNTU
  * WINDOWS

Each part has build and run instructions in the following format:
```
$ some-command
<output>
```

The text above is an example of running a commmand on the command line.
the dollar sign, $ , is where the user typed a command, and the word <output> below it is the output of that command.
The remainder of this guide will have command line examples as shown above.



Assumptions and Limitations
---------------------------
Various parts of this guide have examples with text enclosed in <brackets>.
These parts are meant to be replaced with actual values by the person following along with this guide.

Finally, this guide does not explain how to install the required dependencies. It only lists what they are for each platform.

NOTE: if you obtained this repository via git, you will need to initialize asio as a submodule
```
$ cd <path/to/nxudp-repo>
$ git submodule update --init --recursive
```

otherwise, it should already be located under the following project directory: vendor/asio/asio



UBUNTU
------

required tools:
  * cmake (3.5 or newer)
  * g++ or clang
  * make

Installation
------------

There are two ways to install.

1. run the build/install script inside of the install_os directory
2. build and install manually from command line


Install Script
--------------

```
$ cd <path/to/nxudp/install_os>
$ source install-linux.sh
```

Manual Build and Install
------------------------

Open a terminal (Ctrl + Alt + T):
```
$ cd <path/to/nxudp>
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/opt/nxudp ..
$ make
$ sudo make install
```

NOTE: both of these will require root privileges for the final step. Enter your password when prompted.


Running the program
-------------------

The program should now be installed to the following directory: /opt/nxudp

Server mode: in a terminal, type:
```
$ cd /opt/nxudp/bin
$ ./app -s
Listening port <12345>
```

The port number in the example <12345> above will not be 12345. take note of the port number your
output produced for use in the next section.


Client mode: in another terminal type:
```
$ cd /opt/nxudp/bin
$ ./app -c 127.0.0.1:<12345> -n 5000
Sent datagram with value “5000” to 127.0.0.1:12345. Waiting for response...
```


Replace the number <12345> the output from your server mode command (omitting the brackets <>).
This starts the app in client mode, which requires it to be pointed at the  <ip-address>:<port>  combination of the app that was started in server mode.

In the example above, 5000 indicates the server should wait 5 seconds (5000 milliseconds) before sending a response

Each program should produce additional output, shown here below the original commands:


Server Mode
```
$ ./app -s
Listening port <12345>
Received request from 127.0.0.1:12345 with value “5000”.
Sent response “DONE” to 127.0.0.1:12345.
```


In the example above, the final line, "Sent response “DONE” to 127.0.0.1:12345.", will have appeared 5 seconds after the line above it.

The server has been tested with up 200,000 simultaneous clients each with a 1000 millisecond wait time


Client mode
```
$ ./app -c 127.0.0.1:<12345> -n 5000
Sent datagram with value “5000” to 127.0.0.1:12345. Waiting for response...
Received response “DONE” from 127.0.0.1:49452.
Exiting...
```

the final two lines will appear after the client receives a response from the server




WINDOWS
-------
required tools:
  * cmake 3.5 https://cmake.org/download/
  * VS Build Tools https://www.visualstudio.com/downloads/?q=build+tool#build-tools-for-visual-studio-2017

NOTE: this program has been tested against msvc 14.1, which comes installed with Visual Studio 2017.
While older compilers(msvc 14.0 with VS 2015 for example) are likely to work, they have not been tested.

Installation
------------

There are two ways to install.

1. run the build/install script inside of the install_os directory
2. build and install manually from command line


Install Script
--------------

Open a terminal:
  1. open the start menu
  2. type cmd.exe
  3. select the highlighted option (Command Prompt)

```
$ cd <path\to\nxudp\install_os>
$ install_windows.bat
```

Manual Build and Install
------------------------

In a terminal:
```
$ cd <path\to\nxudp>
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --target install
```


This will install the program to the following directory:

* C:\Program Files (x86)\nxudp

Running The Program
-------------------


Server mode: In a terminal:
```
$ cd "C:\Program Files (x86)\nxudp\bin"
$ .\app.exe -s
Listening port <12345>
```


The port number in the example <12345> above will not be 12345. take note of the port number your output produced for use in the next section.


Client mode: in another terminal:
```
$ cd "C:\Program Files (x86)\nxudp\bin"
$ .\app.exe -c 127.0.0.1:<12345> -n 5000
Sent datagram with value “5000” to 127.0.0.1:12345. Waiting for response...
```


Replace the number <12345> the output from your server mode command (omitting the brackets <>).
This starts the app in client mode, which requires it to be pointed at the  <ip-address>:<port>  combination of the app that was started in server mode.

In the example above, 5000 indicates the server should wait 5 seconds (5000 milliseconds) before sending a response

Each program should produce additional output, shown here below the original commands:


Server Mode
```
$ .\app.exe -s
Listening port <12345>
Received request from 127.0.0.1:12345 with value “5000”.
Sent response “DONE” to 127.0.0.1:12345.
```


In the example above, the final line, "Sent response “DONE” to 127.0.0.1:12345.", will have appeared 5 seconds after the line above it.

The server has been tested with up 200,000 simultaneous clients each with a 1000 millisecond wait time



Client mode
```
$ ./app -c 127.0.0.1:<12345> -n 5000
Sent datagram with value “5000” to 127.0.0.1:12345. Waiting for response...
Received response “DONE” from 127.0.0.1:49452.
Exiting...
```


the final two lines in the example above will appear after the client receives a response from the server



```
$ Special thanks to Andrey Kolesnikov for giving me this assignment.
```
