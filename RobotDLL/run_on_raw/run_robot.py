import argparse
import glob
import subprocess

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Run RobotCodeFinal')
    
    parser.add_argument('inputdir', metavar='INPUTDIR',
                        help='Directory of all raw files you want to process.')

    args = parser.parse_args()


    flist = glob.glob("%s/*"%args.inputdir)
    for f in flist:
        if "phase" not in f:
            print "processing file", f
            # process the file
            subprocess.call(['./RobotCodeFinal', f, f.replace(".raw", "_phase.txt")])
