## Maya Math Nodes
Collection of math nodes for Autodesk Maya

![status](https://img.shields.io/badge/status-alpha-orange.svg)

#### Building
To build the library on Windows, clone the repository and navigate to the cloned directory,
then run the following:

```
mkdir build
cd build
cmake ../. -G "Visual Studio 15 2017 Win64"
```

This will generate a Visual Studio solution you can use to build.

To build the library on OSX or Linux use the following command:

```
mkdir build
cd build
cmake ../. -G "CodeBlocks - Unix Makefiles"
make
```

The build looks for Maya in the default installation directory for each platform, however you can always provide it with a custom path:

```-DMAYA_LOCATION=/apps/autodesk/```

Likewise, the build defaults to Maya version 2018, this can be changed as follows:

```-DMAYA_VERSION=2017```

Some environments require having a specific unique name prefix for custom nodes.
This can be achieved by passing the following variable to the cmake command:

```-DNODE_NAME_PREFIX=prefix```

#### Testing
In order to run the test suite, execute the *mayapy* interpreter and pass the *tests* directory as an argument, for example:

```/Applications/Autodesk/maya2018/Maya.app/Contents/bin/mayapy tests```
