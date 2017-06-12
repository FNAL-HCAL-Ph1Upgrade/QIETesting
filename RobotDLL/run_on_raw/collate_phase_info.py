import glob

if __name__ == "__main__":
    outdir = "files_raw_QIE10_wafer11-13_phase"

    indirs = glob.glob("/Users/nstrobbe/Work/HCALPhase1/GIT/QIETesting/RobotDLL/run_on_raw/files_raw_QIE10_vWafer1[1,2,3]*")
    for indir in indirs:
        #print indir.split("/")[-1]
        #continue
        with open("%s/%s_phase.txt"%(outdir, indir.split("/")[-1]), 'w') as outf:
            fs = glob.glob("%s/*phase.txt"%indir)
            for f in fs:
                with open(f) as g:
                    outf.write(" Chip Number =   %s\n" % (f.strip().split("/")[-1].split("_")[0]))
                    for l in g:
                        outf.write(l)
