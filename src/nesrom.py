#! /usr/bin/python3
"""NES ROM

Copyright (c) 2016 Alex Dale

This program is licensed under the MIT License (MIT). Please review the
accompanying LICENSE file for details.

This program consists of a set of useful tools for handling NES ROM files for
emulated NES hardware.  This tool is intended to make it easier for classic
NES game fans to produce and convert ROM files for various purposes.

This tool does not contain any copyrighted information
about the ROM files them self.  All design details have been retrieved from
publicly available information.

Source:
    http://nesdev.com/NESDoc.pdf

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import argparse
import codecs
import os
from os.path import basename
import random
import sys

def binaryToHeader(istream, ostream, ofilename, varname):
    """Binary (ROM) to C Header

    Writes the binary content of the input file to the output header file as
    a single unsigned character array.  The produced header file is ANSI C89
    onwards complient.

    Parameters:
        istream     - Input file stream containing the binary information of a
                      ROM.
        ostream     - Output file stream to be produced into a valid C header
                      file.
        ofilename   - The name of the C header file, used to produce the header
                      guard macro.
        varname     - The name of the variable.  Must be a valid C identifier.

    Returns:
        None
    """

    if istream is None or ostream is None:
        raise ValueError("Need input and output streams")

    def printOut(message):
        print(message, file=ostream, end="", flush=True)

    def readByte():
        return istream.read(1)

    macro = basename(ofilename.split(".")[0]).upper().replace("-", "_")

    # Start header guard
    printOut("#ifndef _{0}_\n".format(macro))
    printOut("#define _{0}_\n\n".format(macro))

    count = 0
    toHex = codecs.getencoder('hex')
    printOut("unsigned char {0}[] = {{".format(varname))
    byte = readByte()
    while byte != b"":
        if count != 0:
            printOut(", ")
        if count % 10 == 0:     # Keeps the line length less than 80 characters
            printOut("\n    ")  # Yes, spaces for intend
        printOut("0x{0}".format(toHex(byte)[0].decode()))
        count += 1
        byte = readByte()

    printOut("};\n\n")

    # End header guard
    printOut("#endif\n\n")

def generateBinary(ostream, nbytes):
    """Generate Binary with Random Bytes

    Generates a binary file with the 8*floor(nbytes/8) bytes of data to the
    specified file stream.

    Parameters:
        ostream     - Output file stream that the random binary information will
                      be written to.
        nbytes      - The maximum number of bytes to run.  Exact number of bytes
                      is closest multiple of 8 less than or equal to nbytes.
    """
    for _ in range(nbytes // 8):
        ostream.write(os.urandom(8))

def parseArguments(*argv):
    parser = argparse.ArgumentParser(description="Utility functions for processing Nintendo Entertainment System game ROM files")
    cmdParsers = parser.add_subparsers(help="Selection for program function", dest="command")

    toHeaderParser = cmdParsers.add_parser("header", help="Generate a C header from a binary")
    toHeaderParser.add_argument("-o", "--output", help="Output file name", action="store", dest="ofilename", metavar="OUTPUT")
    toHeaderParser.add_argument("--var", help="Output character array name", action="store", dest="varname", metavar="VAR_NAME")
    toHeaderParser.add_argument(metavar="INPUT", help="Input binary file to be converted", dest="ifilename")

    randomBinaryParser = cmdParsers.add_parser("rand", help="Generate a random binary file")
    randomBinaryParser.add_argument("-o", "--output", help="Output file name", action="store", dest="ofilename")
    randomBinaryParser.add_argument("-s", "--size", help="size of the binary file to be created in kB",
                                    action="store", type=int, choices=[32, 64], dest="binsize")

    return parser.parse_args(args=argv)

def main(*argv):
    options = parseArguments(*argv)

    if options.command == "header":
        istream = open(options.ifilename, mode="rb")
        if istream is None:
            print("Failed to open input file {0}".format(options.ifilename), file=sys.stderr)
            return 1
        ostream = open(options.ofilename, mode="w+")
        if ostream is None:
            print("Failed to open output file {0}".format(options.ofilename), file=sys.stderr)
            istream.close()
            return 1
        binaryToHeader(istream, ostream, options.ifilename, options.varname)
        istream.close()
        ostream.close()
        return 0

    if options.command == "rand":
        ostream = open(options.ofilename, mode="w+b")
        if ostream is None:
            print("Failed to open output file {0}".format(options.ofilename), file=sys.stderr)
        nbytes = options.binsize * 1024
        generateBinary(ostream, nbytes)
        ostream.close()
        return 0

    return 0

if __name__ == "__main__":
    # Program starts here.  Strips the first argument off
    sys.exit(main(*sys.argv[1:]))
