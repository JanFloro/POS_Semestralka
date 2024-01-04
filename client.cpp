#include "client.h"

using namespace std;

int hodnota;

void vykresli();

int CitanieZoServera(char buffer[512], int n, int sockfd)
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
    int nacitanie;
    int pocetCiernichPloch;
    int surPolickaX, surPolickaY;

    DATA d = {};

    cout << "-----------\nLangtonov mravec\n-----------\n";
    cout << "Chces nacitat mapu zo suboru: \n  1,ANO\n  2,NIE \n";
    cin >> nacitanie;

    if (nacitanie == 1 ) {
        // TODO nacitanie zo servera
    } else {
        cout << "Paradicka tak si pome vytvorit mapu.\n";
        cout << "Zadaj maximalnu vysku: \n";
        cin >> d.vyska;


        cout << "Zadaj maximalnu sirku: \n";
        cin >> d.sirka;

        cout << "Vyber sposob vygenerovania mapy: \n  1,Nahodna\n  2,Vlastna \n";
        cin >> typGenerovania;

        //-----------------------------------Generovanie mapy--------------------------

        //--------------------------------Inicializacia bool pola
        d.pole = new bool*[d.vyska];
        for (int i = 0; i < d.vyska; ++i) {
            d.pole[i] = new bool[1];
        }
        //--------------------------------Nastavenie pola na false
        for (int i = 0; i < d.vyska; ++i) {
            for (int j = 0; j < d.sirka; ++j) {
                d.pole[i][j] = false;
            }
        }


        if (typGenerovania == 1) {
            //--------------------Nahodne generovanie mapy
            srand(time(0));
            for (int i = 0; i < d.vyska; ++i) {
                for (int j = 0; j < d.sirka; ++j) {
                    if (rand() % 2 == 1) {
                        d.pole[i][j] = true;
                    }
                }
            }
        } else {
            //--------------------Vlastne generovanie mapy

            cout << "Kolko ciernich policok chces: \n";
            cin >> pocetCiernichPloch;

            for (int i = 0; i < pocetCiernichPloch; ++i) {
                cout << "Zadaj suradnicu X: \n";
                cin >> surPolickaX;
                cout << "Zadaj suradnicu Y: \n";
                cin >> surPolickaY;
                d.pole[surPolickaX][surPolickaY] = true;
            }
        }
        //-----------------------------------------Opytanie sa ci chce ulozit mapu
        cout << "-------------------Vykreslenie mapy------------------- \n";
        vykresli(&d);

        cout << "Chces ulozit mapu do suboru : \n  1,ANO\n  2,NIE\n";
        int ulozenie;
        cin >> ulozenie;

        if (ulozenie == 1) {
            //--------------------TODO ulozenie mapy na server
        }
    }

    //-------------------------------Mravci--------------------------------------------------------------------

    //-------------------------Pocet mravcov na mape
    cout << "Kolko mravcov chces mat na mape: \n";
    cin >> d.pocetMravcov;

    d.poleMravcov= new int* [d.vyska];
    for (int i = 0; i < d.sirka; i++) {
        d.poleMravcov[i] = new int[4];
    }


    //------------------------Logika mravcov
    cout << "Vyber si logiku mravcov:  \n  1,Priama\n  2,Inverzna\n";
    cin >> d.typMravcov;

    //-------------------------------Generovanie pozicie mravcov
    cout << "Generovanie pozicie mravcov:  \n  1,Nahodne\n  2,Vlastne\n";
    int genMravci;
    cin >> genMravci;
    //---------------------------Nahodne generovanie pozicie
    if (genMravci == 1) {
        srand(time(0));
        for (int i = 0; i < d.pocetMravcov; ++i) {
            d.poleMravcov[i][0] = rand() % d.vyska; //Pozicia X
            d.poleMravcov[i][1] = rand() % d.sirka; //Pozicia Y
            d.poleMravcov[i][2] = rand() % 4; //Smer pohibu 0-3
            d.poleMravcov[i][3] = 1; // 1 zivy mravec 2 mrtvy mravec
        }
    } else {
        for (int i = 0; i < d.pocetMravcov; ++i) {
            int surX, surY;
            cout << "Zadaj suradnicu Y:  \n";
            cin >> surX;
            cout << "Zadaj suradnicu X:  \n";
            cin >> surY;
            d.poleMravcov[i][0] = surX; //Pozicia X
            d.poleMravcov[i][1] = surY; //Pozicia Y
            d.poleMravcov[i][2] = rand() % 4; //Smer pohybu 0-3
            d.poleMravcov[i][3] = 1; // 1 zivy mravec 2 mrtvy mravec
        }
    }

    cout << "---------------------Mapa aj s mravcami -----------------\n";
    vykresli(&d);
    cout << "---------------------Simulacia sa pusta -----------------\n";

    while (true){
        posunMravcov(&d);
        vykresli(&d);
        sleep(3);
    }

    close(sockfd);
    return 0;
}


