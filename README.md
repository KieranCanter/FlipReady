![FlipReady Logo Long](https://github.com/KieranCanter/FlipReady/assets/74733079/daf10696-262d-4701-a09c-e532a969c486)

# Introduction

FlipReady is a BakkesMod plugin for Rocket League. Enabling the plugin will display text on screen that indicates whether you are eligible to air flip/double jump or not. After jumping, a gauge bar will appear and decay from full to empty to represent the time your air flip/double jump is active. Settings include customizing colors, sizes, and the location of text/gauge bar.

*Due to the unfairness/imbalance this can cause to competitive play, this plugin is only active within freeplay/exhibition game modes.*
<p align="center">
  <img src="https://github.com/KieranCanter/FlipReady/assets/74733079/373de1ed-5877-46d0-be32-822accfd42f5" alt="Intro Demo" />
</p>

![Intro Demo](https://github.com/KieranCanter/FlipReady/assets/74733079/373de1ed-5877-46d0-be32-822accfd42f5)


# Installation

* Copy `FlipReady.dll` to your `bakkesmod/plugins` folder, most likely located at: 
  `C:/Users/{username}/AppData/Roaming/bakkesmod/bakkesmod/plugins`

* Plugin will load automatically with Rocket League while BakkesMod is active. The plugin can be disabled through the F2 settings window (F2 > Plugins > FlipReady) or through the F6 console with `plugin unload flipready`. Similarly, the plugin can be reenabled through the F2 settings window or through the F6 console with  
`plugin load flipready`.

# Settings
The settings can be accessed through `F2 > Plugins > FlipReady`. The current customizable settings include colors, sizes, and location of the element.  

<p align="center">
  <img src="https://github.com/KieranCanter/FlipReady/assets/74733079/c7d49e4e-761c-42c8-88de-7e11f166ef7b" alt="Settings Window" />
</p>
![Settings Window](https://github.com/KieranCanter/FlipReady/assets/74733079/c7d49e4e-761c-42c8-88de-7e11f166ef7b)


## Colors
* The color of both the "flip" and "no flip" text displays, as well as the gauge bar, can be altered to any RGBA, HSLA, or Hexcode color of your choice.
* The color picker on the right allows the option to manually input color values or drag to select. Keep in mind that alpha (opacity) can also be adjusted. The RGBA value boxes can be dragged to scrub through values or ctrl+clicked to manually type them in.
* On selection, the appropriate element will be displayed to actively show the change in color.
* Once altered, buttons will appear to reset the value to default or revert back to the last saved value. Hovering over these buttons will show what the value will be reset/reverted to.

<p align="center">
  <img src="https://github.com/KieranCanter/FlipReady/assets/74733079/711caea8-9f16-4222-92b6-edf8fb10d4eb" alt="Colors" />
</p>
<p align="center">
  <img src="https://github.com/KieranCanter/FlipReady/assets/74733079/6d26e449-8ab0-4d40-b39a-8498edbc1de0" alt="Color Picker" />
</p>
![Colors](https://github.com/KieranCanter/FlipReady/assets/74733079/711caea8-9f16-4222-92b6-edf8fb10d4eb)
![Color Picker](https://github.com/KieranCanter/FlipReady/assets/74733079/6d26e449-8ab0-4d40-b39a-8498edbc1de0)



# Commands
Settings are found in the F2 settings window; however, you may also use the following commands in the F6 console $\color{#87D687}\textsf{(default values in green)}$:

| Command                    | Arguments                                               | Effect                                            |
| -------------------------- | ------------------------------------------------------- | ------------------------------------------------- |
| flipready_enabled          | [ 0 \| $\color{#87D687}\textsf{1}$ ]                    | [ Disable plugin \| Enable plugin]                |
| flipready_color_fliptext   | [ $\color{#87D687}\texttt{\\#00FF00FF}$ ]               | [ Change "flip" text color with RGBA hexcode ]    |
| flipready_color_nofliptext | [ $\color{#87D687}\texttt{\\#FF0000FF}$ ]               | [ Change "no flip" text color with RGBA hexcode ] |
| flipready_color_gaugebar   | [ $\color{#87D687}\texttt{\\#00FF00FF}$ ]               | [ Change gauge bar color with RGBA hexcode ]      |
| flipready_fontsize         | [ 1 - $\color{#87D687}\textsf{20}$ - 100 ]              | [ Change font size ]                              |
| flipready_barlen           | [ 1 - $\color{#87D687}\textsf{20}$ - 100 ]              | [ Change gauge bar length ]                       |
| flipready_barheight        | [ 1 - $\color{#87D687}\textsf{5}$ - 25 ]                | [ Change gauge bar height ]                       |
| flipready_decaydir         | [ $\color{#87D687}\textsf{left}$ \| right ]             | [ Change direction gauge bar decays ]             |
| flipready_positionx        | [ left \| $\color{#87D687}\textsf{middle}$ \| right ]   | [ Change horizontal position ]                    |
| flipready_positiony        | [ $\color{#87D687}\textsf{top}$ \| middle \| bottom ]   | [ Change vertical position ]                      |

**Example:** If I want to make the "flip" text blue, change the font size to 25, and set the horizontal position to the left, I would type into the F6 console:  
`flipready_color_fliptext #0000FFFF`  
`flipready_fontsize 25`  
`flipready_positionx left`  
