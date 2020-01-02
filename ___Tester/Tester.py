import os
import datetime

# num_prove = 10
num_prove = 2

separatore_decimale_numero = ',' # Il mio excel è italiano e ci sono problemi se c'è il punto
tabulazione = ';' # col il punto e virgola il mio excel funziona bene
eseguibile_nome = "Project1.exe"

# istanze = [1, 2, 3, 4, 5, 6, 7, 8]
istanze = [2, 3, 6, 7, 8]

#tlims = [1, 2, 3, 4, 5, 10, 20, 30, 60, 120, 180, 240, 300, 600] # tempo espresso in secondi
tlims = [1, 5, 10, 60, 180] # tempo espresso in secondi
studenti = [611, 941, 1125, 4360, 5349, 21266, 2823, 2750]
benchmark_prof = [157.033, 34.709, 32.627, 7.717, 12.901, 3.045, 10.050, 24.769]

# Calcolo tempo di esecuzione
somma_tlim = 0
for t in tlims:
    somma_tlim += t

num_istanze = 0
for i in istanze:
    num_istanze += 1

tempo = somma_tlim * num_istanze * num_prove / 60.0**2 # tempo è espresso in ore
print("L'esecuzione del programma richiede " + str(tempo) + " ore. Procedere (Y|N)? ")
risp = 0
while risp != 'Y' and risp != 'N' and risp != 'y' and risp != 'n':
    risp = input()

if risp == 'N' or risp == 'n':
    quit()

# Creazione cartella
ora = str(datetime.datetime.now()).split('.')[0].replace(':','.')
parent = "Benchmark " + ora
os.makedirs(parent)
os.makedirs(parent + "/solutions")

###
for istanza_numero in istanze: # per ogni istanza che abbiamo
    istanza_nome = "instance0" + str(istanza_numero)
    
    file = open(parent + "/results_" + istanza_nome + ".csv", "w")
    output = ""
    
    for prova in range(-1, num_prove):
        if prova == -1:
            output += "tlim" + tabulazione
        else:
            output += "Prova " + str(prova) + tabulazione
        
        for t in range(len(tlims) + 1):
            if prova == -1:
                if t == len(tlims):
                    output += 'Benchmark\n'
                else:
                    output += str(tlims[t]) + tabulazione
            else:
                if t == len(tlims):
                    output += str(benchmark_prof[istanza_numero - 1]).replace('.',separatore_decimale_numero) + '\n'
                else:
                    # Project1.exe instance01 -t 300
                    comando = eseguibile_nome + " " + istanza_nome + " -t " + str(tlims[t])
                    print()
                    val = os.system(comando)
                    output += str(val / studenti[istanza_numero - 1]).replace('.',separatore_decimale_numero) + tabulazione
                    
                    # nome_file_vecchio = "instance01_DMOgroup01.sol"
                    # nome_file_nuovo = "solutions/instance01_tlim10_prova1.sol"
                    nome_file_vecchio = istanza_nome + "_DMOgroup01.sol"
                    nome_file_nuovo = parent + "/solutions/" + istanza_nome + "_tlim" + str(tlims[t]) + "_prova" + str(prova) + ".sol"
                    os.rename(nome_file_vecchio, nome_file_nuovo)
        # for t end
    # for prova end
    
    # stampa risultati
    file.write(output)
    file.close()
# for istanza end

print("\n\nFine.")