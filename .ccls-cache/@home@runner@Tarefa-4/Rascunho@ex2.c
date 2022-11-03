#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

// Para limitar o tamanho da fila 
#define LIM 32	

// Quantidade de mensagens a serem trocadas
#define MAX_LOOP 128

typedef struct msgbuf 
{
    long mtype;
    char mNum;
} message_buf;

void produtor(int msgID);	
void consumidor(int msgID);

int main(void)
{
	int msgID, id;
	msgID = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);	// Cria fila de mensagens e guarda o ID da fila em msgID
	if(msgID == -1)
	{
		printf("Erro na criação da Fila de Mensagens\n");
	}

	struct msqid_ds aux;			// Cria estrutura auxiliar para controlar a fila de mensagens
	
    msgctl(msgID, IPC_STAT, &aux);	
	aux.msg_qbytes = sizeof(int)*LIM;
	
    msgctl(msgID, IPC_SET, &aux);
	
	id = fork();
	if(id == 0)	
	{
		produtor(msgID);
	}
	else
	{
		consumidor(msgID);
	}
}

void produtor(int msgID)
{
	message_buf buffer;

	int i, res;
	
    for(i = 0; i < MAX_LOOP; i++)
	{
		buffer.mtype = 1;	// Seta o tipo da mensagem no buffer de mensagens
		buffer.mNum =  rand() % ('Z' - 'A' + 1) + 'A';	// Coloca um inteiro no buffer de mensagens
		res = msgsnd(msgID, &buffer, sizeof(buffer.mNum), 0);	// Coloca mensagem na fila. Caso não tenha espaço na fila, bloqueia até ter
		
        if(res == -1) 
		{ 
			perror("Erro ao enviar mensagem"); 
		}
		
        // printf("Produtor enviando %c para a fila de mensagens\n", buffer.mNum);
		// sleep(1);		// Envia a cada 1 segundo
	}
	
	buffer.mtype = 2;
	res = msgsnd(msgID, &buffer, 0, 0);
}
void consumidor(int msgID)
{
	message_buf buffer;		// Cria buffer para receber a mensagem
	int res;
	
    for(int i = 0; i < MAX_LOOP; i++)		// Os consumidores estão sempre esperando para consumir algo
	{
		res = msgrcv(msgID, &buffer, sizeof(buffer.mNum), 0, 0);	// Tira mensagem da fila e coloca no buffer 

		
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
		// sleep(1);
	}
}