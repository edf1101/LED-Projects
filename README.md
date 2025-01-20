# Arduino LED Projects Code
_Code to support various Embedded system LED based projects I have built over the years_

## Introduction
Having built many LED based projects in the past I wrote this code to unify them all with simpler, more readable code 
that promotes modularity and reusability. This code is designed to be used with PlatformIO and the ESP32 platform.

## Projects

This section shows the projects that work with this code and how to declare them.

### 6x6 Display Project

![alt text](res/6x6Grid.jpg)
```cpp
// Create an instance of the project
Project *myProject = new Grid("16x8 Grid",10, NEO_GRB, NEO_KHZ800,
6, 6, ROTATION_270, ROW_MAJOR);

// in setup()
myProject->init();
myProject->addWifi("...", "...");
```

### 16x8 Clock Project
```cpp
// Create an instance of the project
Project *myProject = new Grid("16x8 Grid",4, NEO_GRB, NEO_KHZ800,
                              16, 8, ROTATION_0, ROW_MAJOR);

// in setup()
myProject->init();
myProject->addAudioAnalysis(14, 15, 32, false);
myProject->addWifi("...", "...");
```
![alt text](res/16x8Grid.jpg)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details