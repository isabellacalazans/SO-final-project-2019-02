#define _GNU_SOURCE
#define _OPEN_THREADS
#include <pthread.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>

// 64kB stack
#define FIBER_STACK 1024*64
struct c {
  int saldo;
};
typedef struct c conta;

conta from, to;
int valor;
pid_t pid;

// The child thread will execute this function
void *transferencia(void *arg)
{
  if (from.saldo >= valor){ // 2
	from.saldo -= valor;
	to.saldo += valor;
  
	printf("Transferência concluída! Saldo c1: %d - Saldo  c2: %d\n" , from.saldo , to.saldo);

	char *ret;
	ret = (char*) malloc(20);
	strcpy(ret, "Fim Ok");
	pthread_exit(ret);
	return 0;
  }
}

int main()
{
 void* stack;
 void* param;
 int i;

 // Allocate the stack
 stack = malloc( FIBER_STACK );
 if ( stack == 0 )
 {
 perror("malloc: could not allocate stack");
 exit(1);
 }
 // Todas as contas começam com saldo 100
 from.saldo = 10000;
 to.saldo = 10000;
 printf( "Transferindo 10 para a conta c2\n" );
 valor = 10;
 const int limite = 100;
 pthread_t thid[limite];
 void * thread_res;
 int rstatus;

 for (i = 0; i < limite; i++) {
   // Call the clone system call to create the child thread
   //pid = clone( &transferencia, (char*) stack + FIBER_STACK,
   //SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, 0 );

     pid = pthread_create (&thid[i], NULL, transferencia,"tread 1" );
     if ( pid != 0 ) {
       perror( "thread create error" );
       exit(2);
     }

    printf ("valor de pid %lu \n", thid[i]);;
 }
 
 for (i = 0; i < limite; i++) {
	rstatus = pthread_join(thid[i], &thread_res);

    /*verificar se ocorreu algum erro na chamada de pthread_join*/
  	if(rstatus != 0)
  	{
  		printf ("Erro ao aguardar finalização do thread.\n");
  		exit(EXIT_FAILURE);
  	}

  	/*exibe o valor de retorno da função 'routine'*/
  	printf ("Thread finalizado! Retorno = %s\n", (char *)thread_res);

 }

 // Free the stack
 free( stack );
 printf("Transferências concluídas e memória liberada.\n");
 return 0;

}
