#include <string.h>
#include <stdio.h>
#include "server.h"
#include "client.h"

int main (int argc, char* argv[]){

    int pocetPrvok = argc-1;

    char* noveArgumenty[pocetPrvok];

    if(pocetPrvok ==2)
    {
        noveArgumenty[0] = argv[1];
        noveArgumenty[1] = argv[2];
    }
    else if(pocetPrvok == 3)
    {
        noveArgumenty[0] = argv[1];
        noveArgumenty[1] = argv[2];
        noveArgumenty[2] = argv[3];
    }

    if(pocetPrvok <=3)
    {
        if(strcmp(argv[1], "client") == 0){
            printf("Zapinam klienta\n");
            client(argc,noveArgumenty);
        }
        else if(strcmp(argv[1], "server") == 0){
            printf("Zapinam server\n");
            server(argc, noveArgumenty);
        }
    } else
    {
        printf("Nesprávny počet parametrov !");
    }
    return 0;

}