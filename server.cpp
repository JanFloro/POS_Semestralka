#include "server.h"

using namespace std;


int citaniePodmienkaServer;

int server(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[512];

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }

    //-------------------------------------------Main Kód---------------------------------

    int sirka, vyska, pocetMravcov, typMravcov, pocetZM, typGenerovaniaPlohy, pocetChyb = 0, typGenerovaniaMapy;
    int pocetCiernichPloch;
    DAT data = {.typMravcov = typMravcov, .pocetMravcov = pocetMravcov, .vyska = vyska, .sirka = sirka} ;

    //---------Server sa opyta ci chce client vytvorit mapu alebo nacitat zo suboru
    PosielanieNaCLienta(" Vyber si moznost - \n 1.Vytvorit mapu: \n 2.Nacitat mapu zo suboru: \n ",n,newsockfd);
    typGenerovaniaPlohy = CitanieZClienta(buffer,n,newsockfd);


    if (typGenerovaniaPlohy == 1) {
        // Vyska
        PosielanieNaCLienta("Zadaj vysku mapy min 5: \n",n,newsockfd);
        vyska = CitanieZClienta(buffer,n,newsockfd);
        data.vyska = vyska;
        //Sirka

        PosielanieNaCLienta("Zadaj sirku mapy min 5: \n",n,newsockfd);
        sirka = CitanieZClienta(buffer,n,newsockfd);
        data.sirka = sirka;

        //--------------------------------------------Generovanie mapy
        PosielanieNaCLienta(" Vyber si moznost - \n 1.Nahodne cierne policka: \n 2.Rucne napisat cierne policka: \n ",n,newsockfd);
        typGenerovaniaMapy = CitanieZClienta(buffer,n,newsockfd);

        if (typGenerovaniaMapy == 1) {
            srand(time(0));

            //----------Hracie pole Inicializacia
            int **pole = new int* [vyska];
            for (int i = 0; i < vyska; i++) {
                pole[i] = new int [sirka];
            }
                                                            // 0 CIERNA 1 BIELA
            for (int i = 0; i < vyska; ++i) {
                for (int j = 0; j < sirka; ++j) {
                    if ((rand() % 2) == 0 ) {
                        pole[i][j] = CIERNA;
                    } else {
                        pole[i][j] = BIELA;
                    }
                }
            }

            data.pole = pole;

        } else {
            //----------------------------------------Urceni pocet ciernich policok
            int **pole = new int* [vyska];
            for (int i = 0; i < vyska; i++) {
                pole[i] = new int [sirka];
            }
            // 0 CIERNA 1 BIELA  Cela mapa je biela
            for (int i = 0; i < vyska; ++i) {
                for (int j = 0; j < sirka; ++j) {
                    pole[i][j] = BIELA;
                }
            }

            PosielanieNaCLienta("Kolko ciernich ploch chces mat : \n",n,newsockfd);
            pocetCiernichPloch = CitanieZClienta(buffer,n,newsockfd);

            for (int i = 0; i < pocetCiernichPloch; ++i) {
                int surX, surY;
                PosielanieNaCLienta("Zadaj suradnicu X :", n, newsockfd);
                surX = CitanieZClienta(buffer,n,newsockfd);
                PosielanieNaCLienta("Zadaj suradnicu Y :", n, newsockfd);
                surY = CitanieZClienta(buffer,n,newsockfd);
                pole[surX][surY] = CIERNA;
            }
            data.pole = pole;
        }

        //------------------------------Vykresli mapu
        for (int i = 0; i < sirka; ++i) {
            for (int j = 0; j < vyska; ++j) {
                printf(" %d ", data.pole[i][j]);
            }
            printf(" \n ");
        }

        int ulozit;
        PosielanieNaCLienta("Chces mapu ulozit: \n 1.ANO  \n 2.NIE  \n",n,newsockfd);
        ulozit = CitanieZClienta(buffer,n,newsockfd);

        if (ulozit == 1) {
            //  --------------------------TODO Ulozenie mapy do suboru-----------------
        }

    } else {
        //TODO     -------------------------Nacitanie mapy zo suboru------------------------------
    }


    //----------Server sa opyta na pocet mravcou a pyta sa to stale dokym client neda rozumne cislo
    PosielanieNaCLienta("Zadaj kolko mravcou chces mat na mape: \n",n,newsockfd);
    pocetMravcov = CitanieZClienta(buffer,n,newsockfd);
    data.pocetMravcov = pocetMravcov;


    //---------------------------------------------Typ mravcov--
    PosielanieNaCLienta("Zadaj aky typ mravcou chces : \n 1.Priamich: \n 2.Inverznich: \n ",n,newsockfd);
    typMravcov = CitanieZClienta(buffer,n,newsockfd);
    data.typMravcov = typMravcov;

    //-------------------------------------Pozicie mravcov-----------------------------
    int rozhodnutie;
    PosielanieNaCLienta("Pozicie mravcou : \n 1.Nahodna: \n 2.Vlastna: \n ",n,newsockfd);
    rozhodnutie = CitanieZClienta(buffer,n,newsockfd);
    //----------------------------------Inicializacia----------
    int **poleMravcov = new int* [vyska];
    for (int i = 0; i < vyska; i++) {
        poleMravcov[i] = new int [sirka];
    }
    //-----------------------------Naplnenie mapy mravcov-----------------
    for (int i = 0; i < vyska; ++i) {
        for (int j = 0; j < sirka; ++j) {
            poleMravcov[i][j] = 9;
        }
    }

    if (rozhodnutie == 1) {
        int x, y;
        srand(time(0));
        for (int i = 0; i < pocetMravcov; ++i) {
            poleMravcov[rand() % vyska] [rand() % sirka] = 2;
        }

        data.poleMravcov = poleMravcov;

        } else {
        for (int i = 0; i < pocetMravcov; ++i) {
            int surX, surY;
            PosielanieNaCLienta("Zadaj suradnicu X :", n, newsockfd);
            surX = CitanieZClienta(buffer,n,newsockfd);
            PosielanieNaCLienta("Zadaj suradnicu Y :", n, newsockfd);
            surY = CitanieZClienta(buffer,n,newsockfd);
            poleMravcov[surX][surY] = 2;        //TODO  Mozno constanta na mnavca
        }
    }
    //------------------------------Vykresli mapu mravcov----------------------
    for (int i = 0; i < sirka; ++i) {
        for (int j = 0; j < vyska; ++j) {
            printf(" %d ", data.poleMravcov[i][j]);
        }
        printf(" \n ");
    }


    printf("Pocet mravcov : %d \n", data.pocetMravcov);
    printf("Typ mravcoc : %d \n", data.typMravcov);
    printf("Vyska : %d \n", data.vyska);
    printf("Pocet sirka : %d \n", data.sirka);
    //----------------------------------------Koniec MAIN-----------------------------------


    /*
    //------Test vypis
           for (int i = 0; i < sirka; ++i) {
               for (int j = 0; j < vyska; ++j) {
                   printf(" %d ", data.pole[i][j]);
               }
               printf(" \n ");
           }
    */



    close(newsockfd);
    close(sockfd);

    return 0;
}

int CitanieZClienta(char buffer[256], int n, int newsockfd)
{
    bzero(buffer,256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
    }
    int cislo = atoi(buffer);
    return cislo;
}

void PosielanieNaCLienta(char sprava[512], int n, int newsockfd) {
    n = write(newsockfd, sprava, strlen(sprava));
    if (n < 0) {
        perror("Error writing to socket");
    }
}