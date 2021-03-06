# README #

![MSBuild](https://github.com/Whiteha/umdh-gui/workflows/MSBuild/badge.svg)

Release v1.1 [download](https://github.com/Whiteha/umdh-gui/releases/download/v1.1/umdh-gui.zip)

### About ###
Simple GUI app for umdh utility based on Qt framework.

The User-Mode Dump Heap (UMDH) tool, Umdh.exe, analyzes the Microsoft Windows heap memory allocations for a given process.
[Read more on MSDN](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/umdh)

Features:
- snapshot creation
- report creation
- report view
- filtration for report view
- sorting for report view
- summary info
- different data representations (filtered plain text, list, calls count info, calls tree (*experimental))

## GUI preview ##

Page-wrapper for umdh
![1](https://user-images.githubusercontent.com/5871853/106532424-38f6c080-6501-11eb-91c1-3a538a4cd4a1.png)

Reports viewer
![2](https://user-images.githubusercontent.com/5871853/106532429-3a27ed80-6501-11eb-9eed-0555e108e6d8.png)

*Build was tested with Visual Studio 2019 (v142)
