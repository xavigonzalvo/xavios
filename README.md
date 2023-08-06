# xavios - Xavi Operating system

A toy example of some experiments to have an operating system.

## Install dependencies

    brew install qemu

## Build the docker container

    docker build -t kernel-builder .

## Run commands

    docker run --rm -v "$(pwd)":/kernel_source kernel-builder make mykernel.iso

## References:

- [Bare bones](https://wiki.osdev.org/Bare_Bones)
- [Write Your Own Operating System](www.wyoos.org)
