# Starfield CPP

The "Starfield CPP" is a simple Windows SDL graphics demo, created as a boilerplate to experiment and port to other languages. It simulates the experience of flying through the stars in a pseudo 3D space.
The difference between this program and the [starfield_esp8266_demo](https://github.com/plrang/starfield_esp8266_demo) is, here is the starfield rotation added and background fade.
Tested with SDL2-2.0.4.

**COMPILATION**

g++ -o starfieldCPP.exe starfieldCPP.cpp -I C:\SDL2\SDL2-2.0.4\x86_64-w64-mingw32\include\SDL2 -L C:\SDL2\SDL2-2.0.4\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 -Wall -mwindows
