# 2D-Heat-Transfer-Sim
Simulates heat transfer in a 2D plate by solving the heat equation using the finite difference method.  
Currently works best when surface is composed of a uniform material.  
The sim is also only stable within a narrow range of parameters (displayed in GUI).

Implemented in C++.  
Requires SFML for graphics.

Controls:
Left Mouse Button - Change temperature/material of nodes clicked  
SPACE - Toggle start/pause sim  
C - Toggle temperature color mapping (Color/Greyscale)  
Up/Down Key - Increase/Decrease brush size  
MouseWheel - Increase/Decrease brush temperature  
1- Switch brush material to PYRITE  
2- Switch brush material to ALUMINUM  
