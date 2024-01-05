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
    /*
     do {
        printf("--->Vyberte spôsob pohybu mravcov po ploche\n-> 1) Priamo\n-> 2) Inverzne\n >>");
        if (!(cin >> typeOfAntsMovement)) {
            cout << "Nesprávny input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (typeOfAntsMovement < 1 || typeOfAntsMovement > 2);


     */
    int typGenerovania;
    int nacitanie;
    int pocetCiernichPloch;
    int surPolickaX, surPolickaY;

    DATA d = {};

    cout << "----------------------\n    Langtonov mravec\n----------------------\n";
    do {
        cout << "Chces nacitat mapu zo suboru: \n  1,ANO\n  2,NIE \n";
        if (!(cin >> nacitanie)) {
            cout << "Nespravny input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (nacitanie < 1 || nacitanie > 2);


    if (nacitanie == 1 ) {
        // -----------------------------------------TODO nacitanie zo servera
    } else {
        cout << "Paradicka tak si pome vytvorit mapu.\n";
        do {
            cout << "Zadaj maximalnu vysku [5-100]: \n";
            if (!(cin >> d.vyska)) {
                cout << "Nespravny input!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (d.vyska < 5 || d.vyska > 100);

        do {
            cout << "Zadaj maximalnu sirku [5-100] : \n";
            if (!(cin >> d.sirka)) {
                cout << "Nespravny input!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (d.sirka < 5 || d.sirka > 100);

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

            do {
                cout << "Kolko ciernich policok chces: \n";
                if (!(cin >> pocetCiernichPloch)) {
                    cout << "Nespravny input!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (pocetCiernichPloch < 0  || pocetCiernichPloch > (d.vyska * d.sirka));



            for (int i = 0; i < pocetCiernichPloch; ++i) {
                do {
                    cout << "Zadaj suradnicu Y: \n";
                    if (!(cin >> surPolickaX)) {
                        cout << "Nespravny input!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (surPolickaX < 0 || surPolickaX > d.vyska);

                do {
                    cout << "Zadaj suradnicu X: \n";
                    if (!(cin >> surPolickaY)) {
                        cout << "Nespravny input!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (surPolickaY < 0 || surPolickaY > d.sirka);

                d.pole[surPolickaX][surPolickaY] = true;
            }
        }

        //-----------------------------------------Opytanie sa ci chce ulozit mapu
        cout << "-------------------Vykreslenie mapy------------------- \n";
        vykresli(&d);

        int ulozenie;
        do {
            cout << "Chces ulozit mapu do suboru : \n  1,ANO\n  2,NIE\n";
            if (!(cin >> ulozenie)) {
                cout << "Nespravny input!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (ulozenie < 1 || ulozenie > 2);


        if (ulozenie == 1) {
            //--------------------TODO ulozenie mapy na server
        }
    }

    //-------------------------------Mravci--------------------------------------------------------------------

    //-------------------------Pocet mravcov na mape
    do {
        cout << "Kolko mravcov chces mat na mape: \n";
        if (!(cin >> d.pocetMravcov)) {
            cout << "Nespravny input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (d.pocetMravcov < 0 || d.pocetMravcov > (d.sirka * d.vyska));


    d.poleMravcov= new int* [d.vyska];
    for (int i = 0; i < d.sirka; i++) {
        d.poleMravcov[i] = new int[4];
    }


    //------------------------Logika mravcov
    do {
        cout << "Vyber si logiku mravcov:  \n  1,Priama\n  2,Inverzna\n";
        if (!(cin >> d.typMravcov)) {
            cout << "Nespravny input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (d.typMravcov < 1 || d.typMravcov > 2);


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
        for (int i = 0; i < d.pocetMravcov; ++i) {
            d.poleMravcov[i][0] = rand() % d.vyska; //Pozicia X
            d.poleMravcov[i][1] = rand() % d.sirka; //Pozicia Y
            d.poleMravcov[i][2] = rand() % 4; //Smer pohibu 0-3
            d.poleMravcov[i][3] = 1; // 1 zivy mravec 2 mrtvy mravec
        }
    } else {
        for (int i = 0; i < d.pocetMravcov; ++i) {
            int surX, surY;
            do {
                cout << "Zadaj suradnicu Y:  \n";
                if (!(cin >> surX)) {
                    cout << "Nespravny input!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (surX < 0 || surX > d.sirka);

            do {
                cout << "Zadaj suradnicu X:  \n";
                if (!(cin >> surY)) {
                    cout << "Nespravny input!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (surY < 0 || surY > d.sirka);


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
        //vykresli2(&d);
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
    printf("------------------------\n");
    for (int i = 0; i < d->vyska; ++i) {
        cout << '|';
        for (int j = 0; j < d->sirka; ++j) {
            char symbol = (d->pole[i][j] ? '#' : ' ');
            for (int k = 0; k < d->pocetMravcov; ++k) {
                if (d->poleMravcov[k][1] == j && d->poleMravcov[k][0] == i) {
                    symbol = 'X'; //-----------------------Znacka mravca
                    break;
                }
            }
            cout << symbol;
        }
        cout << '|';
        cout << '\n';
    }
    cout << '\n';
    printf("------------------------\n");
}

// ---------------Test graficke znazornenie
void vykresli2(DATA* d) {
    for (int i = 0; i < d->vyska; ++i) {
        printf("|");
        for (int j = 0; j < d->sirka; ++j) {

            for (int k = 0; k < d->pocetMravcov; ++k) {
                if (d->poleMravcov[k][1] == j && d->poleMravcov[k][0] == i) {
                    cout << "\U0001F41C"; //-----------------------Znacka mravca
                    break;
                }
            }
            if (d->pole[i][j]){
                cout << "⬛";
            } else {
                cout << "⬜";
            }

        }
        printf("|\n");
    }
    printf("\n");

}

void posunMravcov(DATA* d) {

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



