/* signaltest.c - anv�ndning av signaler */
#include <sys/types.h> /* definierar bland annat typen pid_t */
#include <errno.h> /* definierar felkontrollvariabeln errno */
#include <signal.h> /* deinierar signalnamn med mera */
#include <stdio.h> /* definierar stderr, dit felmeddelanden skrivs */
#include <stdlib.h> /* definierar bland annat exit() */
#include <unistd.h> /* definierar bland annat sleep() */

#define TRUE ( 1 ) /* definierar den Boolska konstanten TRUE */
pid_t childpid; /* processid som returneras fr�n fork */

void register_sighandler(int signal_code, void(*handler)(int sig))
{
	int return_value; /* f�r returv�rden fr�n systemanrop */
	/* datastruktur f�r parametrar till systemanropet sigaction */
	struct sigaction signal_parameters;
	/*
	* ange parametrar f�r anrop till sigaction
	* sa_handler = den rutin som ska k�ras om signal ges
	* sa_mask = m�ngd av �vriga signaler som ska sp�rras
	* medan handlern k�rs (h�r: inga alls)
	* sa_flags = extravillkor (h�r: inga alls)
	*/
	signal_parameters.sa_handler = handler;
	sigemptyset(&signal_parameters.sa_mask); /* skapar tom m�ngd */
	signal_parameters.sa_flags = 0;
	/* beg�r hantering av signal_code med ovanst�ende parametrar */
	return_value = sigaction(signal_code, &signal_parameters, (void *)0);
	if (-1 == return_value) /* sigaction() misslyckades */
	{
		perror("sigaction() failed"); exit(1);
	}
}

/* signal_handler skriver ut att processen f�tt en signal.
Denna funktion registreras som handler f�r child-processen */
void signal_handler(int signal_code)
{
	char * signal_message = "UNKNOWN"; /* f�r signalnamnet */
	char * which_process = "UNKNOWN"; /* s�tts till Parent eller Child */
	if (SIGINT == signal_code) signal_message = "SIGINT";
	if (0 == childpid) which_process = "Child";
	if (childpid > 0) which_process = "Parent";
	fprintf(stderr, "%s process (pid %ld) caught signal no. %d (%s)\n",
		which_process, (long int)getpid(), signal_code, signal_message);
}

/* cleanup_handler d�dar child-processen vid SIGINT
Denna funktion registreras som handler f�r parent-processen */
void cleanup_handler(int signal_code)
{
	char * signal_message = "UNKNOWN"; /* f�r signalnamnet */
	char * which_process = "UNKNOWN"; /* s�tts till Parent eller Child */
	if (SIGINT == signal_code) signal_message = "SIGINT";
	if (0 == childpid) which_process = "Child";
	if (childpid > 0) which_process = "Parent";
	fprintf(stderr, "%s process (pid %ld) caught signal no. %d (%s)\n",
		which_process, (long int)getpid(), signal_code, signal_message);
	/* if we are parent and signal was SIGINT, then kill child */
	if (childpid > 0 && SIGINT == signal_code)
	{
		int return_value; /* f�r returv�rden fr�n systemanrop */
		fprintf(stderr, "Parent (pid %ld) will now kill child (pid %ld)\n",
			(long int)getpid(), (long int)childpid);
		return_value = kill(childpid, SIGKILL); /* kill child process */
		if (-1 == return_value) /* kill() misslyckades */
		{
			perror("kill() failed"); exit(1);
		}
		exit(0); /* normal successful completion */
	}
}

int main()
{
	fprintf(stderr, "Parent (pid %ld) started\n", (long int)getpid());
	childpid = fork();
	if (0 == childpid)
	{
		/* denna kod k�rs endast i child-processen */
		fprintf(stderr, "Child (pid %ld) started\n", (long int)getpid());
		/* installera signalhanterare f�r SIGINT */
		register_sighandler(SIGINT, signal_handler);
		while (TRUE); /* do nothing, just loop forever */
	}
	else
	{
		/* denna kod k�rs endast i parent-processen */
		if (-1 == childpid) /* fork() misslyckades */
		{
			perror("fork() failed"); exit(1);
		}
		/*
		Kommer vi hit i koden s� �r vi i parent-processen
		och fork() har fungerat bra - i s� fall inneh�ller
		variabeln childpid child-processens process-ID
		*/
		/*
		* G�r en liten paus innan vi registrerar parent-processens
		* signal-handler f�r SIGINT. Om vi trycker control-C i pausen
		* s� avbryts parent-processen och d�r. Vad h�nder d� med
		* child-processen? Prova!
		*/
		sleep(1);
		register_sighandler(SIGINT, cleanup_handler);
		while (TRUE)
		{
			int return_value; /* f�r returv�rden fr�n systemanrop */
			fprintf(stderr,
				"Parent (pid %ld) sending SIGINT to child (pid %ld)\n",
				(long int)getpid(), (long int)childpid);
			/* skicka SIGINT till child-processen, om och om igen */
			return_value = kill(childpid, SIGINT);
			if (-1 == return_value) /* kill() misslyckades */
			{
				fprintf(stderr,
					"Parent (pid %ld) couldn't interrupt child (pid %ld)\n",
					(long int)getpid(), (long int)childpid);
				perror("kill() failed");
				/* programmet forts�tter �ven om detta h�nder */
			}
			sleep(1); /* g�r en liten paus mellan varje SIGINT --
					  vi ska inte stress-testa systemet */
		}
	}
}