## Maya Math Nodes
Collection of math nodes for Autodesk Maya

#### Building the code
To build the library on Windows, clone the repository and navigate to the cloned directory,
then run the following:

```
mkdir build
cd build
cmake ../. -G "Visual Studio 15 2017 Win64" -DMAYA_VERSION=2017
```

This will generate a Visual Studio solution you can use to build.

To build the library on OSX use the following command:

```
mkdir build
cd build
cmake ../. -G "CodeBlocks - Unix Makefiles" -DMAYA_VERSION=2017
make
```

Alternatively you can use an IDE that supports CMake projects, such as CLion.

Note that Linux build is currently not supported, PRs for Linux support are welcomed!

#### Notes
Some environments require having a specific unique prefix for custom node names.
This can be achieved by passing the following variable to the cmake command:

```-DNODE_NAME_PREFIX=prefix```
