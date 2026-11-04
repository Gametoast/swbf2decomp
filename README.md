# Star Wars: Battlefront II (2005) Decompiled

## Progress

```
Implemented:  99.37%  (19261 / 19383)
Perfect:      16354 / 19383
Accuracy:     96.16%
Progress:     95.55%
```

## Goals

**Note: This project is still a work in progress and should NOT be considered complete or byte-accurate to the original. While it currently does compile and run, until every function has been 100% matched, there may be unknown issues, particularly when playing online.**

This is a fork of the SWBF2 decompilation project started by Fredly (https://github.com/OpenSWBF2/swbf2decomp). The goal is to improve the accuracy and get as close as possible to a 100% byte-identical build. This project is based on the Steam version of the game, you will need to copy your `BattlefrontII.exe` from your GameData folder to the project root and rename it to `BattlefrontII_Steam.exe`. This project uses a forked version of `reccmp` available here: https://github.com/Alpha5k/reccmp

**Stage 1a**: Get as close to a 100% byte-identical build as possible. Due to compiler entropy, a true 1:1 build may be infeasible, but each function should at least be instruction-matched and confirmed to be identical to the source binary.

**Stage 1b**: Verify each function and global name against the PDBs we have access to. We do not have a PDB for the Steam version of the game, but we do have several for various debug builds, which contain 95% of the same functions. Every function, class, global, and static variable should be correctly mapped to its corresponding symbol in the source tree, including its types and arguments.

**Stage 2**: Clean up the source tree and reconstruct the original function layout, including variable names and calling conventions. Many of the functions currently compile to the same as the original, but use "hacks" including repeated type casting, raw pointers, and direct assignments. The goal is accurate human-readable source code, and each function should be reviewed by a human for accuracy.

**Stage 3**: Use the decompiled source to create a "community patch" for the game, incorporating fixes from several community projects.

## Contributing

* Copy your `BattlefrontII.exe` from your Steam GameData folder into the project root and rename to `BattlefrontII_Steam.exe`
* Install the forked `reccmp` using the instructions in README: https://github.com/Alpha5k/reccmp
* Install the prerequisites including VS 2013 and the DXDK SDK
* Compile the game using the build command below
* Run `reccmp-reccmp --target SWBF2` to see the current progress

When you make a change to a function, you can run `reccmp-reccmp --target SWBF2 --verbose <address>` to see the accuracy score for that specific function. If you add a new function, make sure to add a reccmp annotation for it so it can be tracked.

## Prerequisites

- Visual Studio 2013 (Update 5) with the v120_xp platform toolset
  https://visualstudio.microsoft.com/vs/older-downloads/
- DirectX SDK (June 2010), which sets DXSDK_DIR
  https://www.microsoft.com/en-us/download/details.aspx?id=6812

## Build

```
C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" BattlefrontII.sln /t:Rebuild /p:Configuration=Release /p:Platform=Win32 /p:SWBF2_NOSTAGE=true
```

The exe is written to `bin\{Platform}\{Config}\BattlefrontII.exe`.

## Run

The build automatically finds the Steam install's GameData through Steam's registry entry, or set `SWBF2_GAMEDATA` to the desired GameData directory to use another copy.
