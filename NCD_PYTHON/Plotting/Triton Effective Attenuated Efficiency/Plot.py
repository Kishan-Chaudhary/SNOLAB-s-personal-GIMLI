#from lib2to3.pytree import convert
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from statistics import mean
from datetime import datetime, date, time, timedelta
import math
from scipy import interpolate



data = pd.read_csv("3NCD_Data2.csv", encoding='latin1')
data2 = pd.read_csv("3NCD_HighResolutionData.csv", encoding='latin1')
df = pd.DataFrame(data)
df2 = pd.DataFrame(data2)
""" For 29 response values
#print(df.head())
SH1 = df.iloc[2:31,3].to_numpy()
SH1ERR = df.iloc[2:31,4].to_numpy()
print(SH1,"SH1")
SH2 = df.iloc[2:31,9].to_numpy()
SH2ERR = df.iloc[2:31,10].to_numpy()
print(SH2,"SH2")
SH3 = df.iloc[2:31,15].to_numpy()
SH3ERR = df.iloc[2:31,16].to_numpy()
print(SH3,"SH3")
SH4 = df.iloc[2:31,21].to_numpy()
SH4ERR = df.iloc[2:31,22].to_numpy()
print(SH4,"SH4")
SH5 = df.iloc[2:31,27].to_numpy()
SH5ERR = df.iloc[2:31,28].to_numpy()
print(SH5,"SH5")
#print(SH2,SH3,SH4,SH5)  

SH6 = df.iloc[2:31,33].to_numpy()
SH6ERR = df.iloc[2:31,34].to_numpy()
print(SH6,"SH6")
# Extracting the energies from the DataFrame
Energies =  df.iloc[2:31,0].to_numpy()
print(Energies)
"""
#print(df.head())
SH1 = df2.iloc[2:70,3].to_numpy()
SH1ERR = df2.iloc[2:70,4].to_numpy()
print(SH1,"SH1")
SH2 = df2.iloc[2:70,9].to_numpy()
SH2ERR = df2.iloc[2:70,10].to_numpy()
print(SH2,"SH2")
SH3 = df2.iloc[2:70,15].to_numpy()
SH3ERR = df2.iloc[2:70,16].to_numpy()
print(SH3,"SH3")
SH4 = df2.iloc[2:70,21].to_numpy()
SH4ERR = df2.iloc[2:70,22].to_numpy()
print(SH4,"SH4")
SH5 = df2.iloc[2:70,27].to_numpy()
SH5ERR = df2.iloc[2:70,28].to_numpy()
print(SH5,"SH5")
#print(SH2,SH3,SH4,SH5)  

SH6 = df2.iloc[2:70,33].to_numpy()
SH6ERR = df2.iloc[2:70,34].to_numpy()
print(SH6,"SH6")
# Extracting the energies from the DataFrame
Energies =  df2.iloc[2:70,0].to_numpy()
#print(Energies)


# Converting to numpy arrays for easier manipulation
Energies = np.array(Energies, dtype=float)
SH1 = np.array(SH1, dtype=float)
SH1ERR = np.array(SH1ERR, dtype=float)
#print(SH1ERR)
SH2 = np.array(SH2, dtype=float)
SH2ERR = np.array(SH2ERR,dtype=float)

SH3 = np.array(SH3, dtype=float)
SH3ERR = np.array(SH3ERR, dtype=float)
print(SH3ERR)
SH4 = np.array(SH4, dtype=float)
SH4ERR = np.array(SH4ERR, dtype=float)

SH5 = np.array(SH5, dtype=float)
SH5ERR = np.array(SH5ERR, dtype=float)

SH6 = np.array(SH6, dtype=float)
SH6ERR = np.array(SH6ERR, dtype=float)



interpolateSH1 = interpolate.interp1d(Energies, SH1, kind='cubic')
interpolatesmoothSH1 = interpolate.UnivariateSpline(Energies, SH1)

interpolateSH2 = interpolate.interp1d(Energies, SH2, kind='cubic')
interpolatesmoothSH2 = interpolate.UnivariateSpline(Energies, SH2)

interpolateSH3 = interpolate.interp1d(Energies, SH3, kind='cubic')
interpolatesmoothSH3 = interpolate.UnivariateSpline(Energies, SH3)

