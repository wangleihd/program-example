#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( int argc, char **argv ) 
{
	int length, i = 0; //read 
	int fd;
	int wd;
	char buffer[BUF_LEN];

	if ((fd = inotify_init())  < 0 ) {
		perror( "inotify_init" );
		exit(1);
	}

	wd = inotify_add_watch( fd, argv[1], IN_MODIFY | IN_CREATE | IN_DELETE );

	while ((length = read( fd, buffer, sizeof(buffer))) > 0 ) {
		i = 0;

		while ( i < length ) {
			struct inotify_event *event = (struct inotify_event *)&buffer[ i ];
			if ( event->len ) {
				if ( event->mask & IN_CREATE ) {
					if ( event->mask & IN_ISDIR ) {
						printf( "The directory %s was created.\n", event->name );       
					}
					else {
						printf( "The file %s was created.\n", event->name );
					}
				}
				else if ( event->mask & IN_DELETE ) {
					if ( event->mask & IN_ISDIR ) {
						printf( "The directory %s was deleted.\n", event->name );       
					}
					else {
						printf( "The file %s was deleted.\n", event->name );
					}
				}
			}
			i += EVENT_SIZE + event->len;
		}

	}
	if(length < 0){
		perror("Read error.");
		exit(1);
	}
	inotify_rm_watch( fd, wd );
	close( fd );
	exit( 0 );
}

