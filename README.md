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
### Instruction File
 This file should contain the instructions that the Turing machine will follow. Each instruction should follow this syntax: 

 ```bash
 <Input State> <Accept String>|<Overwrite Char>,<Instruction> <Output State>
 ```

 An example would be as follows

 A machine that multiplies a number by 10:
 ```txt
 q0 123|,R q1 
 q1 _|0,S qS 
 ```
 

 ### Input String
 An string that the machine would use as its tape
 If the Input String is rejected by the machine, it would output "Reject"
