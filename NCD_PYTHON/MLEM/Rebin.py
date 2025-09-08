
from encodings import latin_1
import pandas as pd
import numpy as np


# === Load your CSV file ===
data = pd.read_csv("3NCD_Data2.csv", encoding='latin1')
data2 = pd.read_csv("3NCD_Data_high_resolution.csv", encoding='latin1')
df = pd.DataFrame(data)
df2 = pd.DataFrame(data2)
def bin_average_response(E, R, bins):
    """
    Compute weighted-average response in energy bins using ΔE weighting.

    Parameters
    ----------
    E : array-like
        Energies (must be sorted ascending).
    R : array-like
        Responses corresponding to energies.
    bins : array-like
        Bin edges (e.g. [0, 1e-6, 0.5, 2, 4, 7, 11]).

    Returns
    -------
    R_bin : np.ndarray
        Weighted-average response per bin.
    """

    E = np.asarray(E)
    R = np.asarray(R)
    bins = np.asarray(bins)

    # compute ΔE for each response point
    dE = np.empty_like(E)
    dE[0]  = (E[1] - E[0]) / 2
    dE[1:-1] = (E[2:] - E[:-2]) / 2
    dE[-1] = (E[-1] - E[-2]) / 2

    # weighted sums inside bins
    num, _ = np.histogram(E, bins=bins, weights=R * dE)
    den, _ = np.histogram(E, bins=bins, weights=dE)

    return num / den


EnergiesNew =  df2.iloc[2:70,0].to_numpy()
print(EnergiesNew)
EnergiesNew = np.array(EnergiesNew, dtype=float)
R1 = df2.iloc[2:70,3].to_numpy()
R2 = df2.iloc[2:70,9].to_numpy()
R3 = df2.iloc[2:70,15].to_numpy()
R4 = df2.iloc[2:70,21].to_numpy()
R5 = df2.iloc[2:70,27].to_numpy()
R6 = df2.iloc[2:70,33].to_numpy()


EnergiesOld =  df.iloc[2:31,0].to_numpy()
print(EnergiesOld)
EnergiesOld = np.array(EnergiesOld, dtype=float)
R11 = df.iloc[2:31,3].to_numpy()
R22 = df.iloc[2:31,9].to_numpy()
R33 = df.iloc[2:31,15].to_numpy()
R44 = df.iloc[2:31,21].to_numpy()
R55 = df.iloc[2:31,27].to_numpy()
R66 = df.iloc[2:31,33].to_numpy()

"""
,
       df.iloc[2:31,9].to_numpy(),
       df.iloc[2:31,15].to_numpy(),
       df.iloc[2:31,21].to_numpy(),
       df.iloc[2:31,27].to_numpy(),
       df.iloc[2:31,33].to_numpy()
 """
R1 = np.array(R1, dtype=float)
R2 = np.array(R2, dtype=float)
R3 = np.array(R3, dtype=float)
R4 = np.array(R4, dtype=float)
R5 = np.array(R5, dtype=float)
R6 = np.array(R6, dtype=float)

R11 = np.array(R11, dtype=float)
R22 = np.array(R22, dtype=float)
R33 = np.array(R33, dtype=float)
R44 = np.array(R44, dtype=float)
R55 = np.array(R55, dtype=float)
R66 = np.array(R66, dtype=float)


# Define your 6 energy bin groups by edges
# Define your energy bins (edges)
bins = [0, 1e-6, 0.1,2,4,7,11]
#print(R1)
R_bin1 = bin_average_response(EnergiesNew, R1, bins)
R_bin11 = bin_average_response(EnergiesOld, R11, bins)
#R_bin1 = R_bin1.T
#print(R_bin1)
R_bin2 = bin_average_response(EnergiesNew, R2, bins)
R_bin22 = bin_average_response(EnergiesOld, R22, bins)
#print(R_bin2)
R_bin3 = bin_average_response(EnergiesNew, R3, bins)
R_bin33 = bin_average_response(EnergiesOld, R33, bins)
#print(R_bin3)
R_bin4 = bin_average_response(EnergiesNew, R4, bins)
R_bin44 = bin_average_response(EnergiesOld, R44, bins)
#print(R_bin4)
R_bin5 = bin_average_response(EnergiesNew, R5, bins)
R_bin55 = bin_average_response(EnergiesOld, R55, bins)
#print(R_bin5)
R_bin6 = bin_average_response(EnergiesNew, R6, bins)
R_bin66 = bin_average_response(EnergiesOld, R66, bins)
#print(R_bin6)

R_rebinned = np.column_stack([R_bin1, R_bin2, R_bin3, R_bin4, R_bin5, R_bin6])

R_rebinned2 = np.column_stack([R_bin11, R_bin22, R_bin33, R_bin44, R_bin55, R_bin66])

np.savetxt("Rebinned_HGResponseMatrix_6x6.csv", R_rebinned, delimiter=",", fmt="%.8e")

np.savetxt("Rebinned_ResponseMatrix_6x6.csv", R_rebinned2, delimiter=",", fmt="%.8e")
