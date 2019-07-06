import sys
import threading

NUM_THREADS = 0
PRODUCT = "2" * 30

_production = None
_locks = None

def main():

    # Verifica argumentos
    try:
        if len(sys.argv) != 2 or not isinstance(int(sys.argv[1]), int):
            print("Usage: python lock_prodcons.py <num_threads>")
            return
    except ValueError:
        print("Usage: python lock_prodcons.py <num_threads>")
        return

    # Inicializa argumentos passados por linha de comando
    global _production, _locks, NUM_THREADS
    NUM_THREADS = int(sys.argv[1])

    # Inicializa o vetor de producao
    _production = [""] * NUM_THREADS
    _locks = [threading.Lock()] * NUM_THREADS

    # Cria threads de produtores e consumidores e locks
    producerThreads = []
    consumerThreads = []
    for i in range(NUM_THREADS):
        print("Starting threads %d" % i)
        producerThreads.append(threading.Thread(target=producer, args=(i,)))
        consumerThreads.append(threading.Thread(target=consumer, args=(i,)))

    # Inicia threads
    for i in range(NUM_THREADS):
        producerThreads[i].start()
        consumerThreads[i].start()

    # Espera ate o fim da execucao de todas as threads
    for i in range(NUM_THREADS):
        producerThreads[i].join()
        consumerThreads[i].join()


def consumer(index: int):
    """
    Consumidor para uma posicao especifica do vetor.
    """
    global _production, _locks, PRODUCT

    cont = 0
    readBuffer = ""
    while cont < len(PRODUCT):
        # Pega o lock para fazer alteracoes no vetor de producao
        if _locks[index].acquire():
            # Le e atualiza o buffer
            if _production[index] != "":
                readBuffer += _production[index]
                _production[index] = ""
                cont += 1
            # Libera o lock
            _locks[index].release()

    # Verifica se pa operação deu certo
    if cont == len(PRODUCT):
        print("Consumer %d - SUCCESS" % index)
    else:
        print("Consumer %d - FAILURE" % index)

    return


def producer(index: int):
    """
    Produtor para uma posicao especifica do vetor
    """
    global _production, _locks, PRODUCT

    cont = 0
    writeBuffer = ""
    while cont < len(PRODUCT):
        # Pega o lock para fazer alteracoes no vetor de producao
        if _locks[index].acquire():
            # Le e atualiza o buffer
            if _production[index] == "":
                _production[index] = PRODUCT[cont]
                writeBuffer += PRODUCT[cont]
                cont += 1
            # Libera o lock
            _locks[index].release()

    # Verifica se pa operação deu certo
    if cont == len(PRODUCT):
        print("Producer %d - SUCCESS" % index)
    else:
        print("Producer %d - FAILURE" % index)

    return


if __name__ == "__main__":
    main()
