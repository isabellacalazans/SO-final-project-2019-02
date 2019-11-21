# Trabalho final de Sistemas Operacionais (Uniceub - Ciências da Computação - 2º semestre)

## Sinopse
Foi recebido um código-fonte em linguagem C para análise e ajustes que permitissem a portabilidade do código para ao menos dois de três sistemas operacionais informados: Linux, Unix e Windows.
O código original, sem alterações, foi compilado para teste de seu funcionamento, quando foi detectado que não funcionava adequadamente, conforme a solicitação apresentada no [projeto final](SO-final-project-2019-02/blob/master/Arquivo%20Projeto%20Final%202019%2002.pdf)

Segue abaixo as etapas detalhando os ajustes para correção e portabilidade do código.

## 1- Função Clone 
No trecho 
                  
                  pid = clone( &transferencia, (char*) stack + FIBER_STACK,
                  SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, 0 );
                  
   ocorre chamada à função clone, que é uma função usada para criar um processo. Entretanto, ao tentar compilar o código original em ambiente windows 10 e Unix Solaris 11 ocorreu erro de compilação, conforme pode ser verificado nos respectivos arquivos [Erro ao compilar com função Clone em Windows 10](Erro%20ao%20Compilar%20com%20funcao%20Clone%20em%20Windows%2010.jpg) e [Erro ao Compilar com funcao Clone em Unix Solaris 11.jpg](https://raw.githubusercontent.com/isabellacalazans/SO-final-project-2019-02/master/Erro%20ao%20Compilar%20com%20funcao%20Clone%20em%20Unix%20Solaris%2011.jpg). Este foi o primeiro obtáculo encontrado para atingir a portabilidade do código.

## 2- Biblioteca PThread
Para resolver o problema, ou seja, permitir que o código se tornasse portável, foi feita uma pesquisa com as seguintes palavras-chaves: "C programming thread". No resultado da pesquisa destacou-se a solução Pthread, que significa POSIX Thread. O acrônimo POSIX significa Portable Operating System Interface e é um conjunto de normas para permitir a compatibilidade de código-fonte entre sistemas operacionais.

## 3- Implementação de PThread
  **3.1- Foram inseridas as linhas abaixo:**
  
                #define _OPEN_THREADS
                #include <pthread.h>
                
  **3.- Para permitir o uso de pthread foi alterada a assinatura da função tranferencia de**
  
                int transferencia( void *arg)
                      
para
                      
                void *transferencia(void *arg)  
                
  3.x - também na função **transferencia** foi incluída a chamada **pthread_exit(ret)** 
  
  
 3. -Foi incluida a variável thid do tipo pthread_t, que é utilizada como identificação única da thread que será criada:
  
                pthread_t thid
                
  3.- Foi comentada a chamada da função clone.
  
  3.- Foi incluida a chamada à função pthread_create, conforme abaixo:
  
                pid = pthread_create (&thid, NULL, transferencia,"tread 1" )
                
  3.- O if de verificação de retorno da função clone (if ( pid == -1 )) foi alterado para (if ( pid != 0 )), pois o teste de retorno de erro de pthread é diferente.
  
  3.- Foi incluida a chamada para a função pthread_join, que é utilizada para aguardar o fim da thread:
  
              rstatus = pthread_join(thid, &thread_res)
              
##4- Como compilar
A biblioteca pthread é dinâmica, ou seja, para compilar o código em C que a utiliza é necessário acrescentar o parâmetro *-lpthread* na linha do *gcc*, conforme o exemplo abaixo:
 
              gcc myprogram.c -o myprogram -lpthread
       
obs: essa linha de compilação serve para todas as plataformas propostas (Win 10/Unix/Linux).
              
##5- Observações gerais
Dependendo da configuração do S.O. pode ser necessário a instalação da biblioteca pthread para a compilação. Por exemplo, no caso do Windows 10 são necessários *cygwing64* e *mingw64*.
