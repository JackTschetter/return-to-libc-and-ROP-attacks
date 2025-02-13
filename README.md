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
- [License](#license)
  
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

**gcc-11 -Og -no-pie -fno-stack-protector -Wall -g printf-server.c -o printf-server**

The option -fno-stack-protector disables stack canaries. We disabled that defense because todays attack is still based on overwriting a return address. -Og is a modest optimization level that uses registers and removes unused code. Compared to some other victim programs that we compiled without any optimization, we had to add some extra code in this program to make sure the vulnerable operations were compiled in the way we wanted and not optimized away.<br>

${\color{red}WARNING}$ For teaching purposes this is deliberately vulnerable code that does not follow software engineering best practices. This command compiles the binary in a way that intentionally disables various defense mechanisms against certain kinds of attacks.

---

## Lab Walkthrough

### Reconnaissance of the victim

The victim in this lab is a simple server named printf-server. 

### Non-ASLR return-to-libc attack

### Adding a Bit of ROP

### Bypassing ASLR

---

## Usage

I developed this lab for teaching the class Designing and Developing Secure Software (CSCI 4271W) at UMN Twin Cities. We provided students in the course the same source code, and pre compiled binary available for download from this repo. 

In addition to leading instruction during weekly lab sections, I held regular office hours and made myself available by appointment to help students.

---

## Contact

Contact me anytime! Day or night. My email is jackrtschetter@gmail.com, and phone number is 612-380-1832. Texting with a short introduction is the most efficient way to get a hold of me. I will respond ASAP.<br>

Assuming you are not a current or prospective student in 4271W, I would be more then happy to go over the answers to this lab.<br>

More generally I would be happy to discuss in (much) greater technical detail the C programming language, and the low level analysis of binary code.<br>

---

## Acknowledgements

I developed and taught from this lab during my time as an undergraduate Teaching Assistant (TA) for the class Designing and Developing Secure Software at UMN Twin Cities. At that time I was the youngest person ever in the history of the University of Minnesota to teach this class. All the previous TA's were grad students. Professor Stephen McCamant was so much more then my boss/teacher. He was also my friend, life coach, research partner and mentor.<br>

To this day Stephen McCamant was the single smartest human being that I have ever had the privilege of working and learning with. He spent hours patiently guiding me, and answering all of my questions. To my own shame Professor Stephen quickly found and corrected several mistakes I had made. In doing this he was quick to disabuse me of any hubris I might have had about my own abilities. Nevertheless I would not be one quarter of the computer scientist I am today without Professor Stephen.
