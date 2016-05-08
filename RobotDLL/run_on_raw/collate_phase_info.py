import glob

if __name__ == "__main__":
    outdir = "files_raw_vWafer2-11"

    indirs = glob.glob("files_raw_vWafer??")
    for indir in indirs:
        with open("%s/%s_phase.txt"%(outdir,indir), 'w') as outf:
            fs = glob.glob("%s/*phase.txt"%indir)
            for f in fs:
                with open(f) as g:
                    outf.write(" Chip Number =   %s\n" % (f.strip().split("/")[-1].split("_")[0]))
                    for l in g:
                        outf.write(l)
