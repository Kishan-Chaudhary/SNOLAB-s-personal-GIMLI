from cProfile import label
from math import pi
from turtle import color
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from numpy import log,exp
import matplotlib.ticker as ticker

data = pd.read_csv("3NCD_Data2.csv", encoding='latin1')
data1 = pd.read_csv("Rebinned_ResponseMatrix_6x6.csv", encoding='latin1', header=None)
data2 = pd.read_csv("Rebinned_HGResponseMatrix_6x6.csv", encoding='latin1', header=None)
df = pd.DataFrame(data)
df1 = pd.DataFrame(data1)
df2 = pd.DataFrame(data2)
#print(df1)
R=[df.iloc[2:31,3].to_numpy(),
       df.iloc[2:31,9].to_numpy(),
       df.iloc[2:31,15].to_numpy(),
       df.iloc[2:31,21].to_numpy(),
       df.iloc[2:31,27].to_numpy(),
       df.iloc[2:31,33].to_numpy()
       ]

#----------------------------------------
#Condition number of the response matrix




R1= [df1.iloc[:,0].to_numpy(),
     df1.iloc[:,1].to_numpy(),
     df1.iloc[:,2].to_numpy(),
     df1.iloc[:,3].to_numpy(),
     df1.iloc[:,4].to_numpy(),
     df1.iloc[:,5].to_numpy()
     ]


R2 = [df2.iloc[:,0].to_numpy(),
     df2.iloc[:,1].to_numpy(),
     df2.iloc[:,2].to_numpy(),
     df2.iloc[:,3].to_numpy(),
     df2.iloc[:,4].to_numpy(),
     df2.iloc[:,5].to_numpy()
     ]


#print(R1)
R = np.array(R, dtype=float)
R1 = np.array(R1, dtype=float)
R2 = np.array(R2,dtype=float)
condition_number = np.linalg.cond(R)
print("Condition number of the response matrix:", condition_number)

condition_number = np.linalg.cond(R1)
print("Condition number of the response matrix:", condition_number)

#----------------------------------------
Energies =  df.iloc[2:31,0].to_numpy()
Energies = np.array(Energies, dtype=float)

#----------------------------------------
# Response matrix import
#----------------------------------------
#R = response_matrix()
n,m = R.shape[0],R.shape[1]
#print(R, n,m )
#----------------------------------------
# Pulse height import
#----------------------------------------
#dir = "unfolding_inputs/"
#neutrons = pd.read_csv(dir+"reduced_data.csv")
N1 = np.array([178956, 365467, 351517,268617,185942, 367])# neutrons["NEUTRON 1"]
#N2 = neutrons["NEUTRON 2"]
#----------------------------------------
# Initial guess
#----------------------------------------
#true_data = np.loadtxt(dir+"energy-spectrum.txt")
#xtrue,xbins = true_data[0],true_data[1]
centers = [0.1, 3.5, 5, 6.5, 7.5]      # Example: peaks at 3 MeV and 7 MeV
sigmas = [0.2, 0.3, 0.2, 0.3, 0.3]   # Example: widths for each peak

# Generate the initial guess by summing Gaussians
x0 = np.ones(len(Energies))
for c, s in zip(centers, sigmas):
    x0 += np.exp(-0.5 * ((Energies - c) / s) ** 2)


x = np.ones((m,))




