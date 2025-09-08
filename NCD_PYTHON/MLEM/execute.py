import numpy as np
import pandas as pd
import seaborn as sns
from mlem import mlem
import matplotlib.pyplot as plt
#from response_matrix import response_matrix
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)

sns.set_theme()
sns.set_style('ticks')
plt.rc('font', family='serif')

def execute(initial_guess):
    """
    initial_guess --> 0 for constant and 1 for ToF initial guess
    """
    data = pd.read_csv("3NCD_Data2.csv", encoding='latin1')
    df = pd.DataFrame(data)

    R=[df.iloc[2:31,3].to_numpy(),
       df.iloc[2:31,9].to_numpy(),
       df.iloc[2:31,15].to_numpy(),
       df.iloc[2:31,21].to_numpy(),
       df.iloc[2:31,27].to_numpy(),
       df.iloc[2:31,33].to_numpy()
       ]

    R = np.array(R, dtype=float)

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
    #----------------------------------------
    # Execute the two algorithms
    #----------------------------------------
    if initial_guess == 0:
        tol = 0.2
        #xg,errorg = gravel(R,N1,x,tol)
        xm,errorm = mlem(R,N1,x,tol)
        suffix = "constant"
    else:
        tol = 0.1
        #xg,errorg = gravel(R,N1,xtrue,tol)
        xm,errorm = mlem(R,N1,x0,tol)
        suffix = "true"

    #fig,ax = plt.subplots()
    #-----------------------------------------
    # Normalize the vectors
    #-----------------------------------------
    #Lt = np.linalg.norm(xtrue) ; xtrue = xtrue/Lt
    #Lg = np.linalg.norm(xg); xg = xg/Lg
    #Lm = np.linalg.norm(xm); xm = xm/Lm
   
    #-----------------------------------------
    # plot MLEM results against ToF
    #-----------------------------------------
    fig,ax = plt.subplots()
    #ax.plot(xbins,(xtrue),color="k",label = "ToF Spectrum",alpha=0.3)
    ax.plot(Energies,(xm),label="MLEM")
    ax.grid(which="major",alpha=0.6)
    ax.grid(which="minor",alpha=0.3)
    ax.set_xlabel("Neutron Energy (MeV)")
    ax.set_ylabel("Normalized Counts")
    #zax.set_ylim(0,0.175)
    ax.xaxis.set_minor_locator(MultipleLocator(1))
    plt.legend(frameon=False)
    #plt.savefig("final-results/real-data-mlem-{}.png".format(suffix),dpi=300,bbox_inches="tight")
    plt.show()
    plt.close()

    #--------------------------------------------------
    # relative error between ToF, GRAVEL and MLEM
    #--------------------------------------------------
    fig,ax = plt.subplots()
    #--------------------------------------------------
    # remove all instances of 0 before calculating relative error
    #--------------------------------------------------
    xbins = np.array([xbins[i] for i in range(m) if xtrue[i] != 0])
    #xg = np.array([xg[i] for i in range(m) if xtrue[i] != 0])
    xm = np.array([xm[i] for i in range(m) if xtrue[i] != 0])
    xtrue = np.array([x for x in xtrue if x != 0])
    diff_g = abs(xtrue-xg) / xtrue; diff_m = abs(xtrue-xm) / xtrue
    np.savetxt("final-results/relative-norm-{}.txt".format(suffix), [np.linalg.norm(diff_g),np.linalg.norm(diff_m)])
    #-----------------------------------------
    # plot error versus iteration
    #-----------------------------------------
    fig,ax = plt.subplots()
    #ax.plot(np.arange(len(errorg)),errorg,color ="k",label="GRAVEL")
    ax.plot(np.arange(len(errorm)),errorm,label="MLEM")
    ax.set_ylim(-5,15)
    ax.axhline(tol,color="r",linewidth=0.75)
    ax.grid(which="major",alpha=0.6)
    ax.grid(which="minor",alpha=0.3)
    ax.xaxis.set_minor_locator(MultipleLocator(10))
    ax.set_ylabel("$\Delta$J")
    ax.set_xlabel("Iteration")
    text = "$\Delta$J$_{stop}$"
    fig.text(0.70,0.15,text+" = {}".format(tol))
    ax.legend(frameon=False)

    plt.savefig("final-results/error-{}.png".format(suffix),dpi=300,bbox_inches="tight")
    plt.close()

    #-----------------------------------------
    # plot a bar chart showing the norms of
    # the relative error vectors
    #-----------------------------------------
    truenorm = np.loadtxt("final-results/relative-norm-true.txt")
    xgtrue,xmtrue = truenorm[0],truenorm[1]

    constnorm = np.loadtxt("final-results/relative-norm-constant.txt")
    xgconst,xmconst = constnorm[0],constnorm[1]

    fig,ax = plt.subplots(figsize=(6,4))

    ax.bar([1,2],[xgtrue,xmtrue],width = 0.35, label="a priori",edgecolor="black")
    ax.bar([3,4],[xgconst,xmconst],width = 0.35,label = "constant",edgecolor="black")

    for bar in ax.patches:
        bar_value = bar.get_height()
        text = '{:.2f}'.format(bar_value)
        text_x = bar.get_x() + bar.get_width() / 2
        text_y = (bar.get_y() + bar_value)
        ax.text(text_x, text_y, text, ha='center', va='bottom')

    plt.xticks([1,2,3,4],["GRAVEL", "MLEM", "GRAVEL", "MLEM"])
    plt.ylabel("Relative Error")
    plt.ylim(0,12)
    plt.legend(frameon=False,loc="upper left")
    plt.savefig("final-results/relative-error.png",dpi=300,bbox_inches="tight")
    plt.close()

#-----------------------------------------
# execute takes one input which tells it
# which initial guess to use
#-----------------------------------------
constant = 0 ; tof = 1
execute(0)
