/* 
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2019/2020
    Semestar:       Zimski (V)
    
    Ime fajla:      server.c
    Opis:           TCP/IP server
    
    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include <stdlib.h>
#include <stdbool.h>
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

int vrste[3]={0,0,0};
int kolone[3]={0,0,0};
int gd=0;
int sd=0;
int brojac=0;
int winnerx=0;
int winnero=0;
int draw=0;

int game_over(int i, int j, int potez, int brojac) {

    int dodaj;
    if(potez==0){
        dodaj=1;
    }
    else{
        dodaj=-1;
    }
    vrste[i]+=dodaj;
    if(vrste[i]==3){
        winnerx=1;
        return 1;
    }
    if(vrste[i]==-3) {
        winnero=1;
        return 1;
    }
    kolone[j]+=dodaj;
    if(kolone[j]==3){
        winnerx=1;
        return 1;
    }
    if(kolone[j]==-3) {
        winnero=1;
        return 1;
    }
    if(i==j){
        gd+=dodaj;
    }
    if(gd==3){
        winnerx=1;
        return 1;
    }
    if(gd==-3) {
        winnero=1;
        return 1;
    }
    if(i+j==2){
        sd+=dodaj;
    }
    if(sd==3){
        winnerx=1;
        return 1;
    }
    if(sd==-3) {
        winnero=1;
        return 1;
    }
    if(brojac == 9) {
        draw=1;
        return 1;
    }
    return 0;
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock[2] , c;
    struct sockaddr_in server , client[2];
    char client_message1[DEFAULT_BUFLEN];
    char client_message2[DEFAULT_BUFLEN];
    char *player_x = "You are player X";
    char *player_y = "You are player O";
    char *your_move = "It's your turn";
    char *win_message="You won! ";
    char *lose_message="You lost ";
    char *draw_message="It's draw";

    char a[3][3];

    for(int l=0; l<3; l++) {
        for(int k=0; k<3; k++) {
            a[l][k]='_';
        }
    }

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 2);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock[0] = accept(socket_desc, (struct sockaddr *)&client[0], (socklen_t*)&c);
    if (client_sock[0] < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted. Player X is connected.");

    //accept connection from an incoming client
    client_sock[1] = accept(socket_desc, (struct sockaddr *)&client[1], (socklen_t*)&c);
    if (client_sock[1] < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted. Player O is connected.");

    if( send(client_sock[0] , player_x , strlen(player_x), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

    if( send(client_sock[1] , player_y , strlen(player_y), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

    int turn=1;
    int go=1;
    int i,j,x;

    while(go) {

	if(turn==1) {
            if( send(client_sock[0] , a , strlen(*a), 0) < 0)
    	     {
        		puts("Send failed");
        		return 1;
   	     }

	    if( send(client_sock[0] , your_move , strlen(your_move), 0) < 0)
    	     {
        		puts("Send failed");
        		return 1;
   	     }

            if(recv(client_sock[0] , client_message1 , DEFAULT_BUFLEN , 0) < 0)
	    {
			puts("Receive failed");
			return 1;
	    }
	    x=atoi(client_message1);
	    i=x/10;
	    j=x%10;
	    if(a[i][j]=='_') {
	    	a[i][j]='X';
	    } else {
		send(client_sock[0], 
	    if( send(client_sock[0] , a , strlen(*a), 0) < 0)
             {
                        puts("Send failed");
                        return 1;
             }

	    turn--;
        } else {
            if( send(client_sock[1] , a , strlen(*a), 0) < 0)
    	     {
        		puts("Send failed");
        		return 1;
   	     }
	     if( send(client_sock[1] , your_move , strlen(your_move), 0) < 0)
    	     {
        		puts("Send failed");
        		return 1;
   	     }
	     if(recv(client_sock[1] , client_message2 , DEFAULT_BUFLEN , 0) < 0 )
             {
			puts("Receive failed");
			return 1;
	     }
	     x=atoi(client_message2);
	     i=x/10;
	     j=x%10;
	     a[i][j]='O';

	     if( send(client_sock[1] , a , strlen(*a), 0) < 0)
             {
                        puts("Send failed");
                        return 1;
             }

             turn++;
        }
	brojac++;
	go=!game_over(i,j, turn, brojac);

	if(go==0){
		char *signal="game_over";
		for(int l=0; l<2; l++) {
			send(client_sock[l], signal, strlen(signal), 0);
		}
	}
    }

    for(int l=0; l<2; l++) {
    	if( send(client_sock[l] , a , strlen(*a), 0) < 0)
   	{
       		puts("Send failed");
                return 1;
    	}
    }

    if(winnerx) {
	if(send(client_sock[0] , win_message  , strlen(win_message), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	if(send(client_sock[1] , lose_message  , strlen(lose_message), 0) < 0)
	{
                puts("Send failed");
		return 1;
        }
    } else if(winnero) {
	send(client_sock[1] , win_message  , strlen(win_message), 0);
        send(client_sock[0] , lose_message  , strlen(lose_message), 0);
    } else if(draw) {
	if(send(client_sock[0] , draw_message  , strlen(draw_message), 0) < 0)
	{
                puts("Send failed");
                return 1;
        }
        if(send(client_sock[1] , draw_message  , strlen(draw_message), 0) < 0)
	{
                puts("Send failed");
                return 1;
        }
    }
    close(client_sock[0]);
    close(client_sock[1]);
    close(socket_desc);

    return 0;
}



