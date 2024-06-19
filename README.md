# Qlock

Quine clock in C, with toolset to developp more quines.
Inspired by this [Tsoding](https://youtu.be/plFwBqBYpcY?si=MA4lpo_jqVJjglhC).
Who was inspired by [Aemkei](https://x.com/aemkei/status/1795573193559994505).

You can find two types of clocks in this repository: a simple one and a bouncing one.
Each clock is written in a "clear" way, and an "optim" way (which is harder to read but shorter).
The final quines are stored in the build directory and suffixed "packed".

## Quick start

```console
$ gcc -o nob nob.c
$ ./nob qlock_bouncing_optim.c
$ ./build/qlock
```

## Result

Bouncing quine clock:

![qlock](qlock.gif)
