Van Helsing game research
=========================

This project was initiated for fun and learning. The main goal is to research the internals of the game *The Incredible Adventures of Van Helsing*. I plan to reverse engineer interesting parts of the game and its file formats. Eventually, it may become useful for myself and other developers, or even end-users.

I with focus on extracting info and mainly do work related to the saves. Modding the game and creating proper game-save editors may not be too far away. :)

Research is currently done by myself, Steffen André Langnes. Please don't just take my work and claim it as your own. :)

A few links for search engines:

- [Dead Island Save Editor (DISE)](http://deadislandsaveeditor.steffenl.com/) - My main project, a game-save editor.
- ["Private" blog](http://blog.steffenl.com/)

## Coding environment
- Operating system: Windows 7 and later.
- IDE/Compiler: Visual Studio/C++ 2013 with update 4.
- Although "cross-platform" code is favored, no extra effort is being made to compile and run the code on other platforms.
- Third party libraries (except Boost) are included, and must be built using included build scripts.

## Current programs
- Unpacker for N2PK packages (command line) (*Files.N2PK*). These files contain textures, models, etc.
- Storage viewer (command line) (*storage.sav*). Displays all of the items you have in your storage.
- Storage editor.

## Game-save location (PC)
`Documents\NeocoreGames\Van Helsing\SaveGame\<number>`

## Files

- *.sav - Game-save/Character.
- timestamp.hts
- backup\*.sav - Backup of previous saves.
- common\storage.sav - Your own storage/chest.
- achievements.sav
