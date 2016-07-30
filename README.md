# nes-rom

A library to aid in NES game ROM conversions.

## Current State

Currently, this program can only convert a ROM file to a character array which
is intended to be used at a later time for easy ROM upload to the Arduino.

## Test

To test the accuracy of the ROM to Header convert run the following command:

```bash
make header-test
```

Compare the 2 MD5 hash sums that are produced.  If successful, the two hashes
should be the same.
