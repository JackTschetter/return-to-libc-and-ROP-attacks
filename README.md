# Return to libc and ROP attacks

![License](https://img.shields.io/badge/license-MIT-blue.svg)  
![Version](https://img.shields.io/badge/version-1.0.0-green.svg)

## Table of Contents

- [About](#about)
- [Tools Used](#tools-used)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
    - [Disclaimer](#disclaimer)
- [Lab](#lab)
    - [Reconnaissance of the Victim](#reconnaissance-of-the-victim)
    - [Non-ASLR Return to libc Attack](#non-aslr-return-to-libc-attack)
    - [Adding a Bit of ROP](#adding-a-bit-of-rop)
    - [Bypassing ASLR](#bypassing-aslr)
- [License](#license)
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
- **(Optional) attack-template.c**. Template code to get students started on launching the third attack which was bypassing ASLR. Technically this is not needed, but having the template makes life easier since it already provides the code to start up the vulnerable server as a child process, and to send commands and receive data back from it. All that is left is to put the pieces together by implementing the function attack.
- **(Optional) GNU Project Debugger (GDB)**. I say optional, but highly recommended.
- **(Optional) objdump**. I say optional, but highly recommended.

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/JackTschetter/return-to-libc-and-ROP-attacks
   cd return-to-libc-and-ROP-attacks/src

This repo provides a precompiled victim binary. This victim binary was compiled from the bcimgview.c source on a Linux x86-64 machine, so it should work on most recent Linux systems. I just recently was able to get the program to work, with slight modifications, on my brand new MacBook air which has an ARM based M3/Apple Silicon chip. This was A LOT of extra work, so please stick with the provided binary on Ubuntu Linux 22.04 for best results.

If their are any die hard Mac lovers such as myself, feel free to contact me and I can walk you through how to get this running on your device and OS.

For anyone interested the command used to compile the victim binary on Ubuntu Linux 22.04 was<br>

**gcc -no-pie -fno-stack-protector -Og -g -Wall \
    $(pkg-config --cflags gtk+-3.0) \
    bcimgview.c -o bcimgview \
    -lgtk-3 -lgobject-2.0 -lglib-2.0 -lgdk_pixbuf-2.0 -lm**

${\color{red}WARNING}$ This is intentionally buggy/vulnerable code created for the purpose of teaching a class on software security. This command compiles the binary in a way that intentionally disables various defense mechanisms against certain kinds of attacks.

---

## Usage

This project was created for the purposes of teaching the class Designing and Developing Secure Software (CSCI 4271W) at UMN Twin Cities. For teaching purposes we provided students in the course the same source code, sample images, and pre compiled binary available for download from this repo. 

The source code is deliberately buggy and does not follow software engineering best practices. In particular their were 4 vulnerabilities which were deliberately planted in the BCIMGVIEW.c source. The goal of this assignment was for students to discover, exploit and mitigate at least 3/4 of the planted vulnerabilities. 

To help with this I regularly held labs, and office hours to assist the students. The goal of this project was to teach core competencies such as code auditing, strategies to audit large code bases, and heuristics to understand program execution logic/control flow. Along with professor I also taught students in the usage of a debugger (GDB/LLVM), and a fuzzer (AFL++). Our expectation was that students would require usage of the debugger and or fuzzer to efficiently uncover the vulnerabilities.

### Disclaimer

To my knowledge this project or some close derivative is still being used for teaching. After careful consideration, and out of fairness to current, former, and future students I will not be making details of the vulnerabilities, attacks or mitigations public.<br>

In the case that you are not a current or propsective student, and would like to understand what the vulnerabilites were, how to find them, or how to mitigate them, reach out to me (contact below), and I would be more then happy to make some time on my calendar and we could go over these details and more in some detail.

## Contact

Contact me anytime! Day or night. My email is jackrtschetter@gmail.com, and phone number is 612-380-1832. Texting with a short introduction is the most efficient way to get a hold of me. I will respond ASAP.<br>

I would be happy to discuss in (much) greater technical detail the C programming language, and the low level analysis of binary code.<br>

I would also be happy to instruct in the various code auditing, debugging, and fuzzing techniques in which I guided my students to efficiently uncover and attack the vulnerabilities.

## Acknowledgements

I developed and taught from this lab during my time as an undergraduate Teaching Assistant (TA) for the class Designing and Developing Secure Software at UMN Twin Cities. At that time I was the youngest person ever in the history of the University of Minnesota to teach this class. All the previous TA's were grad students. Professor Stephen McCamant was so much more then my boss/teacher. He was also my friend, life coach, research partner and mentor.<br>

To this day Stephen McCamant was the single smartest human being that I have ever had the privilege of working and learning with. He spent hours patiently guiding me, and answering all of my questions. To my own shame Professor Stephen quickly found and corrected several mistakes I had made. In doing this he was quick to disabuse me of any hubris I might have had about my own abilities. Nevertheless I would not be one quarter of the computer scientist I am today without Professor Stephen.
