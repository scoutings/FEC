# Assignment 5

This is a program that will encode and decode given text. Thiswill use FEC to be able to correct an erros ion bits.

bm.c:           implementation of the Bit Matrix ADT

bv.c            implementation of the Bit Vector ADT

constants.c     a file used to create constants for encode and decode

encode.c        this will take a file as input and output the file encoded using hamming code

decode.c        this will take an encoded file as input and decode it coreecting an bits if possible

entropy.c       this is a given file that will level the measure of entropy in a given file

error.c         this ia a given file that is able to produce errors from stdin to stdout

hamming.c       implementaion of hamming encode and decode on the byte level

The command line arguments when running encode are:

  -h             Program usage and help.
  -i infile      Input data to encode.
  -o outfile     Output of encoded data.

The command line arguments when running decode are:

  -h             Program usage and help.
  -v             Print statistics of decoding to stderr.
  -i infile      Input data to encode.
  -o outfile     Output of encoded data.

## Build

    $ make all
    $ make encode
    $ make decode

## Running

    $ ./encode -h -i <file_name> -o <file_name>
    $ ./decode -hv -i <file_name> -o <file_name>

## Cleaning

    $ make clean    
