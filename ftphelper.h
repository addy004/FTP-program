#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>

#define	BUFFERSIZE		1000

//Structure to hold filesize and filename of the file being transferred
struct file_metadata
{
	int filesize;
	char filename[20];
};