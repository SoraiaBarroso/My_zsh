# Welcome to My Zsh
***

## Task
Create a shell alternative for commands execution. It runs binarys and 8 shell built-ins which are EXIT, CD, ECHO, PWD, WHICH, ENV, SETENV, UNSETENV.
This built-ins are implemented by us while the binarys are executed in the program using exceve(); 

## Description
The program runs in a loop waiting for the input until termination.
Each input is then gets tokenized - split into parts for further processing and classificated for handling error commands.
Based on the first token ("command") either a built-in function or an external executable is called.

## Installation
No installation is required.
Build with `make`.

## Usage
Execute it like this `./my_zsh`

### The Core Team

Soraia Lima Cid &&
Michael Mullins

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
