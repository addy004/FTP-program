#include "ftphelper.h"

void transfer_file(int, char *);

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if(argc != 3)
	{
		printf("usage client <IPaddress> <Filename>");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877); //Port
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	transfer_file(sockfd, argv[2]);

	exit(0);
}

void transfer_file(int sockfd, char *path)
{
	FILE *fp;
	char *buffer;
	size_t bytesread;
	struct file_metadata filedata;

	if((buffer = malloc(BUFFERSIZE)) == NULL)
		exit(1);

	fp = fopen(path, "rb");
	fseek(fp, 0L, SEEK_END);
	filedata.filesize = htonl(ftell(fp));
	rewind(fp);

	//Extract filename if arg 2 is a relative path
	char *filename = strrchr(path, '/') + 1;
	if(filename == NULL)
		filename = path;

	sprintf(filedata.filename, "%s", filename);

	//Write 24Bytes of metadata to socket
	write(sockfd, &filedata, sizeof(filedata));

	//Write the file data
	while((bytesread = fread(buffer, sizeof(char), BUFFERSIZE, fp)) > 0 )
	{
		int bytestowrite = bytesread, byteswritten = 0;
		while(bytestowrite > 0)
		{
			byteswritten = write(sockfd, buffer, bytestowrite);
			bytestowrite -= byteswritten;
			buffer += byteswritten;
		}

		buffer -= bytesread;
	}

	fclose(fp);
}
