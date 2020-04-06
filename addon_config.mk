meta:
	ADDON_NAME = ofxOrbbecAstra
	ADDON_DESCRIPTION = OF wrapper Orbbec Astra SDK
	ADDON_AUTHOR = Matt Felsen
	ADDON_TAGS = "orbbec" "astra" "depth camera"
	ADDON_URL = https://github.com/mattfelsen/ofxOrbbecAstra

common:	

vs:
    # version 2.1.0
    ADDON_INCLUDES = src
    ADDON_INCLUDES += libs/astra_v2.1.0/include
	ADDON_LIBS_EXCLUDES = libs/astra_v0.5.0/
	ADDON_DATA = libs/astra_v2.1.0/lib/vs/*
	ADDON_DLLS_TO_COPY  = libs/astra_v2.1.0/lib/vs/

linux64:
    # version 2.0.19
    ADDON_INCLUDES = libs/astra_v2.0.19/include
	ADDON_LIBS = libs/astra_v2.0.19/lib
	ADDON_DATA = libs/astra_v2.0.19/lib/linux64/*	
	ADDON_DLLS_TO_COPY  = libs/astra_v2.0.19/lib/linux64/

osx:
    ADDON_INCLUDES = libs/astra_v0.5.0/include
    ADDON_LIBS = libs/astra_v0.5.0/lib/osx/libastra_core_api.dylib
    ADDON_LIBS += libs/astra_v0.5.0/lib/osx/libastra_core.dylib
    ADDON_LIBS += libs/astra_v0.5.0/lib/osx/libastra.dylib


    # ↑↑↑ this enables to excute app from Xcode. But not by double clicking .app from finder
    # To make standalone app package, we need additional process (copy and install_name_tool).
    # Check Xcode project setting->Build Phase->RunScript section and find how oF manages libfmodex.dylib

    # Another nice option is dylibbundler https://github.com/auriamg/macdylibbundler
    # You might see warning saying libmodex.dylib is missing but you can simply solve it by passing relative path to directory of libfmodx.dylib (see sample command below)
    # Sample useage of dylibbundler (please install first)
    # $ cd <your-project-path>/bin
    # $ dylibbundler -od -b -x ./example.app/Contents/MacOS/example -d ./example.app/Contents/libs
    #
    #/!\ WARNING : Cannot resolve path '@executable_path/../Frameworks/libfmodex.dylib'
    #/!\ WARNING : Library libfmodex.dylib has an incomplete name (location unknown)
    # Please specify now the directory where this library can be found
    # $ ./example.app/Contents/Frameworks
    # Fixing dependency blur blur blur
    # ....
    # Now you can double click and launch your app
    # more discussion at here https://forum.openframeworks.cc/t/dynamic-libraries-for-osx-addon/19908/11

	# All of the libs, including those inside the Plugins folder, do need to be
	# available for loading at runtime, so copy them into the bin folder,
	# maintaining the folder structure
    # this does not work on OSX
    # ADDON_DLLS_TO_COPY  = libs/astra/lib/osx/