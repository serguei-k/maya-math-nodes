## Maya Math Nodes
Collection of math nodes for Autodesk Maya

#### Building
[![Build](https://github.com/serguei-k/maya-math-nodes/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/serguei-k/maya-math-nodes/actions/workflows/build.yml)
[![Documentation Status](https://readthedocs.org/projects/maya-math-nodes/badge/?version=latest)](https://maya-math-nodes.readthedocs.io/en/latest/?badge=latest)

To build the library on Windows, clone the repository and navigate to the cloned directory,
then run the following commands:

```
mkdir build
cd build
cmake ../. -G "Visual Studio 16 2019 Win64"
```

This will generate a Visual Studio solution you can use to build.

To build the library on OSX or Linux use the following commands:

```
mkdir build
cd build
cmake ../. -G "CodeBlocks - Unix Makefiles"
make
```

The build looks for Maya in the default installation directory for each platform, however you can always provide it with a custom path:

```
-DMAYA_LOCATION=/apps/autodesk/
```

Likewise, the build defaults to Maya version 2022, this can be changed as follows:

```
-DMAYA_VERSION=2020
```

Please note that the lowest supported version is 2019.

#### Installation
To install the library on OSX or Linux run the following command:

```
make install
```

By default the resulting module gets installed under *dist* folder in the current source directory.

To make the module available in Maya, add the install location to *MAYA_MODULE_PATH* environment variable or copy the module to a directory already in the module path.

The install location can also be changed as follows:

```
-DINSTALL_DIR=/preffered/install/directory
```

#### Testing
In order to run the test suite, execute the *mayapy* interpreter and pass the *tests* directory as an argument, for example:

```
/Applications/Autodesk/maya2018/Maya.app/Contents/bin/mayapy tests
```
