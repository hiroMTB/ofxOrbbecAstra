[![Build Status](https://travis-ci.org/hiroMTB/ofxOrbbecAstra.svg?branch=master)](https://travis-ci.org/hiroMTB/ofxOrbbecAstra)
[![Build status](https://ci.appveyor.com/api/projects/status/p5hc034bjff0pj16?svg=true)](https://ci.appveyor.com/project/hiroMTB/ofxorbbecastra)

# ofxOrbbecAstra

openFrameworks addon for using the [Orbbec Astra SDK](https://orbbec3d.com/develop/). 

## Setup

The Astra SDK is bundled in the `libs` folder and does not require a separate download or installation.
Make sure to copy all dynamically linked libraries from the `lib` folder appropriate to your platform into your `bin` folder.

## Astra SDK version
Astra SDK 2.0.19

## Support

This has been tested with the following setup:

- openFrameworks 0.11.0
- Orbbec Astra camera
- [x] Ubuntu 16.04
- [x] macOS 10.13.6
- [ ] Windows 10, Visual Studio ( Probabry I just need to replace dynamic lib files )

## Important notice using on macOS
- Orbbec stops macOS support after Astra SDK 0.5.0.
- It does not support body tracking feature.
- Current master branch uses **Astra SDK 2.0.19** which has different API from 0.5.0, hence master branch does not work on macOS.
- Please use [OSX-AstraSDK.0.5.0](https://github.com/hiroMTB/ofxOrbbecAstra/tree/OSX-AstraSDK.0.5.0) branch for macOS
- We can not download Astra SDK from Orrbec website but this branch contains all necessaly library files(.dylib and headers).
- However!! you might need OpenNI driver installed (I will check this later...)
