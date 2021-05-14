[![](glTF.png)](https://github.com/KhronosGroup/glTF/tree/master/specification/2.0)

# Generate glTF 2.0 content

gen2gltf2 is a command line tool for generating glTF 2.0 content.  
An animation rotation and translation can be generated.  

Usage: `gen2gltf2.exe [-o rotation -d 2.0 -x 0.0 -y 1.0 -z 0.0 -b false]`  

`-o rotation` Default operation `rotation`. Set `translation` as another operation.  
`-d 2.0` Default duration of 2 seconds for the complete animation.  
`-x 0.0` Rotation x-axis or end position of translation.  
`-y 1.0` Rotation y-axis or end position of translation.  
`-z 0.0` Rotation z-axis or end position of translation.  
`-b false` Bounce start and end position.  


## Software Requirements

* C/C++ 17 compiler e.g. gcc or Visual C++
* [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/packages/release/2021-03/r/eclipse-ide-cc-developers) or  
* [CMake](https://cmake.org/)  


## Import the generated glTF

Import the generated glTF in e.g. [Gestaltor](https://gestaltor.io/) and reuse in your scene.  

A short tutorial can be found here: [Gestaltor - How to use an imported animation](https://docs.gestaltor.io/#use-an-imported-animation).  
