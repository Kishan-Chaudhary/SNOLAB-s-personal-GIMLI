import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# === Load the response matrix from CSV ===
# Each "Effective Efficiency" column is at: 3, 9, 15, 21, 27, 33

# Each "Effective Efficiency" column is at: 3, 9, 15, 21, 27, 33
df = pd.read_csv("3NCD_Data2.csv", header=None)

Energies =  df.iloc[3:32,0].to_numpy()
#print(Energies)
Energies = np.array(Energies, dtype=float)

# Extract 29 rows (energy bins) starting from row 3 (index 3 to 31)
eff_cols = [3, 9, 15, 21, 27, 33]
R_list = [df.iloc[3:32, col].astype(float).values for col in eff_cols]

# Stack into a response matrix (shape: 6 configs × 29 energy bins)
R = np.vstack(R_list)
print(R)
# === Measured counts (provided by user) ===
data = np.array([178956, 365467, 351517, 268617, 185942, 367], dtype=float)


# === Initial guess (multiple Gaussian peaks) ===
# Define centers and sigmas for each peak
centers = [0.1, 3.5, 5, 6.5, 7.5]      # Example: peaks at 3 MeV and 7 MeV
sigmas = [0.2, 0.3, 0.2, 0.3, 0.3]   # Example: widths for each peak

# Generate the initial guess by summing Gaussians
x0 = np.zeros(len(Energies))
for c, s in zip(centers, sigmas):
    x0 += np.exp(-0.5 * ((Energies - c) / s) ** 2)

# Optional: Normalize x0
x0 = x0 / np.sum(x0)

# Optional: Normalize x0 if desired
#x0 = x0 / np.sum(x0)
x1 = np.ones(len(Energies))  # Alternative initial guess (constant spectrum))
# === MLEM function ===
def mlem(R, data, x0, iterations):
    x = x0.copy()
    R_T = R.T
    for _ in range(iterations):
        estimate = R @ x
        estimate = np.maximum(estimate, 1e-12)  # avoid divide-by-zero
        update_factor = R_T @ (data / estimate)
        normalization = R_T.sum(axis=1)
        normalization = np.maximum(normalization, 1e-12)
        x *= update_factor / normalization
    return x

# === Run MLEM ===
x_mlem = mlem(R, data, x0, iterations=10000000)

# === Normalize spectrum ===
x_mlem_norm = x_mlem / np.sum(x_mlem)

# === Plot ===
plt.figure(figsize=(10, 5))
plt.plot(Energies, x_mlem_norm, marker='o', label='MLEM Unfolded Spectrum')
plt.xlabel("Energies")
plt.ylabel("Normalized Neutron Counts")
plt.title("Unfolded Neutron Spectrum using MLEM")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()