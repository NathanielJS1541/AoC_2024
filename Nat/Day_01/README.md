# Day 1: Historian Hysteria

- Language of choice: AArch64 Assembly.
- Difficulty: Yes. (I'm a C# developer...)

<!-- omit from toc -->
## Contents

- [Requirements](#requirements)
  - [Input Data](#input-data)
  - [Linux](#linux)
  - [QEMU User Space Emulator](#qemu-user-space-emulator)
  - [Assembler and Linker](#assembler-and-linker)
  - [**(Optional)** Debugger](#optional-debugger)
  - [Just](#just)
- [Getting Started](#getting-started)
- [Debugging](#debugging)
  - [Checking Return Codes](#checking-return-codes)
  - [Viewing System Call Logs (QEMU)](#viewing-system-call-logs-qemu)
  - [Building with Debug Information](#building-with-debug-information)
  - [Using `gdb` to Display Variables](#using-gdb-to-display-variables)


## Requirements

I'm not sure why you'd want to, but if you do want to run this you'll need a
couple of things.

### Input Data

The input data can be obtained from the
[Day 1 Advent of Code page](https://adventofcode.com/2024/day/1). You'll need to
sign in to get a puzzle input.

The path to the puzzle input is passed in as an argument to the program, so it's
easiest to put it next to `main.s`.

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

If you need to debug, you'll need either `gdb` on native aarch64 hardware, or
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
    build # (Default) build without debugging information.
    clean # Clean up any generated files.
    debug # Build with debugging information, and launches gdb after building.
```

To build the program, just run `just build`. You should then be able to run the
binary using `qemu-aarch64 main`.

For it to run successfully, you'll need to supply the path to the input file
using the `-f` option:

```zsh
qemu-aarch64 main -f input.txt
```

You can also view the program usage information by supplying only the `-h` flag:

```zsh
$ qemu-aarch64 main -h
Usage: ./main -f <input_file>
```

## Debugging

This is more for my own notes... Maybe I'll re-discover this at some point and
need it...

### Checking Return Codes

There are some return codes defined using the `.equ` directives in `main.s`.
After running the program, you can use `echo $?` to print the return code and
compare it to these values to see if it indicates an error.

### Viewing System Call Logs (QEMU)

Before jumping in with `gdb`, it is sometimes helpful to log the system calls
from [QEMU](https://www.qemu.org/) to see if it helps pinpoint a problem. To do
this, simply [build](#getting-started) the binary as normal, but run it with
`qemu-aarch64 -strace main` in place of `qemu-aarch64 main`, supplying the same
arguments as usual. This will show information about any syscalls made by the
program, and any return values from the calls.

This can also be used to view the return codes, as it shows the `exit` syscall
and return value:

```zsh
$ qemu-aarch64 -strace main -f
2088 write(1,0x4105c9,36)Error: Invalid arguments provided.
 = 36
2088 write(1,0x4105ed,31)Usage: ./main -f <input_file>
 = 31
2088 exit(1)
```

### Building with Debug Information

To build the object files with debug information, use the `just debug` recipe.
This will include the debug information in the object files, and also ensure
that the binary is up to date.

In another shell instance, you can then launch the binary in
[QEMU](https://www.qemu.org/), and get it to wait for `gdb` to connect with
`qemu-aarch64 -g 2001 main -f input.txt`, supplying arguments to the program as
normal.

Since the `just debug` recipe automatically launches `gdb`, you should then be
able to enter `target remote localhost:2001` into the `gdb` window to connect.

### Using `gdb` to Display Variables

Once in the program, set breakpoints as normal with `break`, and use `continue`,
`stepi`, and `nexti` as usual to get to a point of interest.

The following commands can then be used within `gdb` to inspect the buffers that
store each column of data:

Store the first 6 numbers in both data columns (used for test input):

```gdb
x/6uw &left_column
x/6uw &right_column
```

View the 4 elements in the middle of both columns (memory location is
incremented by 1992 as column is 4000 bytes long, grouped into 4-byte uints,
0-indexed):

```gdb
x/4uw ((char*)&left_column + 1992)
x/4uw ((char*)&right_column + 1992)
```

Display the entire of the left and right column:

```gdb
x/1000uw &left_column
x/1000uw &right_column
```
