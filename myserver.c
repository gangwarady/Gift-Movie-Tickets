#include "csapp.h"

void *ticket(void *conn){
	int des=*((int*)conn);
	Pthread_detach(pthread_self());
	Free(conn);
	int is_static;
    	struct stat sbuf;
    	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    	char filename[MAXLINE], cgiargs[MAXLINE];
    	rio_t rio;
    	Rio_readinitb(&rio, des);
    	if (!Rio_readlineb(&rio, buf, MAXLINE)){  
    		Close(des);
        	return NULL;
        	
        }	
    	sscanf(buf, "%s %s %s", method, uri, version);
    	char *name;
    	char *quan;
    	char *type;
    	char *email;
    	char *info;
    	char t[500];
    	info=strtok(uri,"?");
    	name=strtok(NULL,"&");
    	quan=strtok(NULL,"&");
    	type=strtok(NULL,"&");
    	email=strtok(NULL,"&");
    	if(name==NULL){
    		printf("Kindly enter guest name\n");
    		Close(des);
    		return NULL;
    	}
    	int quantity=atoi(quan);
    	if(quantity==0){
    		printf("Invalid quantity requested.\n");
    		Close(des);
    		return NULL;
    	}
    	if(type==NULL){
    		printf("Kindly enter type of ticket\n");
    		Close(des);
    		return NULL;
    	}
    	if(email==NULL){
    		printf("Invalid email address.\n");
    		Close(des);
    		return NULL;
    	}
    	
    	if(quantity<=0 ){
    		printf("Invalid quantity requested.\n");
    		Close(des);
    		return NULL;
    	}
    	if(!isalpha(email[0]) && !isdigit(email[0])){
    		printf("Invalid email address.\n");
    		Close(des);
    		return NULL;
    	}
    	int dot=0;
    	int at=0;
    	int email_size=strlen(email);
    	for(int i=0;i<=email_size;i++){
    		if(email[i]=='@'){
    			at=i;
    		}
    		else if(email[i]=='.'){
    			dot=i;
    		}
    	}
    	if(at==0 || dot==0 || at>dot){
    		printf("Invalid email address.\n");
    		Close(des);
    		return NULL;
    	}
    	FILE *mydata = fopen("data.txt", "r");
    	if (mydata == NULL) {
        	printf("File does not exist\n");
        	exit(EXIT_FAILURE);
    	}
    	char data[50];
    	int n_tickets[5];
    	int i=0;
    	while (fgets(data, sizeof data, mydata) != NULL) {
        	if (sscanf(data, "%d", &n_tickets[i]) == 1) {
            		i++;
        	}
    	}
    	fclose(mydata);
    	if(strcasecmp(type,"normal")==0){
    		for(int i=0;i<quantity;i++){
    			char buff[5];
    			sprintf(buff, "%d", n_tickets[0]);
    			strcat(t,buff);
    			strcat(t,"N, ");
    			n_tickets[0]--;
    		}
    		if(n_tickets[0]<0){
    			printf("No sufficient tickets are available in this category.\n");
    			Close(des);
    			return NULL;
    		}
    		
    	}
    	else if(strcasecmp(type,"deluxe")==0){
    		for(int i=0;i<quantity;i++){
    			char buff[5];
    			sprintf(buff, "%d", n_tickets[1]);
    			strcat(t,buff);
    			strcat(t,"D, ");
    			n_tickets[1]--;
    		}
    		if(n_tickets[1]<0){
    			printf("No sufficient tickets are available in this category.\n");
    			Close(des);
    			return NULL;
    		}
    	
    	}
    	else if(strcasecmp(type,"premium")==0){
    		for(int i=0;i<quantity;i++){
    			char buff[5];
    			sprintf(buff, "%d", n_tickets[2]);
    			strcat(t,buff);
    			strcat(t,"P, ");
    			n_tickets[2]--;
    		}
    		if(n_tickets[2]<0){
    			printf("No sufficient tickets are available in this category.\n");
    			Close(des);
    			return NULL;
    		}
    	
    	}
    	else{
    		printf("No sufficient tickets are available in this category.\n");
    		Close(des);
    		return NULL;
    	}
    	FILE *mydata2 = fopen("data.txt", "w");
    	if (mydata2 == NULL) {
        	printf("Data File does not exist\n");
        	Close(des);
        	return NULL;
    	}
    	fprintf(mydata2,"%d\n%d\n%d",n_tickets[0],n_tickets[1],n_tickets[2]);
    	fclose(mydata2);
    	FILE *mydata3=fopen("file.txt","w");
    	if (mydata3 == NULL) {
        	printf("Mail File does not exist\n");
        	Close(des);
        	return NULL;
    	}
    	fprintf(mydata3,"To: %s\nFrom: 2021csm1001@iitrpr.ac.in\nSubject: Tickets confirmed\n\nDear %s,\nThe ticket booking was successful. Ticket numbers: %s. Total tickets booked are %d. ",email, name, t, quantity);
    	fclose(mydata3);
    	char command[100];
    	sprintf(command,"sendmail %s < file.txt",email);
    	system(command);
    	printf("<%s>-The ticket booking was successful. Ticket numbers: %s.Total tickets booked %d. Sent to <%s> \n",name,t,quantity,email);
    	Close(des);
    	return NULL;
}

 int main(int argc, char **argv) 
{
	if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(1);
    	}
    	char hostname[MAXLINE], port[MAXLINE];
    	pthread_t tid;
    	socklen_t clientsize;
    	struct sockaddr_storage clientaddr;
    	sem_t mutex;
	int mylisten, *myconn;
	Sem_init(&mutex, 0, 1);
    	mylisten = Open_listenfd(argv[1]);
    	while (1) {
		clientsize = sizeof(clientaddr);
		myconn = Malloc(sizeof(int));
		*myconn = Accept(mylisten, (SA *)&clientaddr, &clientsize); 
        	Getnameinfo((SA *) &clientaddr, clientsize, hostname, MAXLINE, port, MAXLINE, 0);
        	printf("Accepted connection from (%s, %s)\n", hostname, port);
        	P(&mutex);      
        	Pthread_create(&tid, NULL, ticket, myconn);      
        	V(&mutex);                                                                 
    	}
}

