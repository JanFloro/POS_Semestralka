#include "client.h"

using namespace std;





void inicializaciaDat(DATA* data, int typMravcov, int pocetMravcov, int vyska, int sirka, bool **pole, int **poleMravcov, int pocetZivich) {
    data->typMravcov = typMravcov;
    data->pocetMravcov = pocetMravcov;
    data->vyska = vyska;
    data->sirka = sirka;
    data->pole = pole;
    data->poleMravcov = poleMravcov;
    data->pocetZivich = pocetZivich;
    pthread_mutex_init(&data->mutexData, NULL);
    data->ukoncenie = false;
    data->pauznutie= false;
}

void odstranenieDat (DATA * data) {
    pthread_mutex_destroy(&data->mutexData);

    for (int i = 0; i < data->vyska; ++i) {
        for (int j = 0; j < data->sirka; ++j) {
            // TODO ostranenie poli
        }
    }
}

void CitanieZoServera(char buffer[512], int sockfd)
{
    bzero(buffer,512);
    int n = read(sockfd, buffer, 511);
    if (n < 0)
    {
        perror("Error reading from socket");
    }
}

void PosielanieNaServer(char buffer[512], int sockfd)
{
    bzero(buffer,512);
    fgets(buffer, 512, stdin);
    int n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
    }
}

void ulozenieMapyDoSuboru(bool** pole, int vyska, int sirka) {
    ofstream outputFile("Mapa.txt");

    if (!outputFile.is_open()) {
        cerr << "Chyba otvorenia suboru: " << "Mapa.txt" << endl;
        return;
    }

    outputFile << vyska << endl;
    outputFile << sirka << endl;

    for (int i = 0; i < vyska; ++i) {
        for (int j = 0; j < sirka; ++j) {
            outputFile << (pole[i][j] ? '1' : '0');
        }
        outputFile << endl;
    }

    outputFile.close();
    cout << "Mapa bola ulozena " << "Mapa.txt" << endl;
}

bool** nacitanieMapyZoSuboru1(int& vyska, int& sirka) {
    ifstream inputFile("Mapa.txt");

    if (!inputFile.is_open()) {
        cerr << "Neotvoril sa subor: " << "Mapa.txt" << endl;
        return nullptr;
    }

    inputFile >> vyska;
    //printf("%d\n", vy);
    inputFile >> sirka;
    //printf("%d\n", si);

    // Vytvori 2d pole
    bool** pole = new bool*[vyska];
    for (int i = 0; i < vyska; ++i) {
        pole[i] = new bool[sirka];
    }

    for (int i = 0; i < vyska; ++i) {
        for (int j = 0; j < sirka; ++j) {
            char cellContent;
            inputFile >> cellContent;
            cellContent == '1' ? pole[i][j] = true : pole[i][j] = false;
        }
    }

    inputFile.close();
    cout << "Mapa bola uspesne nacitana: " << "Mapa.txt" << endl;

    return pole;
}


