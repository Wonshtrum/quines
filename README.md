# Quines

> A quine is a computer program that takes no input and
> produces a copy of its own source code as its only output.

Collection of quines in C, with a toolset to develop more quines.
Inspired by [Tsoding](https://youtu.be/plFwBqBYpcY?si=MA4lpo_jqVJjglhC).
Who was inspired by [Aemkei](https://x.com/aemkei/status/1795573193559994505).

Directories:
- `qlock_clear`: easy to read, commented clocks to explain the principles used.
- `qlock_opti1`: harder to read but much shorter clocks, using:
    - bitmap compression by integer subtraction
    - boolean expression evaluation order
    - removal of curly braces around single statement blocks
- `qlock_opti2`: even shorter and more complex clocks, using:
    - bitmap compression by ASCII conversion
    - ASCII shifting for escaping
    - arbitrary padding between string and code
- `gol`: Game of Life quine (optimized)
- `badapple`: 19 seconds Bad Apple gif player (optimized)

The `qlock_opti2`, `gol`, and `badapple` quines use bitmaps to efficiently store information. The python scripts used to generate them are included in their respective folders.

## Results

Bouncing quine clock:

![qlock](qlock.gif)

Conway's Game of Life:

![gol](gol.gif)

Bad Apple:

![badapple](badapple.gif)

## Quick start
### Build a single quine with nob

Compile the nob script:

```console
$ gcc -o nob nob.c
// or
$ make nob
```

Use nob to pack a quine. For example:

```console
$ ./nob qlock_opti1/qlock_bouncing.c
```

You will find in the `build` directory:
- `fmt.c`: formatted source code (containing the packed pre-str and post-str code)
- `pck.c`: final packed quine source code (with the escaped code string)
- `bin`: executable binary

Check the output:

```console
$ cat build/pck.c
$ ./build/bin
```

### Build multiple quines with nob

The `nob.c` script accepts directory as source. In this case it will find every `src_*.c` files in this directory and generate for each:
- a `pck_*.c` file: the final packed quine source code
- a `bin_*` file: the corresponding executable

For example:

```console
$ ./nob qlock_opti1
```

Check the output:

```console
$ ls qlock_opti1
$ cat qlock_opti1/pck_qlock_bouncing.c
$ ./qlock_opti1/bin_qlock_bouncing
```

### Build all quines with the Makefile

Run the Makefile:
```console
$ make
```

This will build all quines in `qlock_clear`, `qlock_opti1`, `qlock_opti2`, `gol`, and `badapple` directories.

Check the output:

```console
$ ls qlock_opti1
$ cat qlock_opti1/pck_qlock_bouncing.c
$ ./qlock_opti1/bin_qlock_bouncing
```

## Developing more quines with this toolset
To simplify the development of quines, this project uses the `nob.c` build script which handles most of the heavy lifting:
- code minimization: removes all unnecessary white spaces, new lines and comments
- code inclusion: replaces "#" placeholder with escaped source code
- code formatting: wraps code according to a fixed width

It takes a C source file as input in the following format:

```c
// all _* variables at the top of the file are read by nob as meta parameters
_escaped = 1;      // uses '\' to escape characters in the string, if 0, uses ASCII shift instead
_width = 80;       // the wrapping width, 80 if not set
_pad = ...;        // line offset of the string end, if 0, _post will grow to fill the width
_data = "DATA..."; // appended at the start of the string
_post = "POST..."; // appended at the end of the string
_skip_post = 0;    // append _post at the end of the code
_mergeable_nl = 0; // if 1 new lines don't count as separators as they can be merged
_no_comments = 0;  // use only spaces (no comments) for line padding

C_CODE_PRE_STR...
S="#";
C_CODE_POST_STR...
```

and outputs the packed corresponding quine:

```c
PACKED_C_CODE_PRE_STR...
S="DATA...ESCAPED_PACKED_C_CODE_PRE_STR...S=\"#\";ESCAPED_PACKED_C_CODE_POST_STR...POST...";
PACKED_C_CODE_POST_STR...POST...
```
