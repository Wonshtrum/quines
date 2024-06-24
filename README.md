# Qlock

Quine clock in C, with a toolset to develop more quines.
Inspired by this [Tsoding](https://youtu.be/plFwBqBYpcY?si=MA4lpo_jqVJjglhC).
Who was inspired by [Aemkei](https://x.com/aemkei/status/1795573193559994505).

Directories:
- `clear`: easy-to-read, commented quines to explain the principles used.
- `opti1`: harder to read but much shorter quines, using:
    - bitmap compression by integer substraction
    - boolean expression evaluation order
    - removal of curly braces around single statment blocks
- `opti2`: even shorter and more complex quines, using:
    - ASCII code for `:` is 58, no need to set it manually (don't know how I didn't see this before)
    - bitmap compression by ASCII conversion
    - ASCII shifting for escaping
    - arbitrary padding between string and code

To simplify the development of quines, this project uses the nob.c build script which handles most of the heavy lifting. It takes a C source file as input in the following format:

```c
_escaped = 1;        // uses '\' to escape characters in the string, if 0, uses ASCII shift instead
_width = 80;         // the wrapping width, 80 if not set
_data = "DATA...";   // appended at the start of the string, it will not be duplicated
_post = "//POST..."; // appended at the end of the string AND at the end of the code
_pad = ...;          // line offset of the string end, if 0, _post will grow to fill the width

C_CODE_PRE_STR...
S="#";
C_CODE_POST_STR...
```

and outputs a packed corresponding quine:

```c
PACKED_C_CODE_PRE_STR...
S="DATA...ESCAPED_PACKED_C_CODE_PRE_STR...S=\"#\";ESCAPED_PACKED_C_CODE_POST_STR...//POST...";
PACKED_C_CODE_POST_STR...//POST...
```


## Quick start
### Build a single quine using nob directly

Compile the nob script:

```console
$ gcc -o nob nob.c
// or
$ make nob
```

Use nob to pack a quine:

```console
$ ./nob opti1/qlock_bouncing.c
```

The packed quine source code will be in `build/packed.c`, and the binary in `build/qlock`.

Check the output:

```console
$ cat build/packed.c
$ ./build/qlock
```

### Build All Quines with the Makefile

Run the Makefile:
```console
$ make
```

Check the outputs:

```console
$ cat opti1/pck_qlock_bouncing.c
$ ./opti1/bin_qlock_bouncing
```

All quines' source code will be in their corresponding directories with the prefix "pck", and all binaries with the prefix "bin".


## Result

Bouncing quine clock:

![qlock](qlock.gif)
