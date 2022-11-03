
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#define EVER ; ;
#define LIM 32		// Para limitar o tamanho da fila 
#define ELEM 128	// Quantidade de mensagens a serem trocadas

typedef struct msgbuf 
{
         long    mtype;
         char mNum;
} message_buf;

void main(void)
{
    int msgID, id;
	msgID = msgget(0xBEEF, 0666 | IPC_CREAT);	// Cria fila de mensagens e guarda o ID da fila em msgID
	if(msgID == -1)
	{
		printf("Erro na criação da Fila de Mensagens\n");
	}
	struct msqid_ds aux;		
	message_buf buffer;		// Cria buffer para receber a mensagem
	int res;
	for(EVER)		// Os consumidores estão sempre esperando para consumir algo
	{
		res = msgrcv(msgID, &buffer, sizeof(buffer.mNum), 0, 0);	// Tira mensagem da fila e coloca no buffer 
		/* Lê a primeira mensagem (de qualquer tipo) da fila. Bloqueia caso não tenha mensagem na fila */
		if(res == -1) 
		{ 
			perror("Erro ao receber mensagem"); 
		}
		if(buffer.mtype == 2)	// Se a mensagem for do tipo 2, é porque o produtor parou de produzir
		{
			printf("Acabou\n");
			break;
		}
		printf("Consumidor recebendo %c da fila de mensagens\n", buffer.mNum);
					// Consumidor i lê a cada i segundo(s)
	}
}