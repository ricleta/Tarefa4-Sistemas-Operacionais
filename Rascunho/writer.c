#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

// Para limitar o tamanho da fila 
#define LIM 32	

// Quantidade de mensagens a serem trocadas
#define MAX_LOOP 128

typedef struct msgbuf 
{
    long mtype;
    char mNum;
} message_buf;

int main(void)
{
    int msgID, id, res;
	msgID = msgget(0xBEEF, 0666 | IPC_CREAT);	// Cria fila de mensagens e guarda o ID da fila em msgID
	
    if(msgID == -1)
	{
		printf("Erro na criação da Fila de Mensagens\n");
        return -1;
	}

	struct msqid_ds aux;

	message_buf buffer;

    msgctl(msgID, IPC_STAT, &aux);	
	
    msgctl(msgID, IPC_SET, &aux);
	
    for(int i = 0; i < MAX_LOOP; i++)
	{
		buffer.mtype = 1;	// Seta o tipo da mensagem no buffer de mensagens
		buffer.mNum =  rand() % ('Z' - 'A' + 1) + 'A';	// Coloca um inteiro no buffer de mensagens
		res = msgsnd(msgID, &buffer, sizeof(buffer.mNum), 0);	// Coloca mensagem na fila. Caso não tenha espaço na fila, bloqueia até ter
		
        if(res == -1) 
		{ 
			perror("Erro ao enviar mensagem"); 
            return -1;
		}
		
        printf("Produtor enviando %c para a fila de mensagens; %ld\n", buffer.mNum, time(NULL));
		sleep(1);		// Envia a cada 1 segundo
	}
	
	buffer.mtype = 2;
	res = msgsnd(msgID, &buffer, 0, 0);

    return 0;
}