def gravel(R,data,x,tolerance):
    """
    R --> Response matrix, shape is (n,m)
    N --> pulse height spectrum, shape is (n,)
    x --> initial guess at neutron spectrum, shape is (m,)
    tolerance --> user-defined stopping condition
    """
    x = x.copy()
    n = R.shape[0]
    m = R.shape[1]
    # eliminate any channel with 0 count
    R = np.array([R[i] for i in range(n) if data[i] != 0])
    data = np.array([x for x in data if x > 0])
    # redefine number of rows after the reduction
    n = R.shape[0]
    J0 = 0 ; dJ0 = 1 ; ddJ = 1
    error = []
    stepcount = 1
    while ddJ > tolerance:
        W = np.zeros((n,m))
        rdot = np.zeros((n,))
        for i in range(n):
            rdot[i] = (R[i,:]@x)

        for j in range(m):

            W[:,j] = data*R[:,j]*x[j] / rdot
            num = np.dot(W[:,j],log(data/rdot))

            num = np.nan_to_num(num)
            den = sum(W[:,j])

            if den == 0:
                x[j] *= 1
            else:
                x[j] *= exp(num/den)

        J = sum((rdot-data)**2) / sum(rdot)
        dJ = J0-J
        ddJ = abs(dJ-dJ0)
        J0 = J
        error.append(ddJ)
        print("Iteration {}, ddJ = {}".format(stepcount,ddJ))
        stepcount += 1
        dJ0 = dJ

    return(x,np.array(error))
import numpy as np

# True neutron spectrum (A)
A = np.array([
    [5],
    [10],
    [15],
    [7],
    [3],
    [1]
])  # shape (6,1)

# Synthetic response matrix (B) – make sure it's well-conditioned
B = np.array([
    [0.4, 0.3, 0.2, 0.1, 0.0, 0.0],
    [0.2, 0.5, 0.2, 0.1, 0.0, 0.0],
    [0.1, 0.2, 0.5, 0.2, 0.0, 0.0],
    [0.0, 0.1, 0.3, 0.4, 0.1, 0.1],
    [0.0, 0.0, 0.1, 0.3, 0.4, 0.2],
    [0.0, 0.0, 0.0, 0.1, 0.3, 0.6]
])  # shape (6,6)

# Compute measured data (C)
C = B @ A  # shape (6,1)
C = (B @ A).flatten()
print(C)
bin_edges = np.array([1e-10, 1e-6, 0.1, 2.0, 4.0, 7.0, 11.0])
x2 = np.ones(len(bin_edges)-1)

testA, error = gravel(B, C, x2, 0.0000001)
print(testA)
#----------------------------------------
# Define energy bin edges (in MeV)
bin_edges = np.array([1e-10, 1e-6, 0.1, 2.0, 4.0, 7.0, 11.0])
x2 = np.ones(len(bin_edges)-1)
x3 = [1, 1, 1, 1, 1, 1 ]
#print(x2, R1)
tol = 0.001
#xg,errorg = gravel(R,N1,xtrue,tol)
xg,errorm = gravel(R1,N1,x3,tol)
print("xg: ", xg)
# Compute bin centers for labeling
bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])

# Create labels for x-axis
bin_labels = [
    "Slow",
    "Epithermal",
    "0.1-2",
    "2-4",
    "4-7",
    "7-11"
]
#print(xg)


Six_Binned_AmBe = np.array([0,0, 2151240, 2838782, 3478108, 1531870])
# Plot
print(np.abs(xg - Six_Binned_AmBe))
plt.figure(figsize=(10, 5))
plt.bar(range(6), xg, tick_label=bin_labels, width=1.0, align='center',color='#D62728', edgecolor='black', label="GRAVEL", alpha= 0.4)
plt.bar(range(6), Six_Binned_AmBe, tick_label=bin_labels, width=1.0, align='center', edgecolor='black',color='#ADD8E6', label="AmBe", alpha = 0.7)  # Add AmBe data
plt.tick_params(axis='x', labelsize=20)  # x-axis ticks
plt.tick_params(axis='y', labelsize=20) 
plt.xlabel("Energy Bins", fontsize= 20)
plt.ylabel("Neutron Counts", fontsize= 20)
plt.yscale('log') 
plt.ylim(1e4, 1e7)
#plt.title("Overlap comparison between GRAVEL and AmBe")
plt.grid(True, axis='y', linestyle='--', alpha=0.6)
plt.legend(frameon=False, fontsize= 22)
plt.tight_layout()
plt.show()

