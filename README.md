# Trabalho 1 | Sistemas Operacionais 2022/2


##### Repositório com a implementação do escalonador de processos circular com critério Round Robin de quantum 5 para um sistema operacional.


&nbsp;

## Como funciona?

O escalonador implementa duas filas circulares de baixa e alta prioridade. Cada processo possui um quantum de 5 unidades de tempo. Existem três filas de dispositivos para atendimento de entrada e saída.

O programa pode atuar de forma aleatória ou com input via arquivo. Nesse último caso, basta fornecer o caminho para o arquivo via linha de comando.

A cada instante, são exibidos os status de cada fila de processos (alta prioridade e baixa prioridade) e de dispositivos. Bem como, quem encerrou, o tempo de serviço restante de cada processo, o número de solicitações de IO para serem realizadas e o quantum restante do processo na cabeça da fila. Também são exibidas informações sobre operações de IO.

O escalonador, no modo aleatório, atua num tempo de 10 minutos (definido na compilação) ou de forma indeterminada (até ser parado).
No modo de input do usuário, atua até os processos fornecidos via arquivo encerrarem.

No modo aleatório, existem apenas 5 processos executando no máximo. A cada instante pode surgir um com probabilidade  de 20%.

As operações de IO são de 3 tipos:

| Tipo      | Duração |
| ----------- | ----------- |
| 1 (Fita)      | 5       |
| 2 (Disco)   | 3        |
| 3 (Impressora)   | 7       |

**Observação: O processo em CPU é SEMPRE o cabeça da fila. Ele não é removido dela.**

## Formato da entrada

- Número de processos a serem executados

- De 0 até o número de processos a serem executados
    - Tempo de chegada
    - Tempo de serviço
    - Número de operações de IO (0 a n)
    - De 0 até o número de IOs a serem realizados
        - Tipo do IO
        - Tempo de chegada

    Exemplo:

    5
    <br>
    0 13 1 3 4
    <br>
    4 11 2 2 2 3 6
    <br>
    5 7 0
    <br>
    7 8 0
    <br>
    10 16 2 3 2 2 7

    Haverá 5 processos.

    P1: Chega no tempo 0, tem 13 segundos de tempo de serviço, irá fazer uma operação de E/S do tipo 3 no instante 4 de execução.

    P2: Chega no tempo 4, tem 11 segundos de tempo de serviço, irá fazer duas operações de E/S, uma do tipo 2 no instante 2 e uma do tipo 3 no instante 6.

    Os demais são análogos.


## Como compilar?

##### Baixe o código ou clone o repositório com:

```
$ git clone https://github.com/thiagoc01/trab1-sistemas-operacionais.git
```

##### Formas de compilação:

Se você deseja executar o programa de forma indefinida, digite:

```
$ make
```

Se você deseja executar o programa com limite de 10 minutos, digite:

```
$ make EXECUCAO_EM_10_MIN=1
```

Se você deseja que o escalonador aguarde uma tecla a cada instante, digite:

```
$ make PARADA_ITERACAO=1
```

Ambas as variáveis podem ser definidas juntas. Logo, ele executará em 10 minutos e aguardando uma tecla por iteração.

Para alterar o nome do executável, altere a variável TARGET para o nome desejado.

##### Execute o programa com:

- Modo aleatório

    ```
    $ ./escalonador
    ```
- Modo de entrada via arquivo

    ```
    $ ./escalonador <nome do arquivo>
    ```