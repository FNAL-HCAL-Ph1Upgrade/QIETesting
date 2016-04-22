# little script to aggregate hard failure info

import argparse
from collections import defaultdict

def mysum(info):
    total = 0
    for k,v in info.iteritems():
        total += v
    return total

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Aggregate hard failure info.')

    parser.add_argument('inputfile', metavar='INPUTFILE',
                        nargs='*',
                        help='Input file(s) containing hard failure data')
    parser.add_argument('-v', '--verbose', dest="v",
                        default=False, action='store_true',
                        help="Be more verbose")

    args = parser.parse_args()


    with open("hardfailures.tex",'w') as f:
        f.write("\\begin{tabular}{l||c|c|c|c||c|c|c}\n")
        f.write("Wafer & capid & registers & DLL & mux & Total & \\#chips & \\% failures \\\\ \\hline \\hline \n")
        total_info = defaultdict(int)
        total_chips_list = []
        # Go through all input files and get info
        for inputfile in args.inputfile:
            # extract which wafer it it
            wafer = inputfile.partition("vWafer")[-1][:2]
            # get hard failure info
            info = defaultdict(int)
            with open(inputfile) as inputf:
                for line in inputf:
                    lsplit = line.strip().split()
                    info[lsplit[-1]] += 1
            for k,v in info.iteritems():
                total_info[k] += v
            # Get total number of chips
            total_chips = ""
            with open(inputfile.replace("_breakdown","")) as chipf:
                total_chips = chipf.readline().strip().split()[-1]
            total_chips_list.append(int(total_chips))
            # Write info
            if len(info) > 4:
                print "More failure modes present"
            f.write("%s & %s & %s & %s & %s & %s & %s & %.1f \\%%\\\\ \n" % (wafer,
                                                                        info["capid_test"],
                                                                        info["qie10_registers"],
                                                                        info["hist_results"],
                                                                        info["local_muxed_adcs"],
                                                                        mysum(info),
                                                                        total_chips,
                                                                        mysum(info)/float(total_chips)*100)
                    )

        f.write("\\hline \n")
        f.write("Total & %s & %s & %s & %s & %s & %s & %.1f \\%%\\\\ \n" % (total_info["capid_test"],
                                                                            total_info["qie10_registers"],
                                                                            total_info["hist_results"],
                                                                            total_info["local_muxed_adcs"],
                                                                            mysum(total_info),
                                                                            sum(total_chips_list),
                                                                            mysum(total_info)/float(sum(total_chips_list))*100)
                )
        f.write("\\end{tabular}\n")

