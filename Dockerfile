# Use a base image with a development environment. For simplicity, let's use Ubuntu.
FROM ubuntu:latest

# Install required packages
RUN apt-get update && apt-get install -y \
  clang \
  g++ \
  make \
  grub-pc-bin \
  xorriso \
  nasm \
  && rm -rf /var/lib/apt/lists/*

# Set working directory in container
WORKDIR /kernel_source

# Copy the kernel source into the container
COPY . /kernel_source/