#for i in range(n):
    #print(R1[i,:]@xg)
sum_counts = 0
for i in range(6):
    sum_counts+=xg[i]
print("Total Predicted Counts: ", sum_counts)
sum_counts = 0
for i in range(6):
    sum_counts+=Six_Binned_AmBe[i]
print("Total actual Counts: ",sum_counts )


#-----------------------------------------
events= 10000000 
activity_neutrons = 63 # neutrons/second
active_time = events /activity_neutrons
CPS = N1/active_time # neutrons per second
Surface_Area_ThreeNCD = 9966 # cm^2
R_Effective= R*A

AmBe_TotalCounts =np.array([0,0,0,0,0,0,0,0,146823,154685, 150505, 140370, 136085, 127153, 111093, 121585, 111093, 105528, 957413, 1286300, 1552482, 1480083, 1131133, 866892, 841616, 344311, 246825, 99118,0])
AmBe_CPS= AmBe_TotalCounts/active_time

#flux,errorm = gravel(R_Effective,CPS,x,0.00000001)


AmBe_Flux= AmBe_CPS/Surface_Area_ThreeNCD




#----------------------------------------
tol = 0.0000001
#xg,errorg = gravel(R,N1,xtrue,tol)
AmBe_29 = np.array([0,0,0,0,0,0,0,0,146823,154685, 150505, 140370, 136085, 127153, 111093, 121585, 111093, 105528, 957413, 1286300, 1552482, 1480083, 1131133, 866892, 841616, 344311, 246825, 99118, 0])
xg,errorm = gravel(R,N1,x0,0.000000001) # x0
fig,ax = plt.subplots()
#ax.plot(xbins,(xtrue),color="k",label = "ToF Spectrum",alpha=0.3)
ax.plot(Energies,xg,label="GRAVEL")
ax.plot(Energies,AmBe_29,label="AmBe 29",color="red",alpha=0.5)
ax.grid(which="major",alpha=0.6)
ax.grid(which="minor",alpha=0.3)
plt.ylim(0,1e7)
plt.yscale('log')
ax.set_xlabel("Neutron Energy (MeV)")
#ax.set_ylabel("Neutron Counts")
ax.set_ylabel("Neutron Flux")
ax.set_yscale('log')
#zax.set_ylim(0,0.175)
#ax.xaxis.set_minor_locator(MultipleLocator(1))
plt.legend(frameon=False)
#plt.savefig("final-results/real-data-mlem-{}.png".format(suffix),dpi=300,bbox_inches="tight")
plt.show()
plt.close()

#----------------------------------------



#----------------------------------------

AmBe_29_CPS= AmBe_29/158730.15
xg_CPS = xg/158730.15

Absolute_error= np.abs(AmBe_29_CPS-xg_CPS)
print(Absolute_error)
for i in range(len(Absolute_error)):
    if Absolute_error[i] < (1e-9):
        Absolute_error[i] = 1e-4


# Example data
bin_centers = np.array([1e-10, 1e-9, 1e-8, 1e-7, 1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.225, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11])  # Replace with your actual bin centers

bin_edges = np.array([ (1e-9 +1e-10)/2 , (1e-8+1e-9)/2, (1e-7+1e-8)/2, (1e-6+1e-7)/2, (1e-5+1e-6)/2, (1e-4+1e-5)/2, (1e-3+1e-4)/2, (1e-2+1e-3)/2, (0.1+1e-2)/2, (0.2+0.1)/2, (0.3+0.2)/2, (0.4+0.3)/2, (0.5+0.4)/2, (0.6+0.5)/2, (0.7+0.6)/2, (0.8+0.7)/2, (0.9+0.8)/2, 0.95,1.225,  1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5])



# Use geometric midpoints between points to define edges
edges = np.sqrt(Energies[:-1] * Energies[1:])  # length 28

