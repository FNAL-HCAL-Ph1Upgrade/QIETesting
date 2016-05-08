# Script to get a sorting file for N tray numbers
# Needs the SortingAll and SortingAllHardFail files

import argparse

def readInput():
    info = []
    with open("SortingAll.txt") as f:
        info.extend(f.readlines())
    with open("SortingAllHardFail.txt") as g:
        info.extend(g.readlines())
    return sorted(updateInput(info))

def updateInput(info):
    new_info = []
    n_bad_chips_phase = 0
    with open("phases_bad_chips.txt") as f:
        bad_chips = [elem.strip() for elem in f.readlines()]
        for sorting in info:
            chipinfo = sorting.split(",")
            if chipinfo[0] in bad_chips:
                if "0" not in chipinfo[-1]:
                    n_bad_chips_phase += 1
                #print chipinfo[0], sorting
                chipinfo[-1] = "0\n"
                new_info.append(",".join(chipinfo))
            else:
                new_info.append(sorting)
    print "Update bad status for %s chips" % (n_bad_chips_phase)
    return new_info
                  
def makeSortingFile(trays, output):
    all_info = readInput()
    nbadchips = 0
    with open(output,'w') as f:
        for l in all_info:
            lsplit = l.split(",")
            if lsplit[1] in trays:
                f.write(l)
                if "0" in lsplit[-1]:
                    nbadchips += 1
    print "%s bad chips in this batch" % nbadchips

if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(description='Extract sorting file')

    parser.add_argument('trays', metavar='tray numbers',
                        nargs='*',
                        help='tray numbers to make a sorting file for')
    parser.add_argument('-o', '--output', metavar='output file',
                        default="Sorting.txt", dest="output",
                        help='Name of output file')
    args = parser.parse_args()

    makeSortingFile(args.trays, args.output)
