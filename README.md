# nes-rom

A library to aid in NES game ROM conversions.

## Current State

Currently, this program can only convert a ROM file to a character array which
is intended to be used at a later time for easy ROM upload to the Arduino.

## How to Setup

This project tries to use all the basic pre-installed/standard libraries of the
various languages.

### Prerequisites

You will need the following list of programs to run:
```
Python 3.4 or up
gcc or cc
```

### Installation

None

## Test

To test the accuracy of the ROM to Header convert run the following command:

```bash
make header-test
```

Compare the 2 MD5 hash sums that are produced.  If successful, the two hashes
should be the same.

## Contributors

Currently, there is no other contributor.  If you wish to be part of this,
please message me.

## License

This program is license using The MIT License (MIT).  Please use the program
as you will.
