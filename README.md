# xavios - Xavi Operating system

A toy example of some experiments to have an operating system.

## Install dependencies

    brew install qemu

## Build the docker container

    docker build -t kernel-builder .

    docker build -t kernel-builder . -f Dockerfile.arm64

## Run commands

    docker run --rm -v "$(pwd)":/kernel_source kernel-builder make mykernel.iso

    make ARCH=x86_64

## Test the kernel

As a BIN:

    qemu-system-x86_64 -kernel mykernel.bin

    qemu-system-aarch64 -M virt -cpu cortex-a53 -kernel kernel.bin -nographic -chardev stdio,id=char0,mux=on -serial chardev:char0 -mon chardev=char0

    Ctrl-a c to go to the qemu monitor.

As an ISO:

    qemu-system-x86_64 -cdrom mykernel.iso

## References:

- [Bare bones](https://wiki.osdev.org/Bare_Bones)
- [Write Your Own Operating System](www.wyoos.org)
