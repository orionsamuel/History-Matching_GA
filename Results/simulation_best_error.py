import os
import math
import numpy as np
import pandas as pd
from math import sqrt
from matplotlib import pyplot as plt
from sklearn.metrics import r2_score
from sklearn.metrics import mean_squared_error
from sklearn.metrics import mean_absolute_error
from sklearn.metrics import explained_variance_score
from sklearn.metrics import mean_absolute_percentage_error

def init(n):
    simulation(n)

def simulation(n):
    if(not os.path.exists("Output_Simulation")):
        os.system("mkdir Output_Simulation")
    else:
        os.system("rm -f Output_Simulation/*")

    os.system("rm -f output_simulation")
    for i in range(n):
        print("Executando a simulação: "+str(i))
        os.system("cp ../Output/0/inputDS_"+str(i)+".dat "
                  +"../../Codigo_Bifasico_Slab/simulacoes/dev/inputDS.dat")
        os.system("./../../Codigo_Bifasico_Slab"
                  +"/rodarSimulador.sh >> output_simulation 2>>"
                  +"output_simulation")
        os.system("cp ../../Codigo_Bifasico_Slab/simulacoes/dev/out/"
                  +"resultadoVazaoAgua.dat Output_Simulation/"
                  +"vazaoAgua_"+str(i)+".dat")
        os.system("cp ../../Codigo_Bifasico_Slab/simulacoes/dev/out/"
                  +"resultadoVazaoOleo.dat Output_Simulation/"
                  +"vazaoOleo_"+str(i)+".dat")


if __name__ == '__main__':
    init(100)




