## Instructions for running on a raw data file using the robot DLL code

I made this code compile on my mac. If you have a different system, you might need to make some modifications. 

You will need the gsl libraries to use this code. To install this I used homebrew:
```
brew install gsl
```
To compile, simply do `make`. 

The code takes a raw input file for one chip as input. If you want to split up a raw data file containing info on multiple chips, you can use the `splitraw.py` script:
```
python splitraw.py -o my_output_dir RAW_FILE_1 [RAW_FILE_2 ...]
```

To run the DLL code on each file, you can use the `run_robot.py` script:
```
python run_robot.py INPUTDIR
```
To run the DLL code on one file:
```
./RobotCodeFinal inputfile outputfile
```

Once all the files have been processes, you can collate them into a "cuts_all"-like file using the `collate_phase_info.py` script. This script is not yet very flexible, so you'll have to update the input files yourself in the script. 
