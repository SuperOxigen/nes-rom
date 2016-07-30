#! /usr/bin/python3

import argparse
import codecs
import os
from os.path import basename
import random
import sys

def binaryToHeader(istream, ostream, ofilename, varname):
    if istream is None or ostream is None:
        raise ValueError("Need input and output streams")

    def printOut(message):
        print(message, file=ostream, end="", flush=True)

    def readByte():
        return istream.read(1)

    macro = basename(ofilename.split(".")[0]).upper().replace("-", "_")

    printOut("#ifndef _{0}_\n".format(macro))
    printOut("#define _{0}_\n\n".format(macro))
    printOut("unsigned char {0}[] = {{".format(varname))

    byte = readByte()
    count = 0
    toHex = codecs.getencoder('hex')

    while byte != b"":
        if count != 0:
            printOut(", ")
        if count % 10 == 0:
            printOut("\n    ")
        printOut("0x{0}".format(toHex(byte)[0].decode()))
        count += 1
        byte = readByte()

    printOut("};\n\n")

    printOut("#endif\n\n")

def generateBinary(ostream, nbytes):
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
    sys.exit(main(*sys.argv[1:]))
