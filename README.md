# hxr

H[e]x[e]r is a tool for binary-related visualisation.

## How to use query command:

`hxr` is brilliant at conversions. If you want to convert a number from decimal to hex:

```
$ hxr query -u 255
   ff              00              00              00
```

If you want to see binary output too, add `-b`

```
$ hxr query -bu 255
   ff              00              00              00
1111 1111       0000 0000       0000 0000       0000 0000
```
Is 4 bytes too much? Tell it the size `-s`

```
$ hxr query -bu 255 -s 1
   ff
1111 1111
```

What about signed integers? 

```
hxr query -bd -255
   01              ff              ff              ff
0000 0001       1111 1111       1111 1111       1111 1111
```

What about text? 

```
hxr query -t "hello"
h               e               l               l               o
68              65              6c              6c              6f
0110 1000       0110 0101       0110 1100       0110 1100       0110 1111
```

## How to use evaluate command

The evaluate command has two modes: an expression mode (experimental) and an input file mode.

To use the evaluate command with a file, simple give it a file and the type word-size for every
chunk:

```
hxr evaluate --file main.cpp --type b
0010 0011        35
0110 1001        105
0110 1110        110
0110 0011        99
0110 1100        108
0111 0101        117
0110 0100        100
0110 0101        101
0010 0000        32
0011 1100        60
0110 0001        97
0111 0010        114
0111 0010        114
0110 0001        97
0111 1001        121
0011 1110        62
```
and it will read one byte at a time, and give you a decimal output of every byte.

If you have other data and know it's type, for instance a binary dump of an array of
unsigned 32-bit integers: 

```
hxr mem.bin --type u32
0110 0100       0000 0000       0000 0000       0000 0000        100
1100 1000       0000 0000       0000 0000       0000 0000        200
0010 1100       0000 0001       0000 0000       0000 0000        300
1001 0000       0000 0001       0000 0000       0000 0000        400
1111 0100       0000 0001       0000 0000       0000 0000        500
```

Notice how the last column in both examples is the decimal representation of the data.

## How to build

The easiest way is to use nix:

```
nix develop
cmake --workflow --preset debug --fresh
# or
cmake --workflow --preset release --fresh
```