int client(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

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


    int typMravcov, pocetMravcov;
    int vyska,sirka;
    bool **pole;
    int **poleMravcov;
    int pocetZivich;


    cout << "----------------------\n    Langtonov mravec\n----------------------\n";
    do {
        cout << "Chces nacitat mapu : \n  1,Server\n  2,Lokalny subor\n  3,NIE\n";
        if (!(cin >> nacitanie)) {
            cout << "Nespravny input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (nacitanie < 1 || nacitanie > 3);



    if (nacitanie == 1 ) {
        // -----------------------------------------TODO nacitanie zo servera
        sprintf(buffer, "%d", nacitanie);
        cout << "Client sending message: " << buffer << endl;
        PosielanieNaServer(buffer, sockfd);

        //PosielanieNaServer("1",sockfd);


        CitanieZoServera(buffer, sockfd);
        vyska = atoi(buffer);
        PosielanieNaServer(buffer,sockfd);

        CitanieZoServera(buffer, sockfd);
        sirka = atoi(buffer);
        PosielanieNaServer(buffer,sockfd);

        for (int i = 0; i < vyska; ++i) {
            for (int j = 0; j < sirka; ++j) {
                int hodnota;
                CitanieZoServera(buffer, sockfd);
                hodnota = atoi(buffer);
                hodnota == 1 ? pole[i][j] = true : pole[i][j] = false;
                /*
                if (hodnota == 1) {
                    pole[i][j] = true;
                } else {
                    pole[i][j] = false;
                }*/
                PosielanieNaServer(buffer,sockfd);
            }
        }


    } else if (nacitanie == 2 ) {
        pole = nacitanieMapyZoSuboru1(vyska, sirka);
        //printf("%d\n", vyska);
        //printf("%d\n", sirka);
    } else {
        cout << "Paradicka tak si pome vytvorit mapu.\n";
        do {
            cout << "Zadaj maximalnu vysku [5-100]: \n";
            if (!(cin >> vyska)) {
                cout << "Nespravny input!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (vyska < 5 || vyska > 100);

        do {
            cout << "Zadaj maximalnu sirku [5-100] : \n";
            if (!(cin >> sirka)) {
                cout << "Nespravny input!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (sirka < 5 || sirka > 100);

        do {
            cout << "Vyber sposob vygenerovania mapy: \n  1,Nahodna\n  2,Vlastna \n";
            if (!(cin >> typGenerovania)) {
                cout << "Nespravny input!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (typGenerovania < 1 || typGenerovania > 2);


        //-----------------------------------Generovanie mapy--------------------------

        //--------------------------------Inicializacia bool pola
        pole = new bool*[vyska];
        for (int i = 0; i < vyska; ++i) {
            pole[i] = new bool[sirka];
        }
        //--------------------------------Nastavenie pola na false
        for (int i = 0; i < vyska; ++i) {
            for (int j = 0; j < sirka; ++j) {
                pole[i][j] = false;
            }
        }


        if (typGenerovania == 1) {
            //--------------------Nahodne generovanie mapy
            srand(time(0));
            for (int i = 0; i < vyska; ++i) {
                for (int j = 0; j < sirka; ++j) {
                    if (rand() % 2 == 1) {
                        pole[i][j] = true;
                    }
                }
            }
        } else {
            //--------------------Vlastne generovanie mapy

            do {
                cout << "Kolko ciernich policok chces: \n";
                if (!(cin >> pocetCiernichPloch)) {
                    cout << "Nespravny input!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (pocetCiernichPloch < 0  || pocetCiernichPloch > (vyska * sirka));



            for (int i = 0; i < pocetCiernichPloch; ++i) {
                do {
                    cout << "Zadaj suradnicu Y: \n";
                    if (!(cin >> surPolickaX)) {
                        cout << "Nespravny input!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (surPolickaX < 0 || surPolickaX > vyska);

                do {
                    cout << "Zadaj suradnicu X: \n";
                    if (!(cin >> surPolickaY)) {
                        cout << "Nespravny input!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (surPolickaY < 0 || surPolickaY > sirka);

                pole[surPolickaX][surPolickaY] = true;
            }
        }

        //-----------------------------------------Opytanie sa ci chce ulozit mapu
        cout << "-------------------Vykreslenie mapy------------------- \n";

        //-----------------------------Vykresli zakladnu mapu
        printf("------------------------\n");
        for (int i = 0; i < vyska; ++i) {
            cout << '|';
            for (int j = 0; j < sirka; ++j) {
                char symbol = (pole[i][j] ? '#' : ' ');
                cout << symbol;
            }
            cout << '|';
            cout << '\n';
        }
        printf("------------------------\n");





        int ulozenie;
        do {
            cout << "Chces ulozit mapu : \n  1,NIE\n  2,Lokalny subor\n  3,Server\n";
            if (!(cin >> ulozenie)) {
                cout << "Nespravny input!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (ulozenie < 1 || ulozenie > 3);


        if (ulozenie == 3) {
            sprintf(buffer,"%d", ulozenie);
            PosielanieNaServer(buffer,sockfd);

            //--------------------TODO ulozenie mapy na server  OTESTOVAT
            sprintf(buffer,"%d",vyska);
            PosielanieNaServer(buffer, sockfd);
            CitanieZoServera(buffer,sockfd);

            sprintf(buffer,"%d",sirka);
            PosielanieNaServer(buffer, sockfd);
            CitanieZoServera(buffer,sockfd);

            for (int i = 0; i < vyska; ++i) {
                for (int j = 0; j < sirka; ++j) {
                    if (pole[i][j]) {
                        int hodnota = 1;
                        sprintf(buffer,"%d", hodnota);
                        PosielanieNaServer(buffer,sockfd);
                        CitanieZoServera(buffer,sockfd);
                    } else {
                        int hodnota = 0;
                        sprintf(buffer,"%d", hodnota);
                        PosielanieNaServer(buffer,sockfd);
                        CitanieZoServera(buffer,sockfd);
                    }
                }
            }


        }
        if (ulozenie == 2) {
            ulozenieMapyDoSuboru(pole,vyska,sirka);
        }
    }

    //-------------------------------Mravci--------------------------------------------------------------------

    //-------------------------Pocet mravcov na mape
    do {
        cout << "Kolko mravcov chces mat na mape: \n";
        if (!(cin >> pocetMravcov)) {
            cout << "Nespravny input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (pocetMravcov < 0 || pocetMravcov > (sirka * vyska));
    pocetZivich = pocetMravcov;

    poleMravcov= new int* [vyska];
    for (int i = 0; i < sirka; i++) {
        poleMravcov[i] = new int[4];
    }


    //------------------------Logika mravcov
    do {
        cout << "Vyber si logiku mravcov:  \n  1,Priama\n  2,Inverzna\n";
        if (!(cin >> typMravcov)) {
            cout << "Nespravny input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (typMravcov < 1 || typMravcov > 2);


    //-------------------------------Generovanie pozicie mravcov
    int genMravci;
    do {
        cout << "Generovanie pozicie mravcov:  \n  1,Nahodne\n  2,Vlastne\n";
        if (!(cin >> genMravci)) {
            cout << "Nespravny input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (genMravci < 1 || genMravci > 2);

    //---------------------------Nahodne generovanie pozicie
    if (genMravci == 1) {
        srand(time(0));
        for (int i = 0; i < pocetMravcov; ++i) {
            poleMravcov[i][0] = rand() % vyska; //Pozicia X
            poleMravcov[i][1] = rand() % sirka; //Pozicia Y
            poleMravcov[i][2] = rand() % 4; //Smer pohibu 0-3
            poleMravcov[i][3] = 1; // 1 zivy mravec 2 mrtvy mravec
        }
    } else {
        for (int i = 0; i < pocetMravcov; ++i) {
            int surX, surY;
            do {
                cout << "Zadaj suradnicu Y:  \n";
                if (!(cin >> surX)) {
                    cout << "Nespravny input!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (surX < 0 || surX > sirka);

            do {
                cout << "Zadaj suradnicu X:  \n";
                if (!(cin >> surY)) {
                    cout << "Nespravny input!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (surY < 0 || surY > sirka);


            poleMravcov[i][0] = surX; //Pozicia X
            poleMravcov[i][1] = surY; //Pozicia Y
            poleMravcov[i][2] = rand() % 4; //Smer pohybu 0-3
            poleMravcov[i][3] = 1; // 1 zivy mravec 2 mrtvy mravec
        }
    }

    DATA data;
    pthread_t client;
    inicializaciaDat(&data,typMravcov,pocetMravcov,vyska,sirka,pole,poleMravcov,pocetZivich);

    cout << "---------------------Mapa aj s mravcami -----------------\n";
    vykresli(&data);

    cout << "---------------------Simulacia sa pusta -----------------\n";

    //hra(&data);
    pthread_create(&client, nullptr,hra,&data);
    //pthread_join(client, NULL);

    while(!data.ukoncenie) {
        int znak = getchar();
        //------------------------------Pauzne simulaciu stlacenim a znaku
        if (znak == 97) {
            pthread_mutex_lock(&data.mutexData);
            data.pauznutie = true;
            printf("-----PAUZA-----\n");
            printf("PRE POKRACOVANIE STLAC u \n");
            printf("PRE KONIEC STLAC  e\n");
            int key;
            do {
                key = getchar();
                //------------------------------Pokracovanie stacenim u znaku
                if (key == 117) {
                    data.pauznutie = false;
                    break;
                }
                if (key == 101) {
                    printf("-----KONIEC-----\n");
                    data.ukoncenie = true;
                    break;
                }

            } while ( key != 117 && key != 101);
            pthread_mutex_unlock(&data.mutexData);

        }
        if (znak == 101) {
            //----------------------------------------Ukoncenie e znakom
            pthread_mutex_lock(&data.mutexData);
            printf("-----KONIEC-----\n");
            data.ukoncenie = true;
            pthread_mutex_unlock(&data.mutexData);
        }
    }

    odstranenieDat(&data);

    close(sockfd);

    return 0;

}

void vykresli(void* data) {
    DATA* d = (DATA*) data;

    printf("------------------------\n");
    for (int i = 0; i < d->vyska; ++i) {
        cout << '|';
        for (int j = 0; j < d->sirka; ++j) {
            char symbol = (d->pole[i][j] ? '#' : ' ');
            for (int k = 0; k < d->pocetMravcov; ++k) {
                if (d->poleMravcov[k][3] == 1) {  //--------------Kontrola zi je zivy
                    if (d->poleMravcov[k][1] == j && d->poleMravcov[k][0] == i) {
                        symbol = '*'; //-----------------------Znacka mravca
                        break;
                    }
                }
            }
            cout << symbol;
        }
        cout << '|';
        cout << '\n';
    }
    printf("------------------------\n");
    printf("PRE PAUZNUTIE STLAC a\n");
    printf("PRE KONIEC STLAC  e\n");
}

void posunMravcov(void* data) {
    DATA* d = (DATA*) data;

    for (int i = 0; i < d->pocetMravcov; ++i) {
        int &x = d->poleMravcov[i][1]; // -----------------------Suradnica X
        int &y = d->poleMravcov[i][0]; // ---------------------Suradnica Y
        int &smer = d->poleMravcov[i][2]; // --------------------Smer
        int &stav = d->poleMravcov[i][3]; // ---------------------Stav mravenca zivy alebo mrtvy


        if (stav == 1) {
            bool farba = d->pole[y][x]; //    Zistim aka je aktualna farba

            // -----------------------------Zmeni smer na podla farby policka
            smer = farba ? (smer + 1) % 4 : (smer + 3) % 4;



            // ---------------------Zmeni aktualnu farbu na opacnu
            d->pole[y][x] = !d->pole[y][x];
        }

        if (d->typMravcov == 1) {
            //-----------------------Priama logika
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
        } else {
            //-------------------Inverzna logika
            switch (smer) {
                case 0:
                    y = (y - 1 + d->vyska) % d->vyska; break; // HORE
                case 3:
                    x = (x + 1) % d->sirka; break; // VPRAVO
                case 2:
                    y = (y + 1) % d->vyska; break; // DOLE
                case 1:
                    x = (x - 1 + d->sirka) % d->sirka; break; // VLAVO
                default: cout << "Chyba Upsss";
            }
        }


    }

}

void zabiMravca(void* data) {
    DATA* d = (DATA*)data;

    for (int i = 0; i < d->pocetMravcov; ++i) {
        for (int j = i + 1; j < d->pocetMravcov; ++j) {
            // porovnanie policok
            if (d->poleMravcov[i][0] == d->poleMravcov[j][0] && d->poleMravcov[i][1] == d->poleMravcov[j][1]) {
                // Zisti sa ci ziju mravce
                if (d->poleMravcov[i][3] == 1 && d->poleMravcov[j][3] == 1) {
                    cout << "Zrazili sa mravce SAKRA\n";
                    // Nastavenie ze je mrtvy
                    d->poleMravcov[i][3] = 2;
                    // Znizenie poctu zivych mravcov
                    d->pocetZivich--;
                }
            }
        }
    }
}

void* hra(void* data) {
    DATA* d = (DATA*) data;
    while (!d->ukoncenie){
        pthread_mutex_lock(&d->mutexData);
        if(d->ukoncenie) {
            pthread_mutex_unlock(&d->mutexData);
            break;
        }
        pthread_mutex_unlock(&d->mutexData);

        while(true) {
            pthread_mutex_lock(&d->mutexData);
            if(d->pauznutie) {
                pthread_mutex_unlock(&d->mutexData);
                break;
            }
            pthread_mutex_unlock(&d->mutexData);

            vykresli(d);
            posunMravcov(d);
            zabiMravca(d);
            sleep(2);
        }

    }
}




