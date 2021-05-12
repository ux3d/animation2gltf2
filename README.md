[![](glTF.png)](https://github.com/KhronosGroup/glTF/tree/master/specification/2.0)

# Generate glTF 2.0 content

gen2gltf2 is a command line tool for generating glTF 2.0 content. For now, a basic animation rotation can be generated

Usage: `gen2gltf2.exe [-o rotation -t 2.0 -a y -d cw -e 2.0 -b false]`

`-o rotation` Default operation `rotation`. Set `translation` as another operation.  
`-t 2.0` Default duration of 2 seconds for the complete animation.  
`-a y` Default rotation or translation axis. Set `x` or `z` for other axis.  
`-d cw` Default clockwise rotation. Set `ccw` for opposite direction.  Only for `rotation` operation.  
`-e 2.0` End target value.  Only for `translation` operation.  
`-b false` Bounce between end value.  Only for `translation` operation.  


## Software Requirements

* C/C++ 17 compiler e.g. gcc or Visual C++
* [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/packages/release/2021-03/r/eclipse-ide-cc-developers) or  
* [CMake](https://cmake.org/)  


## Import the generated glTF

Import the generated glTF in e.g. [Gestaltor](https://gestaltor.io/) and reuse in your scene.  

A short tutorial can be found here: [Gestaltor - How to use an imported animation](https://docs.gestaltor.io/#use-an-imported-animation).  
