# Windows Service Template

This repository provides a C++ template for creating a Windows Service, with the necessary files to install, uninstall, and control the service.

- [Windows Service Template](#windows-service-template)
  - [Features](#features)
  - [Compilation](#compilation)
  - [Running](#running)
  - [Programming Guide](#programming-guide)
    - [What to Care About](#what-to-care-about)
    - [More Details](#more-details)
  - [Acknowledgements](#acknowledgements)
  - [License](#license)

## Features

- C++ Windows Service template
- Use C++17, CMake
- Service Installer for easy installation and removal
- Base classes for creating custom Windows Services
- Logging and error handling

## Compilation

```powershell
cd <project_root>
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

The executable will be located at
`<project_root>\build\main\Release\service.exe`,
or - if you compiled in Debug mode - at
`<project_root>\build\main\Debug\service.exe`.

In case you use Ninja as the generator
(by adding `-G Ninja` to the cmake command above),
the executable will be located at
`<project_root>\build\main\service.exe`.

## Running

Suppose the executable is named `service.exe`.
**The following commands need Admin privileges.**

- Install the service:

  ```powershell
  .\service.exe -install
  ```

- Remove the service:

  ```powershell
  .\service.exe -remove
  ```

- Start, stop, or control the service using the
    Windows Services control panel, or command-line
    tools like `sc` or `net`.

- View the service logs using:
  - the Windows Event Viewer application; or
  - `dbgview64` from Sysinternals,
      with the following options under
      the "Capture" menu enabled:

      - Capture Win32
      - Capture Global Win32
      - Passthrough
      - Capture Events

## Programming Guide

In this section we're talking about the files
located under the `main/` folder.

### What to Care About

- **WindowsService.h**: Define the service's settings.

- **WindowsServiceImpl.h** and **WindowsServiceImpl.cpp**:
    Implement the service's logic/behavior. Notably,
    the `ServiceWorkerThread` method is where the main
    work happens.

    You may also modify the `OnStart` and `OnStop` methods.

Modify these two files, compile the whole project
and run the resulting executable. That's it.

### More Details

- For logging:
  - You can use the `DebugLog` macro
      from `Logging.h` - this is the simplest
      method. Had you used `std::wcout` like this:

      ```cpp
      std::wcout << L"Hello, World!" << 12 << std::endl;
      ```

      then you could use `DebugLog` like this:

      ```cpp
      DebugLog(L"Hello, World!", 12);
      ```
  
  - You can also log messages to the Windows Event Log
      using the `WriteEventLogEntry` method in `CWindowsServiceImpl`
      (inherited from `CServiceBase`). This function,
      however, currently only supports direct string messages,
      so you may need to format your messages
      using `swprintf` or similar functions
      before feeding them to this function.

- For converting from std::wstring to std::string (UTF-8)
    and vice versa, you can use the utility functions
    in `CharacterEncoding.h`.

- **WindowsService.cpp**: Main file containing the `wmain` function to install, uninstall, or run the service.
- **WindowsService.h**: Header file for the `CWindowsServiceImpl` class derived from `CServiceBase`.
- **ServiceBase.h**: Header file for the `CServiceBase` class, which provides a base class for a service that will exist as part of a service application.
- **ServiceInstaller.cpp**: The logic for installing and uninstalling the service.

## Acknowledgements

This project is based on the
[Windows Service Template](https://github.com/magicsih/WindowsService).

Changes include:

- Convert the whole project to use CMake as the build system.
- Fix some code to use C++17 standard.
- Fix service's long stop time due to busy-waiting in the worker thread
    (now use Events and `WaitForMultipleObjects`).
- Improve service start logic; add the option to autostart the service
    after installation.
- Improve service removal logic.

## License

Copyright (c) 2025 Vu Tung Lam

Copyright (c) 2012 ILHWAN

This project is open source and available under the MIT License,
which is compatible with the license of the original project.

See the [`LICENSE.txt`](./LICENSE.txt) file for details.
