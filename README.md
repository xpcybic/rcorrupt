# rcorrupt - A ROM (or other file) corruptor written in C
an interesting description goes here

## Features
- replaces parts of a file with garbage
- range of addresses and corruption rate can be specified
- is tiny and fast
- ~~probably~~ definitely broken somewhere

## Installing
1. git clone or download zip
2. make

## Usage
`rcorrupt <options> <target>`
### Options

Argument | Description
--- | ---
Corruption mode (what to write to corrupted bytes)
`-m rand, -m r `| Random values (default)
`-m <val> `| Fixed value (0-255)
`-m +<val> `| dd value (0-255)
Corruption range/amount options
`-r <start> <end> `| Range of bytes to corrupt (default: entire file)
`-p <val> `| Approx. percentage of addresses to corrupt (default: 10)
`-i <val> `| Increment (corrupt every *val*th byte)
Other
`-f `| Force (don't ask for confirmation before corrupting)
`-q `| Quiet (don't display results when done)
`-h `| Display help

### Examples
- Pokemon Red/Blue
	- `rcorrupt -f -r 550000 650000 pkmnred.gb`
- Megaman II
	- `rcorrupt -f -r 130000 180000 megaman2.nes`

## Todo
- file backup
- user-friendliness
- testing
- a good readme

## Disclaimer
Use at your own risk. I am not responsible for damage, loss of data, etc. as a result of using this, especially on important files you don't want to write garbage to.
