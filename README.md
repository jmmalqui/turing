# C Turing Machine
A program that simulates a single tape Turing machine.

## Build 
Make sure to have the Meson build system installed on your system, if not, follow the instructions from https://mesonbuild.com/Quick-guide.html

Once Meson is installed, run the following commands on the root directory of this project:

```bash
setup meson build 
cd build
meson compile 
```

## Run 
This program accepts two arguments as input as follows:
```bash
./turing <Input String> <Instruction File> 
```
 ### Input String
 An string that the machine would use as its tape.
 If the Input String is rejected by the machine, it would output "Reject".
### Instruction File
 This file should contain the instructions that the Turing machine will follow. Each instruction should follow this syntax: 

 ```bash
 <Input State> <Accept String>|<Overwrite Char>,<Instruction> <Output State>
 ```
#### Input State
Executes the instruction if the current state of the machine is the same as this string.
#### Accept String
Checks if the content of the current cell of the tape is inside this string, if not then it would look to another instructions that contains the same Input State.
#### Overwrite Char
Character that will be used when overwriting the current tape cell. If the Overwrite Char is '/' then the machine would not perform overwriting.
#### Instruction
The instruction that the machine will perform: R(Right) L(Left) S(Halt the machine)
#### Output State
Indicates the state in which the machine will be after executing the instruction 

## Examples
A machine that multiplies a number by 10:
```txt
q0 123456789|/,R q1 
q0 0|/,S qS
q1 0123456789|/,R q1
q1 _|0,S qS 
```

 A machine that multiplies a number by 100 and then adds 10:
 ```txt
 q0 123456789|/,R q1
q1 0123456789|/,R q1 
q1 _|1,R q2
q2 _|0,S qS
 ```
 