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
    #read_files_simulation("../../Output/344/344.csv", n)
    simulation(n)
    '''create_dataset(n)
    objective_function(n)
    target_test, target_pred = read_dataset("real_simulation.csv",
                                            "../../Output/344/344.csv")
    calc_error(target_test, target_pred)
    plot_chart(target_test, target_pred)'''

def read_files_simulation(path, n):
    dataset = pd.read_csv(path)

    if(not os.path.exists("Input_Simulation")):
        os.system("mkdir Input_Simulation")
    else:
       os.system("rm -f Input_Simulation/*")
       
    for i in range(n):
        inputFile = open("../../Input_Simulation_Files/inputDS_"+str(i)+".dat", "r")
        outputFile = open("inputDS_"+str(i)+".dat", "w")
        count = 0;
        for line in inputFile:
            if(count == 142):
                found = line.split(" ")
                outputFile.write("         " +found[9]+"   "
                                +str(f'{dataset["Porosity"][i]:e}')
                                +"   "+str(f'{dataset["Permeability_1"][i]:e}')
                                +"   "+str(f'{dataset["Permeability_2"][i]:e}')
                                +"   "+str(f'{dataset["Permeability_3"][i]:e}')+"   "
                                +found[24]+"   "+found[27]+"   "+found[30]
                                +"   "+found[33]+"   "+found[36])
            else:
                outputFile.write(line)

            count = count + 1
            
        outputFile.close()
        inputFile.close()

        os.system("mv inputDS_"+str(i)+".dat Input_Simulation/inputDS_"
                  +str(i)+".dat")

def simulation(n):
    if(not os.path.exists("Output_Simulation")):
        os.system("mkdir Output_Simulation")
    else:
        os.system("rm -f Output_Simulation/*")

    os.system("rm -f output_simulation")
    for i in range(n):
        print("Executando a simulação: "+str(i))
        os.system("cp ../Output/14/inputDS_"+str(i)+".dat "
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

def create_dataset(n):
    outputFile = open("real_simulation.csv", "w")
    outputFile.write("Porosity,Permeability_1,Permeability_2,Permeability_3"
                         +",Error\n")

    error_result = objective_function(n)
    
    for i in range(n):
        inputFile = open("Input_Simulation/inputDS_"+str(i)+".dat", "r")                         
        count = 0;
        for line in inputFile:
            if(count == 142):
                found = line.split(" ")
                outputFile.write(str(found[12])+","+str(found[15])+","
                                 +str(found[18])+","+str(found[21])+","
                                 +str('{:e}'.format(error_result[i]))+"\n")

            count = count + 1

        inputFile.close()

    outputFile.close()

def objective_function(n):
    water_result = []
    realWater = []
    oil_result = []
    realOil = []
    error_result = []
    
    real_result_water = open("../../Input/resultadoVazaoAgua.dat", "r")
    real_result_oil =  open("../../Input/resultadoVazaoOleo.dat", "r")

    for line in real_result_water:
        found = line.split(" ")
        realWater.append(float(found[3]))

    for line in real_result_oil:
        found = line.split(" ")
        realOil.append(float(found[3]))

    real_result_water.close()
    real_result_oil.close()
    
    for i in range(n):
        water = []
        oil = []
        
        inputFile_Water = open("Output_Simulation/vazaoAgua_"
                               +str(i)+".dat", "r")
        inputFile_Oil = open("Output_Simulation/vazaoOleo_"
                             +str(i)+".dat", "r")
        for line in inputFile_Water:
            found = line.split(" ")
            water.append(float(found[3]))

        water_result.append(water)

        inputFile_Water.close()

        for line in inputFile_Oil:
            found = line.split(" ")
            oil.append(float(found[3]))

        oil_result.append(oil)
        inputFile_Oil.close()

        rank = 0
        for j in range(2):
            if(j == 0):
                for k in range(len(realWater)):
                    rank = rank + math.pow((realWater[k] - water_result[i][k]),2)
                rank = rank * 0.6
            elif(j == 1):
                for k in range(len(realOil)):
                    rank = rank + math.pow((realOil[k] - oil_result[i][k]),2)
                rank = rank * 0.4

        rank = math.sqrt(rank / (len(realWater) * 2))

        error_result.append(rank)
    return error_result

def read_dataset(path1, path2):
    dataset = pd.read_csv(path1)

    target_test = dataset.iloc[:,-1].values

    dataset = pd.read_csv(path2)

    target_pred = dataset.head(100).iloc[:,-1].values

    return target_test, target_pred

def calc_error(target_test, target_pred):
    print("MAPE Error: ", mean_absolute_percentage_error(target_test, target_pred))
    print("MAE Error: ", '{:e}'.format(mean_absolute_error(target_test, target_pred)))
    print("MSE Error: ", '{:e}'.format(mean_squared_error(target_test, target_pred)))
    print("RMSE Error: ", '{:e}'.format(sqrt(mean_squared_error(target_test, target_pred))))
    print("R² Error: ", r2_score(target_test, target_pred))
    print("Explained Variance Error: ", explained_variance_score(target_test, target_pred))

def plot_chart(target_test, target_pred):
    instancias = []

    for i in range(len(target_test)):
        instancias.append(i) 

    plt.title("Error History Matching")
    plt.ylabel("Error Rate (Log Scale)")
    plt.xlabel("Instancies")
    plt.yscale('log')
    #plt.xscale('log')

    plt.plot(instancias, target_pred, color='purple', label ='Predict')
    plt.plot(instancias, target_test, color='red', label ='Real')
    plt.legend(loc = 'upper right')
    #plt.show()
    plt.savefig("History Matching - Linhas.png")

if __name__ == '__main__':
    init(1)




