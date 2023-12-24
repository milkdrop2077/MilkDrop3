# MilkDrop3

- How to download and install : https://youtu.be/OQdZFlOHe5o<br/>
- All the new features explained : https://youtu.be/QBsDXPENe1I<br/>
- How to create a beautiful double-preset : https://youtu.be/AdcUPpp1O0k<br/>

<img align="left" width="300" height="316" src="https://github.com/milkdrop2077/MilkDrop3/raw/main/MilkDrop324a.jpg">
<img  width="300" height="316" src="https://github.com/milkdrop2077/MilkDrop3/raw/main/MilkDrop324b.jpg">

MilkDrop 3.0 is a portable program that supports any audio source (Spotify, YouTube, SoundCloud, Winamp...)<br/>
It is based on BeatDrop from Maxim Volskiy, so it's 100% compatible with any presets created with MilkDrop and projectM.<br/>
MilkDrop3 does everything that MilkDrop2 can do, I will be only listing on this page the additional new features :)<br/><br/>
It supports a new type of double-preset (.milk2 file) mixing 2 presets and displaying them both simultaneously.<br/>
Here are a few double-preset: https://www.youtube.com/watch?v=6UXKyz4nOfI<br/>
You can create new presets by pressing just a few keys, without knowing any lines of code<br/>
(F9 and then press the spacebar to create a new double-preset, that's it).<br/>
Create new colors by pressing just the key 'c' https://twitter.com/MilkDrop2077/status/1614526230929498113<br/>
Also I created a new mode for auto-changing the presets based on beat detection.<br/>
Here is a fast version of that: https://www.youtube.com/watch?v=pSzdThg1iW8<br/><br/>
<img src="https://github.com/milkdrop2077/MilkDrop3/raw/main/MilkDrop3.jpg" width="600" />

- Lots of new key shortcuts, see the whole list above.<br/>
For some letters, the majuscule and minuscule matters.<br/>
As an example, I'm using the lower-case "a" for the classic random mini-mash-up and<br/>
upper-case "A" (shift+a) for another function: back to previous random preset.<br/>

- Support always on top window (F7) and borderless mode.<br/>

- Support multiple monitor stretching (ALT+SHIFT) thanks to milkdropper for the code.<br/>

- Real-time toggling FPS (F2) 60fps, 90fps, 120fps ect... (some presets may not work well on high fps).<br/>

- Real-time toggling seconds between auto transition to next preset (F3) 15sec, 30sec, 45sec ect...<br/>

- Support of displaying 16 shapes simultaneously instead of 4.<br/>
Shapes are now exported with the '.shape' file extension in Milkdrop3\shapes\file.shape<br/>

- Support of displaying 16 waves simultaneously instead of 4.<br/>
Waves are now exported with the '.wave' file extension in Milkdrop3\waves\file.wave<br/>

- 8 new simple waveform (total of 16).<br/>

- The q1-q32 variables have been expanded; the range is now q1-q64.<br/>

- Support going back to previous generated preset when pressing the key 'A' (Shift+a).<br/>
I've waited 20 years for that function!<br/>

- Support deep-mash-up, not just warp and comp mashup, but all 5 bins (z).<br/>
Note: this is quite unstable, some complex presets could crash MilkDrop3. But the results can be really great.<br/>

- Support going back to previous generated preset when pressing the key 'Z' (Shift+z).<br/>

- Support randomize colors function, pressing the (c) key will change all the colors in the comp shader directly in the memory.<br/>
Don't forget to save the preset if you like the new color.<br/>

- Support going back to previous generated color when pressing the key 'C' (Shift+c).<br/>

- Lots of new debug information pressing the letter 'n', very useful if you want to create or mashup a double-preset.<br/>

- Support auto-changing the presets based on beat detection (F8) ex: https://twitter.com/MilkDrop2077/status/1617405613960560641<br/>
hardcut1 : load a new preset if bass > 1.5 with a minimum delay of 0.2 sec<br/>
hardcut2 : load a new preset if treb > 2.9 with a minimum delay of 0.5 sec<br/>
hardcut3 : load a new preset if treb > 2.9 with a minimum delay of 1 sec<br/>
hardcut4 : load a new preset if treb > 2.9 with a minimum delay of 3 sec but if treb > 8 then it will load a new preset<br/>
hardcut5 : load a new preset if treb > 2.9 with a minimum delay of 5 sec<br/>
hardcut6 : load a new preset if bass >1.5 + will load Bass/WHITE.milk if bass > 4.90<br/>

- Lots of new transition effects (when pressing the spacebar), the first old 4 were : "zoom", "side", "plasma", "cercle".<br/>
New ones: "plasma2", "plasma3", "snail", "snail2", "snail3", "triangle", "donuts", "corner", "patches", "checkerboard", "bubbles", "stars", "cisor", "wave",  "curtain", "vertical", "horizontal", "linesvertical", "lineshorizontal".<br/>
And since version 3.21, an extra 7 transitions: "square", "stars2", "clock", "nuclear", "arrow", "cross", "cross2".<br/>
A few example here: https://www.youtube.com/watch?v=WXK-pzCPfYk<br/>

- Improved search function:<br/>
Now we can search for anything in the presets list, not just one letter.<br/>
You can type 'mar' for martin's presets.<br/>
You can even type 'kings' if you search for 'martin - kings cross' for example.<br/>

- Bunches of optimized functions with chatgpt<br/>

- PLAYER CONTROL:<br/>
Pressing CTRL+right will load the next track (on any player, spotify, youtube if you're listening to a playlist of songs...)<br/>
CTRL+left will load the previous track on any player.<br/>
CTRL+up will send a fastforward 5sec command to winamp only.<br/>
CTRL+down will send a fastrewind 5sec command to winamp only.<br/>

 - Pressing the key 'f' will filter the presets : showing (and loading) only normal .milk presets, only double .milk2 presets or ALL of them.<br/>
When pressing the keys A,D,M,Z or F9, it will auto-switch to 'Show only classic-presets' to avoid any mash-up errors.<br/>

- Double-preset (.milk2) https://www.youtube.com/watch?v=n37SbxfLK0k<br/>
Press F9 to enter double-preset mode. Now if you press the spacebar, you'll create a mix of 2 presets.<br/>
You can save them with the key 's' like a normal preset, it will create a new milk2 file.<br/>
Try the different types of blending pattern by pressing the key 'p' ("zoom", "side", "plasma" ect...).<br/>
If you press the key 'P' (shift+p) you can force the blending progress (10%, 20%, 30% ect...).<br/>
Some blending pattern like plasma have a lots of random values,<br/>
you can reload those values to see a different plasma effect by pressing the key 'o'.<br/>
All of these random values are saved in the .milk2 file.<br/>
You can now flip the preset1 and preset2 by pressing the key 'u'.<br/>
Force the blending direction (from the inside or outside) by pressing the key 'U' (shift+u).<br/>

Also:<br/>
- WAVE_SIZE can now have 0-100 value<br/>
- ZOOM_AMOUNT can now have 0-100 value<br/>
- ZOOM_EXPONENT can now have 0-100 value<br/>
- WARP_AMOUNT can now have 0-100 value<br/>
- WARP_SCALE can now have 0-100 value<br/>
- WARP_SPEED can now have 0-100 value<br/>
- Max shape sides increased to 500 points (from 100 originally)<br/>

I've setup up a patreon page, I've spend countless hours on MilkDrop3, if you would like to support this project and future updates:<br/>
https://www.patreon.com/MilkDrop3<br/>
https://www.buymeacoffee.com/MilkDrop2077/<br/>

<img src="https://github.com/milkdrop2077/MilkDrop3/raw/main/MilkDrop32.jpg" width="600" />

# History:

★ <ins>24/12/2023 : Version 3.24</ins><br/>
- You can now inject effects directly in the shaders (toggle with F11 or use the menu).<br/>
- Easy sprites controls: choose the blending mode, opacity, size, position, movement, speed... directly from the menu!<br/>
- Sprites are now staying if you resize the window! This was quite a challenge to code.<br/>
- Drag and Drop added, files supported : milk, milk2, wave, shape, png, jpg, jfif, bmp.<br/>
- Delete mode is finally working perfectly. Press Ctrl+d to activate it, then preset delete to directly delete the current preset.<br/>
- In case of a shader compilation error, MilkDrop3 will automatically update the shader (2>3>4) and try again.<br/>
This should remove the max instructions slots limit error! This mean you'll only see the same error not matter what went wrong.<br/>
If you want to debug a preset, you should disable this option in the MilkDropPanel.<br/>
- By default MilkDrop3 start in 'Sprites' mode, that mean you can direclty call sprites (pressing 00, 01, 02...) from the milk3_img.ini<br/>
If you want to switch to 'Messages' mode, press Shift+k. Check the milk3_img.ini and milk3_msg.ini for more informations.<br/>
Press F1 to see the new keys used for Sprites and Messages.<br/>
I reduced the number of presets to 500, the cream of the cream, what a collection :)<br/><br/>

★ <ins>21/10/2023 : Version 3.23</ins><br/>
- fixed some issues with display DPI scaling.<br/><br/>

★ <ins>11/10/2023 : Version 3.22</ins><br/>
- Better compatibility with Windows 11.<br/>
- Better compatibility for low and ultra high resolution screen.<br/>
The font size will be adjusted if the resolution is lower than 1000 or higher than 2000 vertical pixels.<br/>
- Font size is now adjustable in the MilkDropPanel.
- An external program like Winamp, Spotify, foobar2000, a videogame... or anything you want<br/>
  can be launched when MilkDrop3 is starting or when pressing the key 't' or 'T'.<br/><br/>

★ <ins>03/10/2023 : Version 3.21</ins><br/>
- 100 new presets (for total of +900!) + some new textures.<br/>
- Options saved in the .ini file were completely rewritten.<br/>
- You can now save any options available, save the exact window position, start MilkDrop in fullscreen...<br/>
- New transition effects: square, stars2, clock, nuclear, arrow, cross, cross2.<br/>
The last 2 are some kind of hybrid transition, some parts of the screen are not blending at all :)<br/>
- New algorithm for saving presets: size is much smaller, load time will be a bit faster.<br/>
  Only the wavecode and shapecode enabled will be saved.<br/>
