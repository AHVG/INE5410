from time import sleep
from random import randint
from threading import Thread, Semaphore

def produtor():
  global buffer
  for i in range(10):
    sleep(randint(0,2))           # fica um tempo produzindo...
    # verifica se há lugar no buffer
    sem_produzir.acquire()
    item = 'item ' + str(i)
    buffer.append(item)
    print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
    sem_consumir.release()

def consumidor():
  global buffer
  for i in range(10):
    # aguarda que haja um item para consumir 
    sem_consumir.acquire()
    item = buffer.pop(0)
    print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
    sem_produzir.release()
    sleep(randint(0,2))         # fica um tempo consumindo...

buffer = []
tam_buffer = 3
# cria semáforos
sem_produzir = Semaphore(1)
sem_consumir = Semaphore(0)
produtor = Thread(target=produtor) 
consumidor = Thread(target=consumidor) 
produtor.start()
consumidor.start()
produtor.join()
consumidor.join() 