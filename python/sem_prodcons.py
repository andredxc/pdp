import threading

NUM_THREADS = 16
PRODUCT = "2" * 30

_production = None
_semaphores = None

def main():

    # Inicializa o vetor de producao
    global _production, _semaphores, NUM_THREADS
    _production = [""] * NUM_THREADS
    _semaphores = [threading.Semaphore()] * NUM_THREADS

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
    global _production, _semaphores, PRODUCT

    cont = 0
    readBuffer = ""
    while cont < len(PRODUCT):
        # Pega o lock para fazer alteracoes no vetor de producao
        if _semaphores[index].acquire():
            # Le e atualiza o buffer
            if _production[index] != "":
                readBuffer += _production[index]
                _production[index] = ""
                cont += 1
            # Libera o lock
            _semaphores[index].release()

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
    global _production, _semaphores, PRODUCT

    cont = 0
    writeBuffer = ""
    while cont < len(PRODUCT):
        # Pega o lock para fazer alteracoes no vetor de producao
        if _semaphores[index].acquire():
            # Le e atualiza o buffer
            if _production[index] == "":
                _production[index] = PRODUCT[cont]
                writeBuffer += PRODUCT[cont]
                cont += 1
            # Libera o lock
            _semaphores[index].release()

    # Verifica se pa operação deu certo
    if cont == len(PRODUCT):
        print("Producer %d - SUCCESS" % index)
    else:
        print("Producer %d - FAILURE" % index)

    return


if __name__ == "__main__":
    main()
