
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

int main(void)
{
    int msgID, id;
	msgID = msgget(0xBEEF, 0666 | IPC_CREAT);	// Cria fila de mensagens e guarda o ID da fila em msgID
	if(msgID == -1)
	{
		printf("Erro na criação da Fila de Mensagens\n");
	}
	struct msqid_ds aux;		
	message_buf buffer;		// Cria buffer para enviar a mensagem
	int i, res;
	for(i = 0; i < ELEM; i++)	// Produzindo ELEM mensagens (ELEM = 256)
	{
		buffer.mtype = 1;	// Seta o tipo da mensagem no buffer de mensagens
		buffer.mNum =  i % ('Z' - 'A' + 1) + 'A';	// Coloca um inteiro no buffer de mensagens
		res = msgsnd(msgID, &buffer, sizeof(buffer.mNum), 0);	// Coloca mensagem na fila. Caso não tenha espaço na fila, bloqueia até ter
		if(res == -1) 
		{ 
			perror("Erro ao enviar mensagem"); 
		}
		printf("Produtor enviando %c para a fila de mensagens\n", buffer.mNum);
		//sleep(1);		// Envia a cada 1 segundo
	}
	/* Após ter produzido os 256 elementos, consumidor envia mensagem para os consumidores indicando que acabou */
	buffer.mtype = 2;
	res = msgsnd(msgID, &buffer, 0, 0);
}