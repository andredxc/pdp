# Trabalho final de PDP
**André Carneiro** e **Érico Moura**

O algoritmo, implementado com locks e semáforos em cada uma das três linguagens
diferentes, é uma versão ligeiramente modificada do produtor consumidor em que
cada par produtor-consumidor tem acesso a um índice do vetor de produção.

A ideia é focar na diferença de desempenho entre as diferentes linguagens e
mecanismos.

Com os resultados obtidos nos testes em duas diferentes máquinas (uma com um
processador Intel Core i5-3330 e a outra, Intel Core i7-7700K ) podemos
observar, entre outros:

1. **Lock é mais eficiente que semáforo, mas é menos poderoso**
Isso se deve ao fato de que o lock é a funcionalidade básica de exclusão utilizada por baixo do semáforo.
2. **C é mais rápido que Java e ambos são muito mais rápidos que Python**
C é uma linguagem de baixo nível compilada, o que a confere a melhor performance, Java é compilada em duas etapas e tem um nível de abstração mais alto, portanto troca uma parte da performance por produtividade enquanto Python é interpretada e de alto nível, o que a confere o pior desempenho entre todas as avaliadas.

## Intruções para a execução
Os códigos se encontram dentro da pasta de sua respectiva linguagem e nomeados de acordo com o mecanismo utilizado.
1. **Java**: compilar o arquivo .java com javac e executar o resultado com o comando java
2. **Python**: executar o código fonte com "python <arquivo.py>"
3. **C**: executar o comando "make" dentro do diretório cria os executáveis "lock" e "sem"
Todos os executáveis podem ser executados com o número de pares produtor-consumidor como parâmetro na linha de comando.
