# CS475: Computer Graphics
## Assignment 3: Rendering and Animation

### Members
* Kumar Saurav [160050057]
* Yash Shah [160050002]

### Honor Code
"We pledge on our honour that we have not given or received any unauthorized assistance on this assignment or any previous task. The assignment is our own work and we have not copied it from anywhere."

### Published Video
https://youtu.be/s5cI0XVszPQ

### Description
The program runs in 4 different modes - `INSPECT`, `CAMERA`, `RECORD` and `PLAYBACK`. Their functionality is as follows (as is evident from their names)
* **INSPECT** - In this mode, each model can be inspected individually. Joint angles can be altered, root positions can be updated, camera can be moved around etc. Basically, this mode serves as a *playground*.
* **CAMERA** - In this mode, only the camera can be translated and rotated. Other models cannot be moved. It also allows marking and deletion of control points in space for the camera's *Bezier curve* trajectory.
* **RECORD** - In this mode, keyframes can be generated and saved to a file. Model movement and joint angle alteration is permitted. Change in lighting conditions is also allowed.
* **PLAYBACK** - This mode is solely for viewing the generated animation. It reads keyframes from the specified file, interpolates them and plays the *movie*. It starts with the motion of camera along the Bezier curve, followed by character animation. Snapshots of each frame are saved on disk as images. This mode is further divided into `STOP`, `INIT`, `PLAY`, `PAUSE` and `RESUME` stages.

### Playing the animation
Run `make` and then execute `a3`. Press `M` to change to `CAMERA` mode. Press `L` to load the camera trajectory. Press `M` again till the `PLAYBACK` mode is reached. Press `S` if you want to save the animation snapshots (ensure that `snaps/` directory exists). Press `P` to play the animation.

For other tasks, refer the section on controls and callbacks.

### Controls/Callbacks
For modifying any DOF, follow these steps: select the character (_root_) using `R`, then select the joint which needs to be modified using `J`, and finally, rotate the joint along any axis as desired using `W/S/A/D/Q/E`. Note that a ball-and-socket joint will rotate about all three axes, while a hinge joint will rotate only about the z axis.

Also, on pressing `R` or `J`, there will be a prompt at the terminal where you will have to enter your selection. A list of possible options (root nodes and joints respectively) will also be shown on the terminal.

*Note (1):* Rotation about the centroid is possible in a similar manner (on pressing `J`, you will find that the _Centroid_ appears as an option).

*Note (2):* The camera path and keyframes are saved in `camera_path.txt` and `keyframes.txt` respectively, by default. The animation snapshots are stored in `snaps/` folder sequentially with names `snap_<t>.tga` (here _t_ is the counter).
```
Controls:

GENERAL

H                          : Display this help
Esc                        : Close window
M                          : Switch modes in cyclic manner [INSPECT|CAMERA|RECORD|PLAYBACK]

[ INSPECT ]

R                          : Select character
J                          : Select joint
Up/Down/Left/Right         : Rotate camera about Y[-/+] and Z[-/+] axes
./,                        : Rotate camera about X[-/+] axis
Shift + Up/Down/Left/Right : Translate camera along Y[-/+] and Z[-/+] axes
Shift + ./,                : Translate camera along X[-/+] axis
W/S/D/A/E/Q                : Rotate selected joint about Z[+/-], X[+/-] and Y[+/-] axes
Shift + W/S/D/A/E/Q        : Translate selected joint along Z[+/-], X[+/-] and Y[+/-] axes
+/-                        : Zoom the camera in/out
L                          : Toggle wall-mounted light
Shift + L                  : Toggle lamp light

[ CAMERA ]

Up/Down/Left/Right         : Rotate camera about Y[-/+] and Z[-/+] axes
,/.                        : Rotate camera about X[-/+] axis
+/-                        : Zoom the camera in/out
Scroll up/down             : Zoom the camera in/out
Left click                 : Mark control point for Bezier curve
Right click                : Delete last marked point
S                          : Save marked points to file
L                          : Load marked points from file
R                          : Toggle rendering of Bezier curve for current control points

[ RECORD ]

R                          : Select character
J                          : Select joint
Up/Down/Left/Right         : Rotate camera about Y[-/+] and Z[-/+] axes
./,                        : Rotate camera about X[-/+] axis
Shift + Up/Down/Left/Right : Translate camera along Y[-/+] and Z[-/+] axes
Shift + ./,                : Translate camera along X[-/+] axis
W/S/D/A/E/Q                : Rotate selected joint about Z[+/-], X[+/-] and Y[+/-] axes
Shift + W/S/D/A/E/Q        : Translate selected joint along Z[+/-], X[+/-] and Y[+/-] axes
+/-                        : Zoom the camera in/out
C                          : Save current keyframe (state) to file
L                          : Toggle wall-mounted light
Shift + L                  : Toggle lamp light

[ PLAYBACK ]

P                          : Toggle between playing/pausing the animation
S                          : Toggle saving the animation snaps to disk
```

### Citations
* DreamWorks Animation (for the inspiration behind the characters, _Po_ and _Mantis_)
* Music score from https://www.youtube.com/watch?v=LMQ8sSvqphg
