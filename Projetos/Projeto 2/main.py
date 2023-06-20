import os
import argparse

from multiprocessing import Process

def work_process(sudokus):
    sudokus_blocks = [[] for _ in sudokus]

def work_threads(blocks):
    pass

def pos_int(value):
    pos_i = int(value)
    if pos_i < 1:
        msg = "Valor recebido %s. Tente um valor > 0!" % value
        raise argparse.ArgumentTypeError(msg)
    return pos_i

def valid_file(file):
    if not os.path.exists(file):
        msg = "Valor recebido %s. Arquivo nao existe!" % file
        raise argparse.ArgumentTypeError(msg)
    return file

# Definindo os parametros do programa
parser = argparse.ArgumentParser(add_help=True, description='Verificador de Sudoku Concorrente em Python')

parser.add_argument('-f', '--file-name', action='store', type=valid_file, required=True, help='O nome do arquivo com as solucoes a serem validadas')
parser.add_argument('-p', '--num-process', action='store', type=pos_int, required=True, help='O numero de processos trabalhadores')
parser.add_argument('-t', '--num-threads', action='store', type=pos_int, required=True, help='O numero de threads de correcao a serem utilizadas por cada processo trabalhador')

# Tratando eventuais erros de entrada
try:
    args = parser.parse_args()
except Exception as e:
    print(e)
    exit(1)

# Lendo as matrizes de sudokus
sudokus = []
with open(args.file_name) as file:
    sudokus = [[[int(e) for e in line] for line in sudoku.split("\n")] for sudoku in file.read().split("\n\n")]

# Fazendo a divisao de trabalho das threads
process_sudokus = [[] for _ in range(args.num_process)]
[process_sudokus[i % args.num_process].append(sudoku) for i, sudoku in enumerate(sudokus)]

# Iniciando os processos
process = [Process(target=work_process, args=(p_s,)) for p_s in process_sudokus]
for p in process:
    p.start()

# Esperando terminar os processos
for p in process:
    p.join()