- Values were optimized for each pattern when displaying a double-preset.<br/>
That means the blend percentage when you're pressing the spacebar will be adjusted for each pattern<br/>
so it won't be over or under-blend. For example "triangle" will be set between 0.2 and 0.6,<br/>
but "cross2" between 0.7 to 0.95, because "cross2" will only start to appear after 0.7!
- When pressing 'P' to change the progress, it will increment the actual progress number and not start from 0.<br/>
For example if you're at the blending progress 0.42, it will jump to 0.45 > 0.50 > 0.55...<br/>
- Locked blending patterns / progress / directions are now displayed in red colour on the information screen (n).<br/>
- New  algorithm for changing colors to avoid multiple ret /= float3 in the composite shader.<br/>
Pressing 'c' will search for //MilkDrop3 Color Mode: and replace the line underneath with the ret /= float.<br/>
- New borderless mode, press F7 multiple times to toggle modes.<br/>
"NoBorder + Resize ON" : can resize the window to any size (but can't use the double-click to enter fullscreen).<br/>
"NoBorder + Resize OFF" can't resize, but can use the  double-click to enter fullscreen.<br/>
- Better behaviour when using deepmash : dm menus will disappear when pressing 's' or 'shift+z'.<br/>
- Corrected a few bugs : while saving a preset after using a blocked preset, while renaming some presets, exiting fullscreen, exiting AlwaysOnTop, I fixed some old legacy bugs from loading textures...<br/>
- New menus and new GUI (MilkDropPanel), both are a bit empty at moment, but they will grow over time with lots of new functionalities coming!<br/>
MilkDropPanel is called for the Options/Donate/About box.
- Know bugs that need to be fixed : don't have a preset named 0.milk, it crashes MilkDrop3 when mashing presets.<br/><br/>

★ <ins>20/07/2023 : Version 3.2</ins><br/>
- Double-preset are now displayed correctly when entering/exiting full screen or resizing the window.<br/>
- Live blending update: pressing the keys 'p' and 'P' will display directly pattern or progress, no more backspace/spacebar needed!<br/>
- Reload a double-preset: pressing 'o' will reload a double-preset only with different random values,<br/> 
 very handy to see different plasma blends for example.<br/>
- Flip presets: press 'u' to flip preset 1 and 2 (and keep the exact position).<br/>
- Locked direction: press 'U' to force the direction of the blending,<br/> 
 ex: if you are using the cercle pattern, -1 = from the INSIDE, 1 = from the OUTSIDE.<br/>
- Block a preset: press 'b' to lock a preset, it will always load first when creating a double-preset (F9 and then SPACEBAR)<br/>
- Press 'B' to choose the blending time.<br/>
- When pressing F4, you can now see both preset names if it's a double-preset.<br/>
- New borderless mode with F7.<br/>
- New delete mode: press CTRL+DELETE to start delete mode.<br/>
 Now you can delete any preset you see simply by pressing the DELETE key (no need to load the list and confirmation).<br/>
 Very handy if you want to clean a large number of presets, be careful you can't restore a deleted preset!<br/>
- When pressing the keys A,D,M,Z or F9, it will auto-switch to 'Show only classic-presets' to avoid any mash-up errors.<br/>
- Added the Beginners Guide to MilkDrop Preset Writing in html and doc in the Milkdrop3\docs folder.<br/><br/>

★ <ins>09/04/2023 : Version 3.12 :</ins><br/> 
- 10x faster way to load the presets list, can load 50.000 presets under 1 second!<br/>
- Better way to display preset names (F4) for milk and milk2 files.<br/>
- New plasma3 transition effect (a bit crazier than plasma and plasma2).  
- Corrected a few bugs while loading preset with textures.<br/>
- I removed the V2M player in the installer to avoid any false-positive.<br/><br/>

★ <ins>19/03/2023 : Version 3.1 : stable.</ins><br/><br/>

★ <ins>03/03/2023 : Version 3.0 : alpha, pretty unstable.</ins>
