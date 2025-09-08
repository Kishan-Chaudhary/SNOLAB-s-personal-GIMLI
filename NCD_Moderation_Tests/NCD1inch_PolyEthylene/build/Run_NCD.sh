#!/bin/bash
#PBS -N NCD_1inchPE_Kishan
#PBS -j oe
#PBS -M kishan.chaudhary@snolab.ca
#PBS -m abe

# Change to the directory from which the job was submitted
cd /users/kchaudhary/NCD/NCD1inch_PolyEthylene/build/

# Optional: print diagnostic info
echo "Running on host: kchaudhary"
echo "Starting job at: $(date)"
echo "Using $PBS_NP cores"

# Run the Geant4 executable with your macro file
./NCD > /dev/null 2>&1

echo "Job finished at: $(date)"
