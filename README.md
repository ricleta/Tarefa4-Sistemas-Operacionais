# Tarefa4-Sistemas-Operacionais
1) Crie dois processos concorrentes, produtor e consumidor, que compartilham uma área de
trabalho (memória) executando um loop 128 vezes
    - Para sincronizar as suas ações, eles fazem uso de semáforos:
        - O processo produtor fica gerando caracteres (letras maiúsculas) colocando-os em um buffer de 32 posições
        - o processo consumidor deve imprimir os caracteres na saída padrão.
    - Garanta que ocorra concorrência entre os processos produtor e consumidor. __Mostre como isso foi comprovado.__
2) Crie dois processos concorrentes, produtor e consumidor, que trocam mensagens (que são as
mesmas descritas no item 1).
    - Faça programas distintos que utilizam o buffer para trocar
mensagens.
    - __Mostre como foi implementado o sincronismo e como isso foi comprovado.__
3) Discuta as diferenças verificadas para implementar o sincronismo entre os processos.