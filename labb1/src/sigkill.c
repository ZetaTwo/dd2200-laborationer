#include <sys/types.h> /* definierar bland annat typen pid_t */
#include <errno.h> /* definierar felkontrollvariabeln errno */
#include <signal.h> /* deinierar signalnamn med mera */
#include <stdio.h> /* definierar stderr, dit felmeddelanden skrivs */
#include <stdlib.h> /* definierar bland annat exit() */
#include <unistd.h> /* definierar bland annat sleep() */

void register_sighandler( int signal_code, void (*handler)(int sig) )
{
  int return_value; /* för returvärden från systemanrop */
  /* datastruktur för parametrar till systemanropet sigaction */
  struct sigaction signal_parameters;
  /*
  * ange parametrar för anrop till sigaction
  * sa_handler = den rutin som ska köras om signal ges
  * sa_mask = mängd av övriga signaler som ska spärras
  * medan handlern körs (här: inga alls)
  * sa_flags = extravillkor (här: inga alls)
  */
  signal_parameters.sa_handler = handler;
  sigemptyset( &signal_parameters.sa_mask ); /* skapar tom mängd */
  signal_parameters.sa_flags = 0;
  /* begär hantering av signal_code med ovanstående parametrar */
  return_value = sigaction( signal_code, &signal_parameters, (void *) 0 );
  if( -1 == return_value ) /* sigaction() misslyckades */
  { perror( "sigaction() failed" ); fprintf(stderr, "%d\n", errno);  exit( 1 ); }
}

void signal_handler( int signal_code )
{
  char * signal_message = "UNKNOWN"; /* för signalnamnet */
  char * which_process = "UNKNOWN"; /* sätts till Parent eller Child */
  if( SIGINT == signal_code ) signal_message = "SIGINT";
  fprintf( stderr, "%s process (pid %ld) caught signal no. %d (%s)\n",
    which_process, (long int) getpid(), signal_code, signal_message );
}

int main()
{
  fprintf( stderr, "Process (pid %ld) started\n", (long int) getpid() );
  register_sighandler( SIGKILL, signal_handler );
}