//[i][0] = Pozicia Y
//[i][1] = Pozicia X
//[i][2] = Smer pohibu 0-3
//[i][3] = 1;

void vykresli(DATA* d) {
    for (int i = 0; i < d->vyska; ++i) {
        cout << '|';
        for (int j = 0; j < d->sirka; ++j) {
            char symbol = (d->pole[i][j] ? '#' : ' ');
            for (int k = 0; k < d->pocetMravcov; ++k) {
                if (d->poleMravcov[k][1] == j && d->poleMravcov[k][0] == i) {
                    symbol = '*'; //-----------------------Znacka mravca
                    break;
                }
            }
            cout << symbol;
        }
        cout << '|';
        cout << '\n';
    }
    cout << '\n';
}

void posunMravcov(DATA* d) {
    if (d->typMravcov == 1) {
        //-------------------------------Priama logika
        for (int i = 0; i < d->pocetMravcov; ++i) {
            int& x = d->poleMravcov[i][1]; // -----------------------Suradnica X
            int& y = d->poleMravcov[i][0]; // ---------------------Suradnica Y
            int& smer = d->poleMravcov[i][2]; // --------------------Smer
            int& stav = d->poleMravcov[i][3]; // ---------------------Stav mravenca zivy alebo mrtvy


            if (stav == 1) {
                bool farba = !d->pole[y][x]; //    Zistim aka je aktualna farba

                // -----------------------------Zmeni smer na podla farby policka
                smer = farba ? (smer + 1) % 4 : (smer + 3) % 4;



                // ---------------------Zmeni aktualnu farbu na opacnu
                d->pole[y][x] = !d->pole[y][x];

                // -------------Posun mravca
                switch (smer) {
                    case 0:
                        y = (y - 1 + d->vyska) % d->vyska; break; // HORE
                    case 1:
                        x = (x + 1) % d->sirka; break; // VPRAVO
                    case 2:
                        y = (y + 1) % d->vyska; break; // DOLE
                    case 3:
                        x = (x - 1 + d->sirka) % d->sirka; break; // VLAVO
                    default: cout << "Chyba Upsss";
                }
            }
        }
    } else {
        //------------------------------Inverzna logika
        for (int i = 0; i < d->pocetMravcov; ++i) {
            int& x = d->poleMravcov[i][1]; // -----------------------Suradnica X
            int& y = d->poleMravcov[i][0]; // ---------------------Suradnica Y
            int& smer = d->poleMravcov[i][2]; // --------------------Smer
            int& stav = d->poleMravcov[i][3]; // ---------------------Stav mravenca zivy alebo mrtvy


            if (stav == 1) {
                bool farba = d->pole[y][x]; //    Zistim aka je aktualna farba

                // -----------------------------Zmeni smer na podla farby policka
                smer = farba ? (smer + 1) % 4 : (smer + 3) % 4;



                // ---------------------Zmeni aktualnu farbu na opacnu
                d->pole[y][x] = !d->pole[y][x];

                // -------------Posun mravca
                switch (smer) {
                    case 0:
                        y = (y - 1 + d->vyska) % d->vyska; break; // HORE
                    case 1:
                        x = (x + 1) % d->sirka; break; // VPRAVO
                    case 2:
                        y = (y + 1) % d->vyska; break; // DOLE
                    case 3:
                        x = (x - 1 + d->sirka) % d->sirka; break; // VLAVO
                    default: cout << "Chyba Upsss";
                }
            }
        }
    }
}



