Van Helsing game research
=========================

This project was initiated for fun and learning. The main goal is to research the internals of the game *The Incredible Adventures of Van Helsing*. I plan to reverse engineer interesting parts of the game and its file formats. Eventually, it may become useful for myself and other developers, or even end-users.

I with focus on extracting info and mainly do work related to the saves. Modding the game and creating proper game-save editors may not be too far away. :)

Research is currently done by myself, Steffen André Langnes. Please don't just take my work and claim it as your own. :)

A few links for search engines:

- [Dead Island Save Editor (DISE)](http://deadislandsaveeditor.steffenl.com/) - My main project, a game-save editor.
- ["Private" blog](http://blog.steffenl.com/)

## Coding environment
- Operating system: Windows 8.
- IDE/Compiler: Visual Studio/C++ 2012.
- Code is written to be cross-platform, but the GUI is coded with WTL (Windows Template Library), specifically for Windows.
- 3rd party libraries are included (except Boost).

## Current programs
- Unpacker for N2PK packages (*Files.N2PK*). These files contain textures, models, etc.
- Storage viewer (command line) (*storage.sav*). Displays all of the items you have in your storage.
- Storage editor (GUI). Very early stage; only basic viewing at this time.

## Early preview
![Storage Editor](https://raw.github.com/wiki/SteffenL/Van-Helsing-game-research/images/mainwindow.png)

## Pages

- [Keep the game running in the background](https://github.com/SteffenL/Van-Helsing-game-research/wiki/Keep-the-game-running-in-the-background)
- [Skip logos on startup](https://github.com/SteffenL/Van-Helsing-game-research/wiki/Skip-logos-on-startup)
- [Use the 32 bit version of the game on 64 bit systems](https://github.com/SteffenL/Van-Helsing-game-research/wiki/Use-the-32-bit-version-of-the-game-on-64-bit-systems)
- [List of items and IDs](https://github.com/SteffenL/Van-Helsing-game-research/wiki/List-of-items-and-IDs)
- [List of enchantments and IDs](https://github.com/SteffenL/Van-Helsing-game-research/wiki/List-of-enchantments-and-IDs)

## Game-save location (PC)
`Documents\NeocoreGames\Van Helsing\SaveGame\21801362`

## Files

- *.sav - Game-save/Character.
- timestamp.hts
- backup\*.sav - Backup of previous saves.
- common\storage.sav - Your own storage/chest.
- achievements.sav

## Saves (and such) inside repo (for research)

Very incomplete, but these files are the files I'm working with *at the moment*.

- bookmarks.hbk - Bookmarks for Hex Workshop. Use with `TestCrve_orig.sav` to locate interesting values.
- TestCrve_orig.sav - The original (well, almost original) save. Revert `TestCrve.sav` to this one when needed.
- TestCrve.sav - Modded save or whatever; should not really keep it in the repo.