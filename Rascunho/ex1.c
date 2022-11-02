// // Eduardo Dantas Luna, 2111484
// // Ricardo Bastos Leta Vieira, 2110526

// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/sem.h>
// #include <sys/shm.h>
// #include <sys/stat.h>
// #include <sys/wait.h>
// #include <unistd.h>

// union semun
// {
//   int val;
//   struct semid_ds *buf;
//   unsigned short *array;
// };

// // inicializa o valor do semáforo
// int setSemValue(int semId, int val);
// // remove o semáforo
// void delSemValue(int semId);
// // operação P
// int semaforoP(int semId);
// // operação V
// int semaforoV(int semId);

// int main(void) 
// {
//   // shared mem
//   int status = 0;
//   int n_loop = 128;
//   int segmento;
//   char *buffer; 
//   char aux;
//   int semId_empty, semId_full, semId_mutex;

//   segmento = shmget(IPC_PRIVATE, sizeof(char) * 32,
//                     IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

//   if (segmento < 0)
//   {
//     perror("error in shared mem");
//   }

//   buffer = (char *)shmat(segmento, 0, 0);

//   if (*buffer == -1)
//   {
//     perror("error in shared mem");
//   }
  
//   semId_empty = semget(8752, 1, 0666 | IPC_CREAT);
//   setSemValue(semId_empty, 32);

//   semId_full = semget(8753, 1, 0666 | IPC_CREAT);
//   setSemValue(semId_full, 0);

//   semId_mutex = semget(8754, 1, 0666 | IPC_CREAT);
//   setSemValue(semId_mutex, 1);

//   if (semId_empty < 0 || semId_full < 0 || semId_mutex < 0)
//   {
//     perror("error semaphore creat");
//   }
  
//   if (fork() == 0) 
//   { 
//     for (int k = 0; k < n_loop; k++) 
//     {
//       aux = rand() % ('Z' - 'A' + 1) + 'A';
      
//       semaforoP(semId_empty);
//       semaforoP(semId_mutex);
      
//       *buffer = aux;
      
//       semaforoV(semId_mutex);
//       semaforoV(semId_full);
//     }
//     sleep(1);
//     exit(0);
//   } 
//   else 
//   {
//     for (int l = 0; l < n_loop; l++) 
//     {
//       semaforoP(semId_full);
//       semaforoP(semId_mutex);

//       printf("%c ", *buffer);

//       semaforoV(semId_mutex);
//       semaforoV(semId_empty);
//     }
//     sleep(1);
//   }

//   for (int m = 0; m < 3; m++) 
//   {
//     waitpid(-1, &status, 0);
//   }

//   printf("\n");
  
//   // libera a memória compartilhada
//   shmdt(buffer);
//   shmctl(segmento, IPC_RMID, 0);

//   delSemValue(semId_empty);
//   delSemValue(semId_full);
//   delSemValue(semId_mutex);
  
//   return 0;
// }

// int setSemValue(int semId, int val) 
// {
//   union semun semUnion;
//   semUnion.val = val;
//   return semctl(semId, 0, SETVAL, semUnion);
// }

// void delSemValue(int semId) {
//   union semun semUnion;
//   semctl(semId, 0, IPC_RMID, semUnion);
// }

// int semaforoP(int semId) {
//   struct sembuf semB;
//   semB.sem_num = 0;
//   semB.sem_op = -1;
//   semB.sem_flg = SEM_UNDO;
//   semop(semId, &semB, 1);
//   return 0;
// }

// int semaforoV(int semId) {
//   struct sembuf semB;
//   semB.sem_num = 0;
//   semB.sem_op = 1;
//   semB.sem_flg = SEM_UNDO;
//   semop(semId, &semB, 1);
//   return 0;
// }