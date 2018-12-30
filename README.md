[![Build Status](https://travis-ci.org/hiroMTB/ofxOrbbecAstra.svg?branch=master)](https://travis-ci.org/hiroMTB/ofxOrbbecAstra)
[![Build status](https://ci.appveyor.com/api/projects/status/p5hc034bjff0pj16?svg=true)](https://ci.appveyor.com/project/hiroMTB/ofxorbbecastra)

# ofxOrbbecAstra

openFrameworks addon for using the [Orbbec Astra SDK](https://orbbec3d.com/develop/). 

## Setup


The Astra SDK is bundled in the `libs` folder and does not require a separate download or installation.
Make sure to copy all dynamically linked libraries from the `lib` folder appropriate to your platform into your `bin` folder.


## Support

This has been tested with the following setup:

- openFrameworks 0.9.8
- OSX (Does not support SDK greater than 0.5.0, therefore no native body tracking)
- Windows 10 using Visual Studio
- Ubuntu 16.04
- Orbbec Astra camera

Windows support hopefully coming soon. The libs from the Win32 and x64 SDKs are included, but I haven't had a chance to test this.

