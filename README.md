# BFV-Tutorial Cheat

![](https://i.imgur.com/p7K8YwV.jpg)

This is an extremely easy cheat to compile and work, and it's rather simple as well.

Both getting the player array, and the overlay code is in seperate static libraries.  You can read up on the PlayerManager lib [here](https://www.unknowncheats.me/forum/battlefield-v/321181-playermanager-lib.html).

Supports Boxes, Health, Names, Distance, and Snaplines.  The code should be really easy to modify, it's built so future modification and such are easy.

You'll need the [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812) to compile, make sure you link against it.  There's comments in `dllmain.cpp` to make this easy.  

Structure:
	dllmain.cpp 	Contains the entry point, and rendering loop for the ESP.  The bulk of the code is here

	fbsdk.h 		Contains the reversed classes for the game, as well as the static offsets

	Utilities.h 	Has the math functions needed to calculate WorldToScreen and distance over 3 dimensions

	PlayerManager.h 	The header for a static lib to get players

	DX9OV.h 		The header for the static lib for rendering