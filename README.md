# Preloading browser
Preloading version 1.02.2

## Introduction
We use Qt-5.4.2 browser.
Because Qt-wetkit have a very large capacity, we upload two directory in root.

This web browser is Qt-based and Webkit-based browser for a system with NVRAM.

We have modified or implemented the following files
 1. /qtbase/src/widgets/widgets/
	- qtabbar.h
	- qtabbar.cpp
	- qtabbar_p.h
	- qtabwidget.h
	- qtabwidget.cpp

 2. /qtwebkit-examples/examples/webkitwigets/browser/
	- browserapplication.h
	- browserapplication.cpp
	- browsermainwindow.h
	- browsermainwindow.cpp
	- Graph.h
	- Graph.cpp
	- linkedqueue.h
	- linkedqueue.cpp
	- main.cpp
	- my_queue.h
	- my_queue.cpp
	- Node.h
	- Node.cpp
	- tabwiget.h
	- tabwiget.cpp
	- webview.h
	- webview.cpp

## Tested environment
 - OS: Ubuntu 14.04.5 64bit

## Installation
### 1. Install libraries for Qt 5.4 and our browser
 - $ sudo apt-get update   
 - $ sudo apt-get install libxcomposite-dev libxrender-dev libjpeg-dev libx11-dev libpng12-dev libxslt1-dev libxml2-dev gir1.2-glib-2.0 libglib2.0-dev libgstreamer0.10-dev icu-devtools libgl1-mesa-dev libxcb1-dev liblzma-dev libpcre3-dev libffi-dev liborc-0.4-dev libexpat1-dev libxcb-present-dev libxcb-dri3-dev libxcb-sync-dev libxshmfence-dev libglapi-mesa libxext-dev libxdamage-dev libxfixes-dev libx11-xcb-dev libxcb-glx0-dev libxcb-dri2-0-dev libxxf86vm-dev libdrm-dev libxau-dev libxdmcp-dev libcups2-dev libevdev-dev alsa-base fontconfig libfreetype6-dev libharfbuzz-dev libicu-dev libgif-dev libmtdev-dev openssl libegl1-mesa-dev libpulse-dev libmysqlclient-dev libodbc1 libsqlite3-dev libudev-dev zlib1g-dev

### 1. Download and install Qt 5.4 for Linux x64
 - $ wget http://download.qt.io/archive/qt/5.4/5.4.2/qt-opensource-linux-x64-5.4.2.run  
 - $ sudo ./qt-opensource-linux-x64-5.4.2.run

### 2. Get files from this repository
 - $ git clone https://github.com/SeungKyuHong/Preloading

### 3. Copy Preloading files to Qt 5.4
 - Copy the files in qtbase of github to /qtbase/src/widgets/widgets of QT 5.4
 - Copy the files in browser of github to /qtwebkit-examples/examples/webkitwigets/browser/ of QT 5.4

### 4. Make a Makefile for this browser
In / of QT 5.4,
 - First, you sould config the QT using "./configure" command

### 5. Compile and Install the browser
In / of QT 5.4,
 - $ make	  
 - $ sudo make install

### 6. Run the browser

