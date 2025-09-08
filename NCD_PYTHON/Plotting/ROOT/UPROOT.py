import uproot
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt



# Open the ROOT file
file = uproot.open("NCD_Poly_AmBeTest_t0.root")
file2 = uproot.open("NCD_Poly_AmBeSpectra.root")
# List all TTrees
print(file.keys())

# Access your TTree (replace 'tree_name' with actual name)
tree = file["EnergyDeposited"]
tree1 = file2["Primary Neutron spectra"]
# View available branches (columns)
print(tree.keys())
print(tree1.keys())
# Convert to pandas DataFrame
df = tree.arrays(library="pd")
df1 = tree1.arrays(library="pd")
# Print all values in 'Total_Energy_Deposited' greater than 0.8
high_edep = df[df["Total_Energy_Deposited"] > 0.8]
print("EDEP with more than 0.8:\n", high_edep)

# Show first few rows
print(df.head())

print(df)


plt.hist(df["Total_Energy_Deposited"], bins=60, color="skyblue", edgecolor="black")
plt.xlabel("Energy Deposition (MeV)")
plt.ylabel("Counts")
plt.title("Energy Deposition Spectrum")
plt.grid(True)
plt.show()
df["Total_Energy_Deposited"] = 1000*df["Total_Energy_Deposited"]
# Calculate number of ion pairs for each entry and add Gaussian noise
#ion_pairs = df["Total_Energy_Deposited"] / 3.6e-5
noise_sigma = 10.0  # standard deviation of the noise 0.04, 0.030
ion_pairs_noisy = []
for i in df:
    ion_pairs_noisy = np.random.normal(loc=df["Total_Energy_Deposited"], scale=noise_sigma)

# Plot the noisy ion pairs histogram
plt.hist(ion_pairs_noisy, bins=1200, color="black", edgecolor="black")
#plt.xlabel("Energy Deposition per event(MeV) in 1500 bins")
plt.xlabel("Energy (keV)", fontsize="21")
plt.ylabel("Counts", fontsize="21")
plt.tick_params(axis='x', labelsize=20)  # x-axis ticks
plt.tick_params(axis='y', labelsize=20)
plt.yscale('log')
plt.xlim(0, 900)  # Set x-axis limits f
#plt.title("Neutron Capture Energy Deposition in GEANT4",fontsize="21")
plt.grid(True)
plt.show()
