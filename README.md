# Return to libc and ROP attacks

![License](https://img.shields.io/badge/license-MIT-blue.svg)  
![Version](https://img.shields.io/badge/version-1.0.0-green.svg)

## Table of Contents

- [About](#about)
- [Tools Used](#tools-used)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Lab](#lab-walkthrough)
    - [Reconnaissance of the Victim](#reconnaissance-of-the-victim)
    - [Non-ASLR Return to libc Attack](#non-aslr-return-to-libc-attack)
    - [Adding a Bit of ROP](#adding-a-bit-of-rop)
    - [Bypassing ASLR](#bypassing-aslr)
- [Usage](#usage)
    - [Disclaimer](#disclaimer)
- [Contact](#contact)
- [Acknowledgments](#acknowledgments)
  
---

## About

In this lab students pretended to be the attacker. During the teaching of this course I didn't go in too much depth about defenses, and counter attack techniques. This lab went one step further then previous labs and projects by letting students create a more sophisticated attack that works even in the presence of W xor X and ASLR.

---

## Tools Used

- **Operating System**: x86 Linux. Specifically all students were using our lab machines which come pre installed with Ubuntu 22.04. This is important because this lab is all about low level attacks. Differences in how the victim program is compiled will affect what attacks are possible.
- **Programming Language(s)**: C, shell
- **Tools**: GNU Project Debugger (GDB), objdump. Neither of these was strictly required, but I found they were very useful to my students.

---

## Getting Started

### Prerequisites

- **Ubuntu 22.04**. The rest of these instructions assume this is what you are using. For instance even if you are using an older Ubuntu 20.04 machine these attacks might not work as expected.
- **printf-server.c**. The victim program for this labs attack.
- **C**. The entire code base for this lab was written using C programming language.
- **(Optional) printf-server**. We provided this precompiled binary. The source file printf-server.c was provided, so students can also compile directly. More can go wrong with that approach since this is low level, and various flags were passed in the compilation.
- **(Optional) attack.c**. Template code to get students started on launching the third attack which was bypassing ASLR. Technically this is not needed, but having the template makes life easier since it already provides the code to start up the vulnerable server as a child process, and to send commands and receive data back from it. All that is left is to put the pieces together by implementing the function attack.
- **(Optional) GNU Project Debugger (GDB)**. I say optional, but highly recommended.
- **(Optional) objdump**. I say optional, but highly recommended.

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/JackTschetter/return-to-libc-and-ROP-attacks
   cd return-to-libc-and-ROP-attacks/src

This repo contains the code for this weeks victim as both a .c source file, and a precompiled binary. It also contains the files attack-template.c, and attack.c useful for constructing attacks in the final part. 

It is strongly recommended to use the precompiled victim. For anyone interested the command used to compile the victim binary on Ubuntu Linux 22.04 was<br>

   ```bash
      gcc-11 -Og -no-pie -fno-stack-protector -Wall -g printf-server.c -o printf-server
   ```

The option ```-fno-stack-protector``` disables stack canaries. We disabled that defense because todays attack is still based on overwriting a return address. ```-Og ``` is a modest optimization level that uses registers and removes unused code. Compared to some other victim programs that we compiled without any optimization, we had to add some extra code in this program to make sure the vulnerable operations were compiled in the way we wanted and not optimized away.<br>

${\color{red}WARNING}$ This is intentionally vulnerable low level code and source code that deliberately ignores software engineering best practices. These files were created for the sole purpose of teaching a class on Designing and Developing Secure Software. The command used to compile the binaries does so in a way that intentionally disables various defense mechanisms against certain kinds of attacks. Exercise enhanced caution when downloading and using the provided code.

---

## Lab Walkthrough

### Reconnaissance of the victim

The victim in this lab is a simple server named printf-server. As the name suggests it provides the service of formatting integers using the C library printf function.<br>

As an instructor the first thing I would encourage my students to do is to look over the source code for the program and try giving it some benign commands. Each command is a single line starting
with a capital letter. The most important commands for the basic functionality are F to set a ```printf ``` format, N to set a number, and P to print it.<br>

The command O has a buffer overflow. If given a long line starting with O the program will crash. As I did for previous programs vulnerable
to a buffer overflow, I encouraged students to use commands inside GDB to figure out where the return address of the ```overflow ``` function is stored relative to the overflowed buffer ```buf ```.<br>

If the ```printf``` functionality didn't give it away this program also has a format string vulnerability. However for the purpose of this lab I only taught on information disclosure, not with usage of ```%n```.<br>

Recall that Linux C binaries are usually dynamically linked with a system library, conventionally called the C library or ```libc```, which implements standard library functions, system calls, and other commonly used functions. This lab works closely with this library. One can list the libraries that a program dynamically links with using the command ```ldd```, and then use commands like ```objdump``` or ```nm``` on it. The ```ldd``` command lists the shared libraries that a program uses; for each one it prints the full path to the library and its base address.<br>

  ```bash
  ldd ./printf-server
  objdump -d /lib/x86_64-linux-gnu/libc.so.6 | less
  ```

### Non-ASLR return-to-libc attack

For the first step of attack development I had students focus on building an attack that works when ASLR is disabled. For this part of the lab use ```setarch -R``` when you run the victim or for your whole shell.<br>

Since the W xor X protection is enabled, we can't inject any shellcode in this program. We need to achieve the attacker's functionality using code already available in the program's address space. Specifically I instructed my students to do what is called a return to libc attack. A simple version of ROP where you call an entire function from the standard library. In particular we will take the classic choice of ```system``` as the library function to call since it already has the functionality of starting an external program. Also classically a convenient program for an attack to start is a shell, and the library includes a string containing the path to the standard shell ```/bin/sh``` since it is already a part of the implementation of ```system```. The functionality we are injecting is going to be like the C code

  ```bash
    system("/bin/sh"); exit(anything);
  ```

The call to ```exit``` is not strictly needed for the attack. For teaching purposes, it is just convienient to terminate the attack to better distinguish between things going wrong before the attack and after. We don't really care about the value of the argument to ```exit```, but we do care about the argument to ```system```.<br>

The pieces of code and data we need for the attack are all kept in the library that is called ```libc``` on Unix systems. Even though we have randomization turned off for now, the OS still loads the library at an address of the OS's choosing, so you'll need to keep in mind how the addresses are changed based on where the library is loaded; Once we turn on ASLR the locations will also be different every time the program runs.<br>

As a preliminary step I would have students try making a return to ```libc``` attack that just calls ```exit```. This is particularly easy because it does not require setting up the argument. Basically the overflow just needs to overwrite the return address of overflow to be the entry point of ```exit```. To see how the address mapping happens, try two ways of finding the location of ```exit```. First, run the server under GDB, and after the program is running and stopped at a breakpoint, look at the results of ```p exit```. Second, look at the output of the this command (based on the C library location we found earlier)

  ```bash
    ldd ./printf-server
    nm -D /lib/x86_64-linux-gnu/libc.so.6 | grep ' exit'
  ```

The ```nm``` command prints symbol table information about an executable or library (here with -D to look at the dynamic symbols of a shared library). Because shared libraries are designed from the beginning to be loaded at different addresses, what's stored in the binary and printed by ```nm``` is just a relative address. You should be able to see that the sum of these two numbers matches the address you saw from GDB.<br>

Verify that you can overflow the buffer and cause ```exit``` to be executed. Note that if exit runs correctly, the program will stop without crashing or printing anything.

### Adding a Bit of ROP

The classic version of return to ```libc``` also put the arguments to the library function in the stack overflow, because that was the calling convention for Linux/x86-32. For our attack against a 64-bit binary, that will not work because the argument to ```system``` needs to be supplied in the register %rdi.<br>

For this part of the attack we need a very simple version of the idea of a gadget from ROP: we need to find code that loads a value from the stack into %rdi and then returns. That will be the first "function" that the attack returns to, and then when that function returns it will continue with other return addresses on the stack. As the easiest version of this task, you'll see there's a function named ```useful_gadget``` in the victim program
that is useful for this purpose. If you're feeling more ambitious, this gadget also exists in the C library, though there's an extra trick needed to find it there.<br>

The two other obvious pieces you need in your attack are the address of the ```system``` function and the address of a copy of the string ```/bin/sh```. You can find the address of ```system``` just like we did for ```exit``` before. The strings program can be used to find printable strings in a binary, in particular
try this command

  ```bash
      strings -tx -a /lib/x86_64-linux-gnu/libc.so.6 | fgrep /bin/sh
  ```

One other trick is related to the fact that some x86-64 code depends on the stack pointer being 16-byte aligned, and not just 8-byte aligned. Some library functions will crash if you call them the stack location at an odd multiple of 8. To fix this you can just pad your return oriented program with the address of a gadget that doesn't do anything at all, and just returns right away. This kind of gadget is even easier to find.<br>

The full overflow is going to have a lot of binary data and null bytes in it, so it will not be convenient to type it in on the terminal. Instead I encourage my students to use something like a scripting language with a pack command, or write the attack in a hexadecimal editor.<br>

Even though it's binary data, ```printf-server``` still terminates each command with a newline, so be sure it ends with that. After you've started a shell, you'll want to give it another command, but if you're redirecting the input from a file or a pipe you won't automatically get a prompt back. For a simple test you can just put the
command you want to give to the shell after the command to ```printf-server```, since ```printf-server``` doesn't read ahead. For instance ```xcalc``` and a newline to start a calculator.

### Bypassing ASLR

Let's generalize the attack so it works even in the presence of ASLR. All the libc-based addresses you're using will now change every time you run the victim, so you can not just hard code them. Conveniently the program also has a format string vulnerability, so you can get information that will let you figure out the library address from the program.<br>

Notice that the main function has a function pointer variable that is initialized to the location of the ```printf``` function in the C library.<br>

Using a format string with %lx to figure out how to leak that value. Notice that even under ASLR, the starting address of the C library always ends in 0x000, which makes it easier to see which address is which.<br>

First try doing the location sensitive attack in a manual way by recomputing the addresses as needed; you may still need a small script or program to supply the attack at the right time. A more sophisticated approach is to embed your attack into a program itself that interacts with the victim program.<br>

To this end I provided a template for my students. Part of this is available in this repository under src as attack.c<br>

The template already provides the code to start up the vulnerable server as a child process, and to send commands and receive data back from it.<br>

However you still need to put the pieces together by actually implementing the function attack.

---

## Usage

I developed this lab for teaching the class Designing and Developing Secure Software (CSCI 4271W) at UMN Twin Cities. We provided students in the course the same source code, and pre compiled binary available for download from this repo. 

In addition to leading instruction during weekly lab sections, I held regular office hours and made myself available by appointment to help students.

### Disclaimer

To my knowledge this project or some close derivative is still being used for teaching. After careful consideration, and out of fairness to current, former, and future students I will not be making detailed answers to this lab public.

In the case that you are not a current or propsective student in 4271W, and would like to go over the answers to this weeks lab questions, I would be more then happy to make some time on my calendar.

---

## Contact

Contact me anytime! Day or night. My email is jackrtschetter@gmail.com, and phone number is 612-380-1832. Texting with a short introduction is the most efficient way to get a hold of me. I will respond ASAP.<br>

Assuming you are not a current or prospective student in 4271W, I would be more then happy to go over detailed answers to this lab.<br>

More generally I would be happy to discuss in (much) greater technical detail the C programming language, and the low level analysis of binary code.<br>

---

## Acknowledgements

I developed and taught from this lab during my time as an undergraduate Teaching Assistant (TA) for the class Designing and Developing Secure Software at UMN Twin Cities. At that time I was the youngest person ever in the history of the University of Minnesota to teach this class. All the previous TA's were grad students. Professor Stephen McCamant was so much more then my boss/teacher. He was also my friend, life coach, research partner and mentor.<br>

To this day Stephen McCamant was the single smartest human being that I have ever had the privilege of working and learning with. He spent hours patiently guiding me, and answering all of my questions. To my own shame Professor Stephen quickly found and corrected several mistakes I had made. In doing this he was quick to disabuse me of any hubris I might have had about my own abilities. Nevertheless I would not be one quarter of the computer scientist I am today without Professor Stephen.
