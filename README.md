# Linux Character Device Driver

## Overview
This repository contains Linux kernel driver examples written in C for learning and practicing Linux device driver development. The project includes a basic character device driver, multiple character device handling, a "/proc" interface, and user-space programs to test device interaction.

## Features
- Implementation of a basic Linux character device driver
- Support for multiple character devices
- User-space applications to interact with device files
- "/proc" interface for communication between kernel space and user space
- Demonstration of file operations such as open, read, write, and release
- Kernel module loading and unloading using `insmod` and `rmmod`
- Verification of driver messages using "dmesg"

## Files
- `char_driver.c` – basic character device driver
- `mul_dev.c` – multiple character devices driver
- `char_user.c` – user-space application for testing character device operations
- `proc_user.c` – user-space application for testing the `/proc` interface
- `proc.c` – `/proc` interface related code
- `Makefile` – used to build the kernel modules

## Build
To compile the kernel modules: make
To clean generated files: make clean

Insert the kernel module: sudo insmod <module_name>.ko

Remove the kernel module: sudo rmmod <module_name>

Check kernel log messages: dmesg

Test:
After loading the module, run the user-space applications to test communication with the driver.

Example: gcc char_user.c -o char_user
./char_user

For the /proc interface: gcc proc_user.c -o proc_user
./proc_user

Driver output and debug messages can be verified using: dmesg | tail

Learning Outcomes
Through this project, I learned:
Basics of Linux kernel module programming
Character device driver implementation in C
Handling multiple devices in a Linux driver
Communication between kernel space and user space
Using the /proc filesystem for interaction
Building, inserting, removing, and testing Linux kernel modules
Debugging driver behavior using dmesg
