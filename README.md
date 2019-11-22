# Trabalho final de Sistemas Operacionais (Uniceub - Ciências da Computação - 2º semestre)

## Sinopse
Foi recebido um código-fonte em linguagem C para análise e ajustes que permitissem a portabilidade do código para ao menos dois de três sistemas operacionais informados: Linux, Unix e Windows.
O código original, sem alterações, foi compilado para testar seu funcionamento, e foi detectado que não funcionava adequadamente, conforme mencionado no [projeto final.](SO-final-project-2019-02/blob/master/Arquivo%20Projeto%20Final%202019%2002.pdf)

Segue abaixo as etapas detalhando os ajustes para correção e portabilidade do código.

## 1- Função Clone 
No trecho 
                  
                  pid = clone( &transferencia, (char*) stack + FIBER_STACK,
                  SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, 0 );
                  
   ocorre chamada à função clone, que é uma função usada para criar um processo. Entretanto, ao tentar compilar o código original em ambiente windows 10 e Unix Solaris 11 ocorreu erro de compilação, conforme pode ser verificado nos respectivos arquivos [Erro ao compilar com função Clone em Windows 10](Erro%20ao%20Compilar%20com%20funcao%20Clone%20em%20Windows%2010.jpg) e [Erro ao Compilar com funcao Clone em Unix Solaris 11.jpg](https://raw.githubusercontent.com/isabellacalazans/SO-final-project-2019-02/master/Erro%20ao%20Compilar%20com%20funcao%20Clone%20em%20Unix%20Solaris%2011.jpg). Este foi o primeiro obtáculo encontrado para atingir a portabilidade do código.

## 2- Biblioteca PThread
Para resolver o problema, ou seja, permitir que o código se tornasse portável, foi feita uma pesquisa com as seguintes palavras-chaves: "C programming thread". No resultado da pesquisa destacou-se a solução Pthread, que significa POSIX Thread. O acrônimo POSIX significa Portable Operating System Interface e é um conjunto de normas para permitir a compatibilidade de código-fonte entre sistemas operacionais.

## 3- Implementação de PThread
  3.1- Foram inseridas as linhas abaixo para a correta compilação e uso das bibliotecas:
  
                #define _OPEN_THREADS
                #include <pthread.h>
                #include <semaphore.h> 
                
  3.2- Para permitir o uso de pthread foi alterada a assinatura da função tranferencia de
  
                int transferencia( void *arg)
                      
  **para**
                      
                void *transferencia(void *arg)  
                
  3.3- Ainda na função **transferencia** foram incluídas as linhas:
  
              sem_wait(&mutex);  //semafóro de espera para controle da concorrência
              .
              .   //checando saldo em conta
              .
              .
              sem_post(&mutex);  //semafóro de liberação de controle
              pthread_exit(ret);  //termina a thread 
  
  
  3.4- Foi incluida a variável thid[], um array do tipo  _pthread_t_. Esse array vai acumular a identificação única de cada thread que será criada:
  
                pthread_t thid[limite]
                
  **observação:** a constante _limite_ define quantas _threads_ serão criadas, ou seja, quantas transferências simultâneas serão realizadas.
  
  3.5- Foi comentada a chamada da função clone.
  
  3.6- Antes da criação das _threads_ foi incluída a linha _sem_init(&mutex, 0, 1);_ para inicialização dos controle de semáforos no programa
  
  3.7- Foi incluida a chamada à função pthread_create, conforme abaixo:
  
                ret = pthread_create (&thid[i], NULL, transferencia,"tread 1" )
              
  **observação:** para simular devidamente a concorrência foram criados 2 _for's_ sendo um deles específico para a criação das _threads_ (ao término deste for já estarão criadas e sendo executadas as 100 threads). E outro _for_ para o as chamadas pthread_join() conforme explicado no próximo tópico.
  
  3.8- Foi incluida a chamada para a função _pthread_join_, que é utilizada para aguardar o fim de thread indicada no primeiro parâmetro:
  
              ret = pthread_join(thid[], &thread_res)
              
  **observação:** as chamadas para _pthread_join_ foram colocadas dentro de outro _for_ para permitir que estas fossem feitas após a criação de todas as _threads_ no _for_ anterior, com objetivo de simular adequadamente a concorrência entre as _threads_. Desta forma, são criadas 100 _threads_ concorrentes entre si.
  
  3.9- No final do programa foi incluída a linha _	sem_destroy(&mutex);_ para liberação dos semafóros.
  
## 4- Como compilar
A biblioteca _pthread_ é dinâmica, ou seja, para compilar o código em C que a utiliza é necessário acrescentar o parâmetro *-lpthread* na linha do *gcc*, conforme o exemplo abaixo:
 
              gcc myprogram.c -o myprogram -lpthread
       
**observação:** essa linha de compilação funciona em todas as plataformas propostas (Win 10/Unix/Linux).


## 5- Como executar
Uma vez compilado conforme indicação no passo anterior, para executar o programa basta proceder conforme abaixo:
  1. Linux: 
      - Abrir a janela do terminal (Ctrl+Shift+T), caso ainda não esteja aberta;
      - Vá até a pasta onde compilou o programa (utilize o comando _cd_ para navegar entre as pastas);
      - digite **./myprogram** e aguarde o resuldado do processamento na tela do terminal.
  2. Unix:
      - Após logar no sistema, vá até a pasta onde compilou o programa (utilize o comando _cd_ para navegar entre as pastas)
      - digite **./myprogram** e aguarde o resultado do processamento na tela do terminal.
  3. Windows: 
     - Abrir o prompt de comando (tecla Win+R, digitar cmd e teclar enter)
     - Vá até a pasta onde compilou o programa (utilize o comando _cd_ para navegar entre as pastas)
     - digite myprogram.exe e aguarde o resultado do processamento no _prompt_ de comando do Windows (nesse momento, alguns antí-virus podem iniciar uma verificação no seu programa, aguarde o término).

## 6- Coleta de Evidencias da Execução
A coleta de evidências da execução do programa nos 3 ambientes (linux, unix e windows) foi feita de duas formas: 
1) _print screen_ da tela de comando/terminal
    [printscreen Windows](Cem%20transferencias%20simultaneas%20Windows%2010.jpg)
    [printscreen Linux](Cem%20transferencias%20simultaneas%20Ubuntu.jpg)
    [printscreen Unix](Cem%20transferencias%20simultaneas%20Unix%20Solaris%2011.jpg)
2) gerando arquivo de saída através do redirecionamento da saída padrão para arquivo
    [arquivo txt Windows](out_win.txt)
    [arquivo txt Linux](out_linux.txt)
    [arquivo txt Unix](out_unix.txt)

## 7- Observações gerais
Dependendo da configuração do S.O. pode ser necessário a instalação da biblioteca pthread para a compilação. 
Por exemplo, no caso do Windows 10 são necessários *cygwing64* e *mingw64*.
