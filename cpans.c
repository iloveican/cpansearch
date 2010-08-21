#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <glib.h>
#include <getopt.h>

#include "cpans.h"
#include "membuf.h"
#include "help.h"
#include "search.h"
#include "utils.h"
#include "init.h"
#include "update.h"

char version[] = "0.1";
char ignore_case = 0;
char fullurl     = 0;
char nameonly    = 0;


void init_local( char * localpath )
{



}


void init_from_minicpanrc()
{
    char minicpanrc[64];
    sprintf( minicpanrc , "%s/.minicpanrc" , g_get_home_dir() );

    if (!g_file_test (minicpanrc, G_FILE_TEST_EXISTS))
        return;



    FILE *rc = fopen( minicpanrc , "r" );
    char buffer[50] = {0};
    char localpath[64] = {0};

    strcat( localpath , "file://" );

    while(!feof(rc)){
        fgets( buffer , 50 , rc );
        if( strstr(buffer,"local:") == buffer ) {
            char * c = strchr(buffer,'/');
            strcat( localpath , c );

            // chomp
            *(localpath + strlen(localpath) - 1) = '\0';
            break;
        }
    }
    fclose(rc);
    assert( localpath != NULL );

    if( * (localpath + strlen(localpath) - 1 ) != '/' )
        strcat(localpath , "/");

    printf( "Found minicpanrc: %s\n" , localpath );
    init( localpath );
}

/* getopt setting start */
static int option_index = 0;
static int thisopt = 0;

static struct option long_options[] = {
  { "fetch"     , required_argument, 0 , 'f' },
  { "init"      , required_argument, 0 , 'f' },
  { "update"    , no_argument      , 0 , 'u' },
  { "recent"    , no_argument      , 0 , 'r' },
  { "help"      , no_argument      , 0 , 'h' },
  { "url"       , no_argument      , 0 , 'l' },
  { "name"      , no_argument      , 0 , 'n' },
};
/* getopt setting end */


int main(int argc, char **argv)
{

    setvbuf( stderr , 0, _IONBF, 0);
    setvbuf( stdout , 0, _IONBF, 0);

    int optbind = 0;
    while( (thisopt = getopt_long(argc, argv, "if:urh:nl", long_options, &option_index)) != -1 ) {

      switch (thisopt) {

        case 'f':
          if (optarg != NULL) {
            init( (char*)optarg);
          } else {
            init_from_minicpanrc();
          }
          return 0;

        case 'i':
          ignore_case = 1;
          ++optbind;
          break;

        case 'l':
          fullurl = 1;
          ++optbind;
          break;

        case 'n':
          nameonly = 1;
          ++optbind;
          break;

        case 'u':
          puts("Update package list from mirror");
          update();
          return 0;

        case 'r':
          puts("Searching packages from recent index");
          // update package list
          return 0;

        case 'h':
          help();
          return 0;
      }

    }

    int index;
    for (index = optind; index < argc; index++) {
        search(argv[index]);
        return 0;
    }

    return 0;
}



