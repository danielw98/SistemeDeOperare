# Sisteme de Operare

## Overview

This repository showcases my journey through understanding and implementing core Linux concepts. Developed between October 2019 and January 2020, this repository reflects not only the acquisition of theoretical knowledge but also practical application in systems programming.

## Featured Program: `talk.c`

Particularly, `talk` is a program that facilitates terminal-based communication on the same host machine using POSIX signals. The `talk.c` program stands out as a creative application of POSIX signals to enable text-based communication between two terminal instances. By assigning distinct signals to different characters, including space and newline, it simulates real-time messaging. This approach demonstrates the application of signals, signal mask blocking/unblocking, and handling race conditions, underscoring the complexity and efficiency of the solution.

## Demo Usage Animation

![Talk Program Demo](https://github.com/danielw98/SistemeDeOperare/blob/master/Talk.gif?raw=true)

## How It Works

- Upon startup, each instance of the `talk` program requires the PID of the other terminal instance to establish a connection.
- The program maps each character (a-z, space, and newline) to a unique POSIX signal, facilitating character-by-character communication.
- Signal handlers ensure that each character sent is appropriately received and displayed in the other terminal, maintaining the order and integrity of the message.

## Building and Running

Note: This project does not include a Makefile. Compile and run the `talk.c` program using gcc:

```bash
gcc talk.c -o talk
./talk
```

## Acknowledgments

This repository draws inspiration and knowledge from a plethora of resources, including Linux manual pages, academic texts (my teacher, Dragulici Dumitru-Daniel), and real-world programming challenges. It is a testament to continuous learning and the practical application of systems programming concepts.