# CS475: Computer Graphics
## Assignment 2: Hierarchical Modelling

### Members
* Kumar Saurav [160050057]
* Yash Shah [160050002]

### Honor Code
"We pledge on our honour that we have not given or received any unauthorized assistance on this assignment or any previous task. The assignment is our own work and we have not copied it from anywhere."

### Callbacks
For modifying any DOF, follow these steps: select the character (_root_) using `R`, then select the joint which needs to be modified using `J`, and finally, rotate the joint along any axis as desired using `W/S/A/D/Q/E`. Note that a ball-and-socket joint will rotate about all three axes, while a hinge joint will rotate only about the z axis.

Also, on pressing `R` or `J`, there will be a prompt at the terminal where you will have to enter your selection. A list of possible options (root nodes and joints respectively) will also be shown on the terminal.

*Note:* Rotation about the centroid is possible in a similar manner (on pressing `J`, you will find that the _Centroid_ appears as an option).
```
Controls:
H                          : Display this help
P                          : Change perspective mode
R                          : Select character
J                          : Select joint
Up/Down/Left/Right         : Rotate camera about +X/-X/+Y/-Y axes
Scroll left/right          : Rotate camera about -Z/+Z axis
Shift + Up/Down/Left/Right : Translate camera about +X/-X/+Y/-Y axes
Scroll up/down             : Translate camera about +Z/-Z axis
W/S/D/A/E/Q                : Rotate selected joint about +Z/-Z/+X/-X/+Y/-Y axes
Shift + W/S/D/A/E/Q        : Translate selected joint about +Z/-Z/+X/-X/+Y/-Y axes
+/-                        : Zoom the camera in/out
```

### Citations
* DreamWorks Animation (for the inspiration behind the characters, _Po_ and _Mantis_)