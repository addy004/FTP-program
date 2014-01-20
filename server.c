#include "ftphelper.h"

void get_file(int);

int main(int argc, char **argv)
{
	int listenfd, connfd;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t clilen;
	pid_t childpid;

	// AF_INET -- IPV4
	// SOCK_STREAM -- TCP socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9877);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));	
	listen(listenfd, 5);
	
	clilen = sizeof(cliaddr);
	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

	get_file(connfd);
	
	close(connfd);
	close(listenfd);
}

void get_file(int connfd)
{
	FILE *fp;
	char *buffer;
	int bytesread;
	long totalread = 0;
	struct file_metadata filedata;
	
	if((buffer = malloc(BUFFERSIZE)) == NULL)
	{
		exit(1);
	}

	// Read 24Bytes of file metadata
	while((bytesread = read(connfd, &filedata, sizeof(filedata))) > 0)
	{
		totalread += bytesread;
		if(totalread == 24)
			break;
	}


	fp = fopen(filedata.filename, "w");

	//Read the content from socket and write to file
	while((bytesread = read(connfd, buffer, BUFFERSIZE)) > 0)
	{
		fwrite(buffer, sizeof(char), bytesread, fp);
	}

	fclose(fp);
}