# Day 1: Historian Hysteria

- Language of choice: Aarch64 Assembly.
- Difficulty: Yes. (I'm a C# developer...)

<!-- omit from toc -->
## Contents

- [Requirements](#requirements)
  - [Linux](#linux)
  - [QEMU User Space Emulator](#qemu-user-space-emulator)
  - [Assembler and Linker](#assembler-and-linker)
  - [**(Optional)** Debugger](#optional-debugger)
  - [Just](#just)
- [Getting Started](#getting-started)
- [Debugging](#debugging)
  - [Viewing System Call Logs (QEMU)](#viewing-system-call-logs-qemu)
  - [Building with Debug Information](#building-with-debug-information)


## Requirements

I'm not sure why you'd want to, but if you do want to run this you'll need a
couple of things.

### Linux

This will only work if you have Linux of some description. Because it's written
in aarch64 assembly, you'll either need native aarch64 hardware or to emulate
it. A Raspberry Pi would be great here (Raspberry Pi Zero 2 / Raspberry Pi 3
onwards are 64-bit), but I've chosen to use [QEMU](https://www.qemu.org/) to
emulate it to make it more accessible.

The reason that Linux is a requirement, is that I've chosen to use
[User Mode Emulation](https://www.qemu.org/docs/master/user/index.html#user-mode-emulation),
which is only supported on Linux.

Don't fret though! On Windows you can easily install
[QEMU](https://www.qemu.org/) in
[Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/install).
I've chosen to use
[Debian in WSL](https://apps.microsoft.com/detail/9msvkqc78pk6) for this
challenge.

If you've freshly installed Debian in WSL, it might be worth doing an update
before moving on:

```zsh
sudo apt update && sudo apt dist-upgrade
```

### QEMU User Space Emulator

If you're not on native aarch64 hardware, install the
[QEMU User Space Emulator](https://www.qemu.org/docs/master/user/main.html)
using your package manager. For example, on Debian:

```zsh
sudo apt install qemu-user
```

### Assembler and Linker

If you're on native aarch64 hardware, the native `binutils` will be fine. If
you're not, you'll need to install `binutils-aarch64-linux-gnu`. This can be
done in Debian using `apt`:

```zsh
sudo apt install binutils-aarch64-linux-gnu
```

### **(Optional)** Debugger

If you need to debug. you'll need either `gdb` on native aarch64 hardware, or
`gdb-multiarch` if you're using [QEMU](https://www.qemu.org/). This can be
installed as follows:

```zsh
sudo apt install gdb-multiarch
```

### Just

Instead of using a Makefile, I've decided to try out
[just](https://github.com/casey/just) on the recommendation of
[Dom](../../dom/). Check the GitHub page for install instructions on your OS.

## Getting Started

The [justfile](./justfile) contains the basic recipes to build the main
executable. For a complete list of recipes, just type `just`!

```zsh
 ~#@❯ just
just --list
Available recipes:
    build       # (Default) build without debugging information.
    build_debug # Build with debugging information, and launches gdb after building.
    clean       # Clean up any generated files.
```

To build the program, just run `just build`. You should then be able to run the
binary using `qemu-aarch64 main`.

## Debugging

This is more for my own notes... Maybe I'll re-discover this at some point and
need it...

### Viewing System Call Logs (QEMU)

Before jumping in with `gdb`, it is sometimes helpful to log the system calls
from [QEMU](https://www.qemu.org/) to see if it helps pinpoint a problem. To do
this, simply [build](#getting-started) the binary as normal, but run it with
`qemu-aarch64 -strace main`.

### Building with Debug Information

To build the object files with debug information, use the `just build_debug`
recipe. This will include the debug information in the object files, and also
ensure that the binary is up to date.

In another shell instance, you can then launch the binary in
[QEMU](https://www.qemu.org/), and get it to wait for `gdb` to connect with
`qemu-aarch64 -g 2001 main`.

Since the `just build_debug` recipe automatically launches `gdb`, you should
then be able to enter `target remote localhost:2001` into the `gdb` window to
connect.