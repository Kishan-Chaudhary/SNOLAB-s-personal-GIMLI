from pickle import STOP
from tkinter import NO
import numpy as np

N_total = 1_000_000
N_response = 189

Stop = "NO"

while Stop != "Yes":
    Tritons = input("Please enter Triton counts: ")
    print("You entered:", Tritons)
    Tritons = int(Tritons)
    R =  Tritons/ N_total
    sigma_R = np.sqrt(Tritons) / N_total
    print(R,"\xB1", sigma_R)
    Stop = input("Do you want to stop? (Yes?)").upper()

   