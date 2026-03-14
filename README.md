# Linux Character Device Driver

This repository contains simple Linux kernel driver examples developed for learning and practicing Linux device driver development.

## Project Description

This project includes:

- A basic **character device driver**
- A **multiple character devices driver**
- **User-space applications** to interact with the device files
- A **proc entry interface** that can be accessed from user space

The project is mainly created to understand how Linux kernel modules work, how device files are handled, and how user-space applications communicate with kernel-space drivers.

## Files in this Repository

- `char_driver.c` – basic character device driver
- `mul_dev.c` – multiple character devices driver
- `char_user.c` – user-space program for testing character device operations
- `proc_user.c` – user-space program to access the proc interface
- `proc.c` – proc file related driver code
- `Makefile` – used to build the kernel modules

## Features

- Registers a Linux character device
- Demonstrates multiple character device creation
- Supports basic file operations:
  - open
  - read
  - write
  - release
- Shows kernel-space and user-space communication
- Includes a proc interface that can be accessed from user space

The output messages from the driver can be seen using `dmesg` after loading or unloading the module with `insmod` and `rmmod`
