<style>
g { color: #87D687 }
</style>

![FlipReady Logo Long](https://github.com/KieranCanter/FlipReady/assets/74733079/daf10696-262d-4701-a09c-e532a969c486)

# Introduction

FlipReady is a BakkesMod plugin for Rocket League. Enabling the plugin will display text on screen that indicates whether you are eligible to air flip/double jump or not. After jumping, a gauge bar will appear and decay from full to empty to represent the time your air flip/double jump is active. Settings include customizing colors, sizes, and the location of text/gauge bar.

*Due to the unfairness/imbalance this can cause to competitive play, this plugin is only active within freeplay/exhibition game modes.*

# Installation

* Copy `FlipReady.dll` to your `bakkesmod/plugins` folder, most likely located at: 
  `C:/Users/{username}/AppData/Roaming/bakkesmod/bakkesmod/plugins`

* Plugin will load automatically with Rocket League while BakkesMod is active. The plugin can be disabled through the F2 settings window or through the F6 console with `plugin unload flipready`. Similarly, the plugin can be reenabled through the F2 settings window or through the F6 console with `plugin load flipready`.

# Commands
Settings are found in the F2 settings window; however, you may also use the following commands in the F6 console (<g>default values are in green</g>):

| Command                    | Arguments                            | Effect                                            |
| -------------------------- | ------------------------------------ | ------------------------------------------------- |
| flipready_enabled          | [ 0 \| <g>1</g>]                     | [ Disable plugin \| Enable plugin]                |
| flipready_color_fliptext   | [ <g>#00FF00FF</g> ]                 | [ Change "flip" text color with RGBA hexcode ]    |
| flipready_color_nofliptext | [ <g>#FF0000FF</g> ]                 | [ Change "no flip" text color with RGBA hexcode ] |
| flipready_color_gaugebar   | [ <g>#00FF00FF</g> ]                 | [ Change gauge bar color with RGBA hexcode ]      |
| flipready_fontsize         | [ 1 - 100 ] [ <g>20</g> ]            | [ Change font size ]                              |
| flipready_barlen           | [ 1 - 100 ] [ <g>20</g> ]            | [ Change gauge bar length ]                       |
| flipready_barheight        | [ 1 - 25 ] [ <g>5</g> ]              | [ Change gauge bar height ]                       |
| flipready_decaydir         | [ <g>left</g> \| right ]             | [ Change direction gauge bar decays ]             |
| flipready_positionx        | [ left \| <g>middle</g> \| right ]   | [ Change horizontal position ]                    |
| flipready_positiony        | [ <g>top</g> \| middle \| bottom ]   | [ Change vertical position ]                      |

**Example:** If I want to make the "flip" text blue, change the font size to 25, and set the horizontal position to the left, I would type into the F6 console:
  ```
  flipready_color_fliptext #0000FFFF
  flipready_fontsize 25
  flipready_positionx left
  ```
