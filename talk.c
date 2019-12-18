/******************************************************************************
*	if(!exista un proces cu pid_ul pa &&									  *
*	cu acelasi nume ca programul meu){										  *
*		exit(EXIT_FAILURE);													  *
*	}																		  *
*																			  *
*	initializarea a 28 de semnale cu sigemptyset,							  * 
*	sigaddset, sigprocmask, sigfillset, sigdelset							  *
*	-----------------------------------------------------------------------   *
*	SETUP																	  *
*	    - asociez semnalelor handler-ul										  *
*	-----------------------------------------------------------------------   *
*	INITIALIZARE I/O														  *
*	    - citesc pid-ul celuilalt proces de la tastatura				      *
*	    - verific daca este valid(daca ruleaza acelasi program)				  *
*	-----------------------------------------------------------------------   *
*	CITIRE LINII															  *
*		- citesc liniile primite de la tastatura							  *
*		- trimit semnalele corespunzatoare									  *
*		- resetez buffer-ul													  *
*																			  *
*	{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 									  *
*	  'h', 'i', 'j', 'k', 'l', 'm', 'n',									  *
*	  'o', 'p', 'q', 'r', 's', 't', 'u',									  *
*	  'v', 'w', 'x', 'y', 'z', ' ', '\n'									  *
*	};																		  *
*	//struct sigaction act;													  *
*	//memset(&act, 0, sizeof(act));											  *
*	//act.sa_handler = handler;												  *
*																			  *
******************************************************************************/
/*

7. (30 puncte) Talk bazat doar pe semnale: un acelasi program este lansat de 
    la 2 terminale diferite de acelasi utilizator,obtinand 2 procese diferite.
    Fiecare proces citeste de la tastatura PID-ul celuilalt. Fiecare proces
    are un tabel care asociaza cate un semnal != SIGKILL, SIGCONT, SIGSTOP
    celor 26 litere, blank-ului si capului de linie. Fiecare proces citeste
    intr-un ciclu cate o linie de la tastatura, apoi o parcurge si trimite
    celuilalt proces semnalul asociat fiecarui caracter din ea (inclusiv
    blank-urile si capul de linie). De asemenea, fiecare proces, la primirea
    unui asemenea semnal, va afla caracterul corespunzator si-l va scrie pe
    ecran. Se va asigura protectia la pierderea unor semnale si se va
    asigura ca semnalele trimise de un proces sa fie primite de celalalt in
    aceeasi ordine (de exemplu un proces nu va emite semnalul corespunzator
    unui caracter decat daca a primit confirmarea ca celalalt proces a
    tratat semnalul pentru caracterul precedent).
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#define BUFFER_SIZE 256


const char semnale[] = "abcdefghijklmnopqrstuvwxyz \n";
const int offset = 34;

char red[] = "\033[1;31m";
char blue[] = "\033[1;34m";
char black[] = "\033[0m";

pid_t pa;
struct sigaction act;
sigset_t ms;


const char* my_color(int k)
{
    if(k == 0){
        if(pa > getpid())
            return red;
        return blue;
    }
    if(k == 1){
        if(pa > getpid())
            return blue;
        return red;
    }
    return NULL;
}


int not_same_process(pid_t m_pa)
{
    char np[BUFFER_SIZE];
    char na[BUFFER_SIZE];
    
    snprintf(np,BUFFER_SIZE,"/proc/%d/cmdline",getpid());
    snprintf(na,BUFFER_SIZE,"/proc/%d/cmdline",m_pa);
    
    FILE *fp = fopen(np, "r");
    FILE *fa = fopen(na, "r");
    
    if(fp != NULL && fa != NULL)
    {
        size_t sp = fread(np,sizeof(char),BUFFER_SIZE,fp);
        size_t sa = fread(na,sizeof(char),BUFFER_SIZE,fa);
        
        fclose(fp); fclose(fa);
        
        if(sp == 0 || sa == 0) {
            return 1;
        }
        
        if(na[sa-1] == '\n') na[sa-1] = '\0';    
        if(np[sp-1] == '\n') np[sp-1] = '\0';
          
        return (strcmp(np, na) == 0) ? 0 : 1;
    }
    
    return 1;
}


pid_t evalueaza(char nume_pid[])
{
    char *caracter;
    pid_t m_pa = (pid_t) strtol(nume_pid, &caracter, 10);
    
    if(*caracter != '\0'||  strlen(nume_pid) == 0)  return (pid_t) 0;
    if(m_pa == getpid() || not_same_process(m_pa))  return (pid_t) 0;
    
    return m_pa;
}


void read_pid()
{
    char nume_pid[BUFFER_SIZE];
    printf("PID propriu: %d\n", getpid());
	printf("Introduceti pid-ul advers: ");
	scanf("%s", nume_pid);

	if(!(pa = evalueaza(nume_pid))){
	    read_pid();
	    return;
	}
	
	printf("PID-ul introdus este %d\n", pa);
	printf("\n\t************** SUNTETI CONECTAT **************\n");
	getchar();
}


int valueOf(char c)
{
	if(c >= 'a' && c <= 'z') return offset + c - 'a';
	if(c == ' ')             return offset + strlen(semnale) - 2;
	if(c == '\n')            return offset + strlen(semnale) - 1;
	return -1;
}


void convert(const char line[], int sig_type[])
{
    int i;
	for(i = 0; i < strlen(line); i++){
	    sig_type[i] = valueOf(line[i]);
	}
}


int valid_line(const char line[])
{
	int nr_caractere = 0, i;
	
	for(i = 0; i < strlen(line); i++)
	{
	    const char ch = line[i];
	    if(ch != ' ' && ch != '\n')
	    {
	        if(ch < 'a' || ch > 'z')
				return 0;
	        else
				nr_caractere++;
	    }
	}
	return nr_caractere;
}


void handler_SIG_INT(int n)
{
    printf("%s\n\t************ CONEXIUNE TERMINATA ************\n", black);

    if(kill(pa, 0) == 0)
        kill(pa, SIGINT);
    exit(0);
}


void handler_SIGUSR1(int n)
{
    signal(n,handler_SIGUSR1);
}


void handler_SIGUSR2(int n)
{
    static int x = 1;
   
    write(STDOUT_FILENO, my_color(x), strlen(my_color(x)));
   
    x = !x;
    
    kill(pa,SIGUSR1);
}


void handler(int n)
{
    int poz = n-offset;
    char ch = semnale[poz];
    
	write(STDOUT_FILENO,&ch,1);
	
	kill(pa,SIGUSR1);
	
	if(ch != '\n')
	    sigsuspend(&act.sa_mask);
}


void send_signals(int sig_type[], int length)
{
    int i;
    for(i = 0; i < length; i++){
        kill(pa, sig_type[i]);
        sigsuspend(&ms);
    }
}


void togglecolor()
{
    kill(pa,SIGUSR2);
    sigsuspend(&ms);
}


void initialize_signals()
{
    signal(SIGUSR1,handler_SIGUSR1);
    signal(SIGUSR2,handler_SIGUSR2);
    signal(SIGINT, handler_SIG_INT);
	
	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;	
	act.sa_flags = 0;	
    
    sigfillset(&act.sa_mask); sigdelset(&act.sa_mask,SIGINT);
    sigfillset(&ms); 
    sigdelset(&ms, SIGUSR1); sigdelset(&ms, SIGINT);
    
	int i, sig_type;
	for(i = 0; i < strlen(semnale); i++)
	{
		sig_type = valueOf(semnale[i]);

		sigaction(sig_type, &act, NULL);
		sigdelset(&act.sa_mask, sig_type);
	}
	sigdelset(&act.sa_mask, SIGUSR1);
	sigdelset(&act.sa_mask, SIGUSR2);
	sigprocmask(SIG_SETMASK, &act.sa_mask, NULL);
}


void block_signals()
{
    sigset_t initial;
    sigfillset(&initial); sigdelset(&initial, SIGINT);
    sigprocmask(SIG_SETMASK, &initial, NULL);
}


int main()
{
    char    line[BUFFER_SIZE];
	int sig_type[BUFFER_SIZE];
	
    block_signals();
    
    read_pid();
    
    write(STDOUT_FILENO, my_color(0), strlen(my_color(0)));
    
	initialize_signals();
	
	while(1)
	{
		fgets(line,BUFFER_SIZE,stdin);
		
		if(valid_line(line))
		{ 
		    togglecolor();
			convert(line, sig_type);
			send_signals(sig_type, strlen(line));
		    togglecolor();
		}
		
		memset(line, 0, BUFFER_SIZE);
	}
	
	return 0;
}


