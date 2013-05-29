Van Helsing game research
=========================

This project was initiated for fun and learning. The goal is to research the internals of the game *The Incredible Adventures of Van Helsing*. The idea is to reverse engineer interesting parts of the game and its file formats. Eventually, it may become useful for myself and other developers, or even end-users.

Research is currently done by myself, Steffen André Langnes. Please don't just take my work and claim it as your own. :)

A few links for search engines:

- [Dead Island Save Editor (DISE)](http://deadislandsaveeditor.steffenl.com/) - My main project, a game-save editor.
- ["Private" blog](http://blog.steffenl.com/)

## Current programs
- Unpacker for N2PK packages (*Files.N2PK*). These files contain textures, models, etc.
- Storage viewer (*storage.sav*). Displays all of the items you have in your storage.

## Pages

- [Keep the game running in the background](https://github.com/SteffenL/Van-Helsing-game-research/wiki/Keep-the-game-running-in-the-background)
- [Skip logos on startup](https://github.com/SteffenL/Van-Helsing-game-research/wiki/Skip-logos-on-startup)
- [Use the 32 bit version of the game on 64 bit systems](https://github.com/SteffenL/Van-Helsing-game-research/wiki/Use-the-32-bit-version-of-the-game-on-64-bit-systems)

## Game-save location (PC)
`Documents\NeocoreGames\Van Helsing\SaveGame\21801362`

## Files

- *.sav - Game-save/Character.
- timestamp.hts
- backup\*.sav - Backup of old saves.
- common\storage.sav - Storage/Chest?
- achievements.sav

## Saves (and such) inside repo (for research)

Very incomplete, but these files are the files I'm workong with *at the moment*.

- bookmarks.hbk - Bookmarks for Hex Workshop. Use with `TestCrve_orig.sav` to locate interesting values.
- TestCrve_orig.sav - The original (well, almost original) save. Revert `TestCrve.sav` to this one when needed.
- TestCrve.sav - Modded save or whatever; should not really keep it in the repo.