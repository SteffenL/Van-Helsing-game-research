Van Helsing game research
=========================

This project was initiated for fun and learning. The main goal is to research the internals of the game *The Incredible Adventures of Van Helsing*. With some file formats researched, this work may become useful for other developers and end-users.

&copy; Steffen André Langnes.

## Coding environment

- Operating system: Windows 7 and later.
- IDE/Compiler: Visual Studio/C++ 2013 with update 4.

## Building

Note: Although "cross-platform" code is favored, no extra effort is being made to compile and run the code on platforms other than Windows.

### Preparation

- Point the environment variable `BOOSTDIR` to the directory of the Boost library. Make sure to build Boost as well.
- Open the `Developer Command Prompt for VS2013` (make sure Git is available).
- Initialize the repository:

        git clone --recursive --branch=master <repository url>
        cd Van-Helsing-game-research

- Build dependencies:

        powershell -ExecutionPolicy Bypass -File Code\scripts\build\thirdparty.ps1

### Build
 
- Update the version number if needed:

        powershell -ExecutionPolicy Bypass -File Code\scripts\UpdateVersion.ps1 <version>

- Build:

        msbuild Code\VanHelsingGameResearch.sln /t:Build /p:Configuration=Release

## Packing for release

- Point the environment `SIGNTOOL` to the full path to the [SignTool](https://msdn.microsoft.com/en-us/library/8s9b9yaz(v=vs.110).aspx) executable.
- Create package(s):

        Code\scripts\CreateReleasePackage.bat

## Current programs

- Unpacker for N2PK packages (command line) (*Files.N2PK*). These files contain textures, models, etc.
- Storage viewer (command line) (*storage.sav*). Displays all of the items you have in your storage.
- Storage editor.

## Game-save location (PC)

`Documents\NeocoreGames\Van Helsing\SaveGame\<number>`

## Files in the game-save location

Name               | Description
-------------------|--------------------------
*.sav              | Game-save/Character.
timestamp.hts      |
backup/*.sav       | Backup of previous saves.
common/storage.sav | Shared storage/chest.
achievements.sav   |
