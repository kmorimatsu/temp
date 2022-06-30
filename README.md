# Temp
Temporary git archive

## about Temp

Currently testing the multicore function of Raspberry Pi Pico

## FIFO

Each FIFO is 32 bits wide, and eight entries deep. One of the FIFOs can only be written by core 0, and read by core 1. The other can only be written by core 1, and read by core 0.

## Result
```console
Hello, multicore world!
Hello, core0 only: 677984!
Hello, core0: 812645!
Hello, core1: 805194!
```
