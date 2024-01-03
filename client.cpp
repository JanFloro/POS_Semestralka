#include "client.h"
int hodnota;

int citaniePodmienkaClient;

int CitanieZoServera(char buffer[512],int n, int sockfd)
{
    bzero(buffer,512);
    n = read(sockfd, buffer, 511);
    if (n < 0)
    {
        perror("Error reading from socket");
    }
    printf("%s",buffer);
    int cislo = atoi(buffer);
    return cislo;
}

int PosielanieNaServer(char buffer[256], int n, int sockfd)
{
    bzero(buffer,256);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
    }
    int cislo = atoi(buffer);
    hodnota = cislo;
    return cislo;
}

int client(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    int kontrolaPredGenerovanim;

    char buffer[512];

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(argv[2]));


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }

    //---------------------------Main kod-----------------------------------
    //----------Vytvorit mapu alebo nacitat mapu zo suboru
    int typGenerovania;
    int vyska, sirka;
    int pocetCiernichPloch;
    int surX, surY;

    CitanieZoServera(buffer,n,sockfd);
    typGenerovania = PosielanieNaServer(buffer,n,sockfd);



    if (typGenerovania == 1 ) {
       // Vyska
       CitanieZoServera(buffer,n,sockfd);
       vyska = PosielanieNaServer(buffer,n,sockfd);

       //sirka
       CitanieZoServera(buffer,n,sockfd);
       sirka = PosielanieNaServer(buffer,n,sockfd);

    }

    //---------------Generovanie mapy
    CitanieZoServera(buffer,n,sockfd);
    hodnota = PosielanieNaServer(buffer,n,sockfd);

    if (hodnota == 2) {
        //----------------Vlastne cierne policka
        CitanieZoServera(buffer,n,sockfd);
        pocetCiernichPloch = PosielanieNaServer(buffer,n,sockfd);

        for (int i = 0; i < pocetCiernichPloch; ++i) {
            CitanieZoServera(buffer, n, sockfd);
            surX = PosielanieNaServer(buffer,n,sockfd);
            CitanieZoServera(buffer, n, sockfd);
            surY = PosielanieNaServer(buffer,n,sockfd);
        }
    }

/*
    //----------Pocet mravcov
    do {
        CitanieZoServera(buffer,n,sockfd);
        citaniePodmienkaClient = PosielanieNaServer(buffer,n,sockfd);
    } while (citaniePodmienkaClient == 0);

    //---------Typ mravcov
    do {
        CitanieZoServera(buffer,n,sockfd);
        hodnota = PosielanieNaServer(buffer,n,sockfd);
        if (hodnota == 1 || hodnota == 2) {
            break;
        }
    } while (true);



*/


    close(sockfd);
    return 0;
}