interpolateSH4 = interpolate.interp1d(Energies, SH4, kind='cubic')
interpolatesmoothSH4 = interpolate.UnivariateSpline(Energies, SH4)

interpolateSH5 = interpolate.interp1d(Energies, SH5, kind='cubic')
interpolatesmoothSH5 = interpolate.UnivariateSpline(Energies, SH5)


interpolateSH6 = interpolate.interp1d(Energies, SH6, kind='cubic')
interpolatesmoothSH6 = interpolate.UnivariateSpline(Energies, SH6)
# Number of different data series
num_series = 6
print(Energies[0:10])
# Get a colormap with enough colors
colors = plt.get_cmap('tab20', num_series)

#SH6=0.5*SH6 # Scaling the bare data to match the others
# Plotting the data
#plt.figure(figsize=(20, 10))
#plt.errorbar(Energies, SH1, yerr=SH1ERR, fmt='o', color='k',label="1 inch PE", elinewidth=2, capsize=5, markersize=2)
plt.errorbar(Energies[20:40],SH3[20:40],SH3ERR[20:40], marker='.', color='k', label="1 inch PE")
#plt.errorbar(Energies[0:10],SH2[0:10],SH2ERR[0:10], marker='.', color='r', label="2 inch PE")
#plt.errorbar(Energies[0:10],SH3[0:10],SH3ERR[0:10], marker='.', color='b', label="3 inch PE")
#plt.errorbar(Energies[0:10],SH4[0:10],SH4ERR[0:10], marker='.', color='g', label="4 inch PE")
#plt.errorbar(Energies[0:10],SH5[0:10],SH5ERR[0:10], marker='.', color='c', label="5 inch PE")
#plt.errorbar(Energies[0:5],SH6[0:5],SH6ERR[0:5], marker='.', color=colors(10), label="Bare")
plt.title("GEANT4 3NCD Comparison",fontsize=20)
plt.xlabel("Energy (MeV)",fontsize=22, color='black')
plt.ylabel("Atteunated Efficiency Probability",fontsize=22, color='black')
plt.xticks(rotation=40, fontsize=22,color='black')
plt.yticks(fontsize=22,color='black')
plt.legend(loc='upper right',fontsize=19)
plt.show()



plt.errorbar(Energies,SH1,SH1ERR, marker='.', color='k', label="1 inch PE", linewidth=3)
plt.errorbar(Energies,SH2,SH2ERR, marker='.', color='r', label="2 inch PE", linewidth=3)
plt.errorbar(Energies,SH3,SH3ERR, marker='.', color='b', label="3 inch PE", linewidth=3)
plt.errorbar(Energies,SH4,SH4ERR, marker='.', color='g', label="4 inch PE", linewidth=3)
plt.errorbar(Energies,SH5,SH5ERR, marker='.', color='c', label="5 inch PE", linewidth=3)
plt.errorbar(Energies,SH6,SH6ERR, marker='.', color='#C71585', label="Bare", linewidth=3)
#plt.plot(Energies,interpolatesmoothSH1(Energies), marker='.', color=colors(0), label="ip 1 inch PE")
#plt.plot(Energies,interpolatesmoothSH2(Energies), marker='.', color=colors(1), label="ip 2 inch PE")
#plt.plot(Energies,interpolatesmoothSH3(Energies), marker='.', color=colors(2), label="ip 3 inch PE")
#plt.plot(Energies,interpolatesmoothSH4(Energies), marker='.', color=colors(3), label="ip Full")
#plt.plot(Energies,interpolatesmoothSH4(Energies), marker='.', color=colors(4), label="ip Bare")

#plt.title("GEANT4 3NCD Response Linear Plot",fontsize=20)
plt.xlabel("Neutron's initial Energy(MeV, log scale)",fontsize=20, color='black')
plt.ylabel("Response (Probability)",fontsize=20, color='black')
plt.xticks(rotation=40, fontsize=15,color='black')
plt.yticks(fontsize=15,color='black')
plt.legend(loc='upper right', fontsize=20)
plt.tight_layout()
plt.xscale('log')
plt.show()
#plt.savefig('Plot', bbox_inches='tight')