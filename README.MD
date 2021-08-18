# CHIP-8 Emulator
**Written in C++ and uses the SDL2 library.**

## What is CHIP-8?
CHIP-8 is an interpreted programming language that runs in the form of a virtual machine. What I'm attempting to do, is to create an interpreter, or emulator of sorts since the machine has it's own opcodes, registers, memory structure etc.

## Reference
[Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

## Build instructions
### Dependencies
- `g++`
- `cmake`
- `SDL2`


### Build command
#### Configure build
```
$ mkdir build
$ cd build
$ cmake ../
```
#### Build debug version
```
$ cmake --build ./build --config Debug --target all
```

### Running the app 
```
$ ./build/CHIP8Emu
```

## Contributing
*Will be added soon*