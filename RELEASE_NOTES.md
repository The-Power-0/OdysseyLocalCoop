# OdysseyTogether development build

The integrated 1.4.1 build includes the English `Local Co-op` confirmation screen and the Spark Pylon Test 3 changes. Extract its `OdysseyTogether` folder into Ryujinx's mod directory. Disable any older co-op mod and the separate `OCoop Guide` entry. The guide copy uses the game's existing layout and applies while that layout loads.

P2 completed the Cap Kingdom pylon crossing to Cascade in Ryujinx with Test 3. P1, later wires, and the English guide in the integrated executable still need gameplay checks. The release notes below apply to the published alpha.6 download.

## OdysseyTogether v0.1.0-alpha.6

The first OdysseyTogether prerelease is for **Super Mario Odyssey 1.4.1 on Ryujinx** using the original Switch game build. Start two full players on a shared screen through the pause menu. This fork builds on [Gudui’s Odyssey Local Co-op](https://github.com/Gudui/OdysseyLocalCoop) and [Colbster937’s executable compatibility work](https://github.com/colbster937/OdysseyLocalCoopPlus).

## Install

1. In Ryujinx, confirm the game list shows **1.4.1**. Right-click Odyssey and select **Open Mods Directory**.
2. Extract **OdysseyTogether-v0.1.0-alpha.6-smo-1.4.1-Ryujinx.zip** into that directory. You should have one **OdysseyTogether** folder containing **exefs** and **romfs**.
3. If you installed an earlier test build, move its loose co-op `exefs` and `romfs` folders out of Odyssey’s mod directory before launch. If you used a named folder, disable that old mod instead. Keep the separate `OCoop Guide` folder if you already installed it.
4. Configure two controllers. Launch Odyssey and choose **Two Players** from the in-game pause menu.

The optional [English co-op guide patcher](https://github.com/The-Power-0/OdysseyLocalCoop/blob/v0.1.0-alpha.6/scripts/patch-coop-guide.py) edits the text in your own RomFS extract. The main mod installs by extracting the ZIP.

## Included

- The tested 1.4.1 executable check and independent player controls.
- Shared camera improvements, including earlier zoom-out and bounded vertical framing.
- A shorter recovery wait when the shared follow camera estimates one player is clearly out of frame and the other can provide a safe anchor.
- A hidden Joy-Con startup guide. Loading can briefly show a black screen before the normal title menu appears.
- Development recovery safeguards, story diagnostics and the opt-in Test Lab. The Test Lab is off by default.

## Known issues

The Cap Kingdom Spark Pylon can stall in co-op. For a new save, play through Topper and the pylon in one-player mode, then save in Cascade before enabling co-op. Avoid changing modes between defeating Topper and reaching Cascade. In the Startup test build, the mode switch reloaded Cap Kingdom with the defeat recorded and the fight trigger still active. That save crashed on re-entry even with the mod removed. Clearing the defeat flag on a test copy let the player fight Topper again and cross to Cascade. The published alpha.6 build has not been checked for this reproduction.

Metro, Lake and Lost Kingdom fixes still need gameplay validation. This prerelease was tested in Ryujinx on the original Switch 1.4.1 executable.

[Cap Kingdom #11](https://github.com/The-Power-0/OdysseyLocalCoop/issues/11) · [Metro #2](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2) · [Lake #3](https://github.com/The-Power-0/OdysseyLocalCoop/issues/3) · [Lost Kingdom #4](https://github.com/The-Power-0/OdysseyLocalCoop/issues/4) · [Double falls #5](https://github.com/The-Power-0/OdysseyLocalCoop/issues/5)
