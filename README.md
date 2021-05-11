[![](glTF.png)](https://github.com/KhronosGroup/glTF/tree/master/specification/2.0)

# Generate glTF 2.0 content

gen2gltf2 is a command line tool for generating glTF 2.0 content. For now, a basic animation rotation can be generated

Usage: `gen2gltf2.exe [-d 2.0 -c true]`

`-d 2.0` Default duration of 2 seconds for one complete rotation.  
`-c true` Clockwise rotation. If set to false, the rotation goes the opposite direction.  


## Software Requirements

* C/C++ 17 compiler e.g. gcc or Visual C++
* [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/packages/release/2021-03/r/eclipse-ide-cc-developers) or  
* [CMake](https://cmake.org/)  


## Import the generated glTF

Import the generated glTF in e.g. [Gestaltor](https://gestaltor.io/) and reuse in your scene.  