# Pad edges to include left and right limits
edges = np.concatenate((
    [Energies[0] / np.sqrt(Energies[1]/Energies[0])],  # left edge
    edges,
    [Energies[-1] * np.sqrt(Energies[-1]/Energies[-2])]  # right edge
))  # length 30
widths = edges[1:] - edges[:-1]
# Calculate bin width (assumes uniform spacing; adjust if needed)
if len(bin_centers) > 1:
    bin_width = np.min(np.diff(bin_centers))
else:
    bin_width = 1  # fallback

bin_widths = np.diff(bin_edges)  # length N
bin_widths[17] = 1.000e-01
bin_widths[18] = 0.55
print(bin_widths)
AmBe_29 = np.array([0,0,0,0,0,0,0,0,146823,154685, 150505, 140370, 136085, 127153, 111093, 121585, 111093, 105528, 957413, 1286300, 1552482, 1480083, 1131133, 866892, 841616, 344311, 246825, 99118, 0])

#print(xg)
#print(AmBe_29-xg)
print(errorm)
plt.bar(bin_centers, AmBe_29, width=bin_widths, align='center',color='#ADD8E6', edgecolor='blue', label="AmBe", alpha= 0.9)
plt.bar(bin_centers, (xg), width=bin_widths, align='center', edgecolor='black',color ='orange', label="GRAVEL", alpha = 0.5)
#plt.plot(Energies, AmBe_29_CPS, color ='orange', label="AmBe" )
#plt.plot(Energies, xg_CPS, color = 'blue', label = "GRAVEL")
#  Set custom ticks and labels
#plt.xticks(ticks=[1,2,3,4,5,6,7,8,9,10,11],
           #labels=['1','2','3','4','5','6','7','8','9','10','11'])

plt.xlabel("Energy bins (MeV)", fontsize=26)
#plt.xlabel("Energy (MeV)", fontsize=22)
#plt.ylabel("Counts per second", fontsize=22)
plt.ylabel("Counts", fontsize=22)
plt.legend(loc= 'upper right', fontsize = 26)
plt.yscale('log')
plt.ylim(0,1e7)
#plt.title("Histogram with Bin Centers as X-axis")
plt.grid(True, axis='y', linestyle='--', alpha=0.1)
plt.tight_layout()
plt.show()
sum_counts = 0




#-------------------------------------


Relative_error = np.divide(Absolute_error, AmBe_29_CPS, out=np.zeros_like(Absolute_error), where=AmBe_29!=0)
Relative_error_percentage = Relative_error*100
fig,ax = plt.subplots()
#ax.plot(xbins,(xtrue),color="k",label = "ToF Spectrum",alpha=0.3)
#ax.plot(Energies,Absolute_error,label="Abs. error")
#ax.plot(Energies,Relative_error_percentage,label="Rel. error (%)",color="orange")
#ax.plot(Energies,AmBe_29,label="AmBe 29",color="red",alpha=0.5)
ax.bar(bin_centers, Relative_error_percentage, width=bin_widths, align='center', edgecolor='black',color ='orange', label="GRAVEL", alpha = 0.7) 
ax.grid(which="major",alpha=0.6)
ax.grid(which="minor",alpha=0.3)
ax.set_xlabel("Neutron Energy (MeV)")
#ax.set_ylabel("Neutron Counts")
ax.set_ylabel("Absolute error % for GRAVEL results")
#ax.set_yscale('log')
#zax.set_ylim(0,0.175)
#ax.xaxis.set_minor_locator(MultipleLocator(1))
plt.legend(frameon=False)
#plt.savefig("final-results/real-data-mlem-{}.png".format(suffix),dpi=300,bbox_inches="tight")
plt.show()
plt.close()

Total_relativeerror = 0
for i in range(29):
    Total_relativeerror += Relative_error_percentage[i]

print(Total_relativeerror)

print(np.average(Relative_error_percentage))
