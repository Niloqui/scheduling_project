import os
import datetime
import time
import shutil

# num_prove = 10
num_prove = 5

separatore_decimale_numero = ',' # Il mio excel è italiano e ci sono problemi se c'è il punto
tabulazione = ';' # col il punto e virgola il mio excel funziona bene
eseguibile_nome = "Project1.exe"

# istanze = [1, 2, 3, 4, 5, 6, 7, 8]
istanze = [2, 3, 4]

#tlims = [0, 1, 2, 3, 4, 5, 10, 20, 30, 60, 120, 180, 240, 300, 600] # tempo espresso in secondi #### vettore originale
#tlims = [0, 10, 30, 60, 120, 180, 240, 300, 600] # tempo espresso in secondi
tlims = [0, 10, 30, 60, 120, 180, 240, 300] # tempo espresso in secondi
studenti = [611, 941, 1125, 4360, 5349, 21266, 2823, 2750]
benchmark_prof = [157.033, 34.709, 32.627, 7.717, 12.901, 3.045, 10.050, 24.769]

### Calcolo tempo di esecuzione
num_istanze = 0
for i in istanze:
    num_istanze += 1

tempo = tlims[-1] * num_istanze * num_prove / 60.0**2 # tempo è espresso in ore
minuti = int(tempo * 60) % 60
ore = int(tempo)
print("L'esecuzione del programma richiede " + str(ore) + " ore e " + str(minuti) + " minuti. Procedere (Y|N)? ")
risp = 0
while risp != 'Y' and risp != 'N' and risp != 'y' and risp != 'n':
    risp = input()

if risp == 'N' or risp == 'n':
    quit()

### Creazione cartella
ora = str(datetime.datetime.now()).split('.')[0].replace(':','.')
parent = "Benchmark " + ora
os.makedirs(parent)
os.makedirs(parent + "/solutions")

###
for istanza_numero in istanze: # per ogni istanza che abbiamo
    istanza_nome = "instance0" + str(istanza_numero)
    # nome_soluzione = "instance01_DMOgroup01.sol"
    nome_soluzione = istanza_nome + "_DMOgroup01.sol"
    
    ### Calcolo delle soluzioni
    for prova in range(num_prove):
        t_da_copiare = []
        # Project1.exe instance01 -t 300
        comando = "start " + eseguibile_nome + " " + istanza_nome + " -t " + str(tlims[-1])
        os.system(comando)
        
        for t in range(1, len(tlims)):
            time.sleep(tlims[t] - tlims[t-1])
            
            if os.path.isfile(nome_soluzione):
                # nome_file_nuovo = "solutions/instance01_tlim10_prova1.sol"
                nome_nuovo_per_soluzione = parent + "/solutions/" + istanza_nome+ "_prova" + str(prova) + "_tlim" + str(tlims[t])  + ".sol"
                os.rename(nome_soluzione, nome_nuovo_per_soluzione)
            else:
                t_da_copiare.append(t)
        # for t end
        
        for t in t_da_copiare:
            nome_sol_mancante = parent + "/solutions/" + istanza_nome+ "_prova" + str(prova) + "_tlim" + str(tlims[t])  + ".sol"
            nome_sol_precedente = parent + "/solutions/" + istanza_nome +"_prova" + str(prova) +  "_tlim" + str(tlims[t-1]) + ".sol"
            shutil.copyfile(nome_sol_precedente, nome_sol_mancante)
        # for t end
    # for prova end
    
    file = open(parent + "/results_" + istanza_nome + ".csv", "w")
    output = ""
    
    for prova in range(-1, num_prove):
        if prova == -1:
            output += "tlim" + tabulazione
        else:
            output += "Prova " + str(prova) + tabulazione
        
        for t in range(1, len(tlims) + 1):
            if prova == -1:
                if t == len(tlims):
                    output += 'Benchmark\n'
                else:
                    output += str(tlims[t]) + tabulazione
            else:
                if t == len(tlims):
                    output += str(benchmark_prof[istanza_numero - 1]).replace('.',separatore_decimale_numero) + '\n'
                else:
                    # MyCalculatePenalty.exe instance01 "Benchmark 2020-01-09 22.00.56/solutions/instance07_prova1_tlim10"
                    comando = "MyCalculatePenalty.exe " + istanza_nome + " "
                    comando += '\"' + parent + "/solutions/" + istanza_nome + "_prova" + str(prova) + "_tlim" + str(tlims[t]) + '\"'
                    val = os.system(comando)
                    output += str(val / studenti[istanza_numero - 1]).replace('.',separatore_decimale_numero) + tabulazione
        # for t end
    # for prova end
    
    # stampa risultati
    file.write(output)
    file.close()
# for istanza end

