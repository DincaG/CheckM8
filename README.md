# CheckM8
SFML Win32 based chess game

Requires SFML. Starting from version 2.0.0, it additionally requires nlohmann-json library, curl and openssl for online mode.
## Setup instructions using Visual Studio and vcpkg
```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg integrate install
.\vcpkg\vcpkg install sfml
.\vcpkg\vcpkg install nlohmann-json
.\vcpkg\vcpkg install curl
.\vcpkg\vcpkg install openssl
```
