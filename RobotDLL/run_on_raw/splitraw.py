# script to split a raw data file into N files, one per chip
# for now only include the phase test info 

import argparse, os
import sys

def splitfile2(inputfile, outputdir):
    with open(inputfile) as inf:
        chipnr = ""
        for line in inf:
            if "Chip" in line:
                chipnr = line.strip().split()[3]
                if chipnr == "=":
                    print line

        outf.close()

def splitfile(inputfile, outputdir):
    with open(inputfile) as inf:
        chipnr = ""
        outf = None
        for line in inf:
            if "Chip" in line:
                chipnr = line.strip().split()[3]
                # Close old file, and open new one
                if outf:
                    outf.close()
                outf = open("%s/%s.raw"%(outputdir,chipnr),'w')
            # if we are in a block of chip data, ie an outf is present
            if outf:
                # first pass is to print all the raw data
                outf.write(line)

        outf.close()

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Split raw QIE test data.')
    
    parser.add_argument('inputfile', metavar='INPUTFILE',
                        nargs='*',
                        help='Input file(s) containing raw QIE test data.')
    parser.add_argument('-o','--outputdir', dest='outputdir', metavar='OUTPUTDIR',
                        help='Output directory',
                        default=None)

    args = parser.parse_args()

    for inputfile in args.inputfile:
        outputdir = args.outputdir if args.outputdir else "files_%s"%args.inputfile[0]
        if not os.path.isdir(outputdir):
            os.makedirs(outputdir)

        splitfile(inputfile,outputdir)
