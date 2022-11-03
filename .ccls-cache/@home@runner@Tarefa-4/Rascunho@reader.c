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
		res = msgrcv(msgID, &buffer, sizeof(buffer.mNum), 0, 0);	// Tira mensagem da fila e coloca no buffer 

		
        if(res == -1) 
		{ 
			perror("Erro ao receber mensagem"); 
            
            return -1;
		}
		
		printf("Consumidor recebendo %c da fila de mensagens\n", buffer.mNum);
		sleep(1);
	}

    return 0;
}