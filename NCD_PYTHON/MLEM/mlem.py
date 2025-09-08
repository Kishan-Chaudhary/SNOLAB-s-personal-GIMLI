import numpy as np
import pandas as pd
import matplotlib.pyplot as plt




data = pd.read_csv("3NCD_Data2.csv", encoding='latin1')
data1 = pd.read_csv("Rebinned_ResponseMatrix_6x6.csv", encoding='latin1', header=None)

df = pd.DataFrame(data)
df1= pd.DataFrame(data1)
R=[df.iloc[2:31,3].to_numpy(),
       df.iloc[2:31,9].to_numpy(),
       df.iloc[2:31,15].to_numpy(),
       df.iloc[2:31,21].to_numpy(),
       df.iloc[2:31,27].to_numpy(),
       df.iloc[2:31,33].to_numpy()
       ]

R1= [df1.iloc[:,0].to_numpy(),
     df1.iloc[:,1].to_numpy(),
     df1.iloc[:,2].to_numpy(),
     df1.iloc[:,3].to_numpy(),
     df1.iloc[:,4].to_numpy(),
     df1.iloc[:,5].to_numpy()
     ]

R = np.array(R, dtype=float)
R1 = np.array(R1, dtype=float)

#----------------------------------------
Energies =  df.iloc[2:31,0].to_numpy()
Energies = np.array(Energies, dtype=float)

#----------------------------------------
# Response matrix import
#----------------------------------------
#R = response_matrix()
n,m = R.shape[0],R.shape[1]
print(R, n,m )
#----------------------------------------
# Pulse height import
#----------------------------------------
#dir = "unfolding_inputs/"
#neutrons = pd.read_csv(dir+"reduced_data.csv")
N1 = [178956, 365467, 351517,268617,185942, 367]# neutrons["NEUTRON 1"]
#N2 = neutrons["NEUTRON 2"]
#----------------------------------------
# Initial guess
#----------------------------------------
#true_data = np.loadtxt(dir+"energy-spectrum.txt")
#xtrue,xbins = true_data[0],true_data[1]
centers = [0.1, 3.5, 5, 6.5, 7.5]      # Example: peaks at 3 MeV and 7 MeV
sigmas = [0.2, 0.3, 0.2, 0.3, 0.3]   # Example: widths for each peak

# Generate the initial guess by summing Gaussians
x0 = np.zeros(len(Energies))
for c, s in zip(centers, sigmas):
    x0 += np.exp(-0.5 * ((Energies - c) / s) ** 2)


x = np.ones((m,))




def mlem(R,data,x,tolerance):
    """
    R --> Response matrix, shape is (n,m)
    N --> pulse height spectrum, shape is (n,)
    x --> initial guess at neutron spectrum, shape is (m,)
    tolerance --> user-defined stopping condition
    """
    x = x.copy()
    n = len(data) # true is the recorded specturm
    m = len(x)
    J0 = 0 ; dJ0 = 1 ; ddJ = 1
    error = []
    stepcount = 1
    while ddJ > tolerance:
        vector = np.zeros((n,))
        q = np.zeros((n,))
        for i in range(n):
            factor = (R[i,:]@x)
            vector[i] = data[i]/factor
            q[i] = factor

        for j in range(m):
            term = np.dot(R[:,j],vector)
            x[j] *= (1 / sum(R[:,j]))*term


        J = sum((q-data)**2) / sum(q)
        dJ = J0 - J
        ddJ = abs(dJ-dJ0)
        error.append(ddJ)
        J0 = J
        dJ0 = dJ
        print("Iteration {}, dJ = {}".format(stepcount,ddJ))
        stepcount += 1

    for i in range(n):
        print(R[i,:]@x)


    return(x,np.array(error))

# Define energy bin edges (in MeV)
bin_edges = np.array([1e-10, 1e-6, 0.1, 2.0, 4.0, 7.0, 11.0])
x2 = np.ones(len(bin_edges)-1)
x3 = [0, 0, 1000000, 2000000, 4000000, 3000000 ]
print(x2, R1)
tol = 0.0000001
#xg,errorg = gravel(R,N1,xtrue,tol)
xg,errorm = mlem(R1,N1,x2,tol)

# Compute bin centers for labeling
bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])

# Create labels for x-axis
bin_labels = [
    "10^(-10)-10^(-6)",
    "10^(-6)-0.1",
    "0.1-2",
    "2-4",
    "4-7",
    "7-11"
]
print(xg)
# Plot
plt.figure(figsize=(10, 5))
bars = plt.bar(range(6), xg, tick_label=bin_labels, width=0.8)
plt.xlabel("Energy Range (MeV)")
plt.ylabel("Neutron Counts")
plt.title("Unfolded MLEM Neutron Spectrum (6 Energy Bins)")
plt.grid(True, axis='y', linestyle='--', alpha=0.6)
plt.tight_layout()
plt.show()

tol = 0.00000000000001
#xg,errorg = gravel(R,N1,xtrue,tol)
xm,errorm = mlem(R,N1,x0,tol)

fig,ax = plt.subplots()
#ax.plot(xbins,(xtrue),color="k",label = "ToF Spectrum",alpha=0.3)
ax.plot(Energies,(xm),label="MLEM")
ax.grid(which="major",alpha=0.6)
ax.grid(which="minor",alpha=0.3)
ax.set_xlabel("Neutron Energy (MeV)")
ax.set_ylabel("Normalized Counts")
#zax.set_ylim(0,0.175)
#ax.xaxis.set_minor_locator(MultipleLocator(1))
plt.legend(frameon=False)
#plt.savefig("final-results/real-data-mlem-{}.png".format(suffix),dpi=300,bbox_inches="tight")
plt.show()
plt.close()


