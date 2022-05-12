# Temp
Temporary git archive

## about Temp

Currently testing the multicore function of Raspberry Pi Pico

## FIFO

Each FIFO is 32 bits wide, and eight entries deep. One of the FIFOs can only be written by core 0, and read by core 1. The other can only be written by core 1, and read by core 0.

## Result
```console
Hello, multicore world!
Hello, core1: 2000!
Hello, core0: 1000!
Hello, core1: 2001!
Hello, core0: 1001!
Hello, core1: 2002!
Hello, core0: 1002!
Hello, core1: 2003!
Hello, core0: 1003!
Hello, core1: 2004!
Hello, core0: 1004!
Hello, core1: 2005!
Hello, core0: 1005!
Hello, core1: 2006!
Hello, core0: 1006!
Hello, core1: 2007!
Hello, core0: 1007!
Hello, core1: 2008!
```
