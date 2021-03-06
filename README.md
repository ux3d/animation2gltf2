[![](glTF.png)](https://github.com/KhronosGroup/glTF/tree/master/specification/2.0)

# Generate animation glTF 2.0 content

animation2gltf2 is a command line tool for generating animation glTF 2.0 content.  
A rotation and translation can be generated.  

Usage: `animation2gltf2.exe [-o rotation -t 2.0 -x 0.0 -y 1.0 -z 0.0 -b false -d 360.0]`  

`-o rotation` Default operation `rotation`. Set `translation` as another operation.  
`-t 2.0` Default duration of 2 seconds for the complete animation.  
`-x 0.0` Rotation x-axis or end position of translation.  
`-y 1.0` Rotation y-axis or end position of translation.  
`-z 0.0` Rotation z-axis or end position of translation.  
`-b false` Bounce start and end position.  
`-d 360.0` Degrees to rotate between.  


## Software Requirements

* C/C++ 17 compiler e.g. gcc or Visual C++
* [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/packages/release/2021-03/r/eclipse-ide-cc-developers) or  
* [CMake](https://cmake.org/)  


## Import the generated glTF

Import the generated glTF in e.g. [Gestaltor](https://gestaltor.io/) and reuse in your scene.  

A short tutorial can be found here: [Gestaltor - How to use an imported animation](https://docs.gestaltor.io/#use-an-imported-animation).  
