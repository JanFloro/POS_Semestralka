#include "server.h"

using namespace std;



void inicializaciaServerData(SERVERDATA *data, int newsockfd) {
    data->buffer = new char [512];
    data->newsockfd = newsockfd;
    data->n = 0;
    pthread_mutex_init(&data->mutexServer, NULL);
}

void odstranenieServerData(SERVERDATA *data) {
    pthread_mutex_destroy(&data->mutexServer);
    delete[] data->buffer;
    close(data->newsockfd);
    data->buffer = nullptr;
    data->n= 0;
}


void CitanieZClienta(char buffer[512], int newsockfd)
{
    bzero(buffer,512);
    int n = read(newsockfd, buffer, 511);
    if (n < 0)
    {
        perror("Error reading from socket");
    }
}

void PosielanieNaCLienta(char sprava[512], int newsockfd) {
    int n = write(newsockfd, sprava, strlen(sprava));
    if (n < 0) {
        perror("Error writing to socket");
    }
}


void ulozenieServer(bool** pole, int vyska, int sirka){
    ofstream outputFile("ServerMapa.txt");

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

bool** nacitanieServer(int &vyska, int &sirka) {
    ifstream inputFile("ServerMapa.txt");

    if (!inputFile.is_open()) {
        cerr << "Neotvoril sa subor na serveri: " << "ServerMapa.txt" << endl;
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
    cout << "Mapa bola uspesne nacitana zo servera: " << "ServerMapa.txt" << endl;

    return pole;
}

void clientServer(int newsockfd) {
    int vyska, sirka;
    bool** serverPole;

    cout << "Server vlakno" << endl;


    SERVERDATA serverdata;

    inicializaciaServerData(&serverdata, newsockfd);
    cout << "Server before reading from client" << endl;
    CitanieZClienta(serverdata.buffer, serverdata.newsockfd);
    cout << "Server after reading from client" << endl;
    cout << "Received message from client: " << serverdata.buffer << endl;
    cout << serverdata.buffer << endl;
    int odpoved = atoi(serverdata.buffer);

    cout << "Server  pred if" << endl;
    cout <<  odpoved << endl;

    if (odpoved == 1) {
        cout << "Odpoved 1 " << endl;
        serverPole = nacitanieServer(vyska, sirka);

        sprintf(serverdata.buffer,"%d",vyska);
        PosielanieNaCLienta(serverdata.buffer, serverdata.newsockfd);
        CitanieZClienta(serverdata.buffer, serverdata.newsockfd);

        sprintf(serverdata.buffer,"%d",sirka);
        PosielanieNaCLienta(serverdata.buffer, serverdata.newsockfd);
        CitanieZClienta(serverdata.buffer, serverdata.newsockfd);

        for (int i = 0; i < vyska; ++i) {
            for (int j = 0; j < sirka; ++j) {
                int hodnota;
                serverPole ? hodnota = 1 : hodnota = 0;
                /*
                if (serverPole) {
                    hodnota = 1;

                } else {
                    hodnota = 0;
                }*/

                sprintf(serverdata.buffer,"%d",hodnota);
                PosielanieNaCLienta(serverdata.buffer, serverdata.newsockfd);
                CitanieZClienta(serverdata.buffer, serverdata.newsockfd);
            }
        }


    }

    if (odpoved == 3) {
        CitanieZClienta(serverdata.buffer, serverdata.newsockfd);
        vyska = atoi(serverdata.buffer);
        PosielanieNaCLienta(serverdata.buffer, serverdata.newsockfd);

        CitanieZClienta(serverdata.buffer, serverdata.newsockfd);
        sirka = atoi(serverdata.buffer);
        PosielanieNaCLienta(serverdata.buffer, serverdata.newsockfd);

        serverPole = new bool*[vyska];
        for (int i = 0; i < vyska; ++i) {
            serverPole[i] = new bool[sirka];
        }


        for (int i = 0; i < vyska; ++i) {
            for (int j = 0; j < sirka; ++j) {
                CitanieZClienta(serverdata.buffer,serverdata.newsockfd);
                serverPole[i][j] = atoi(serverdata.buffer);
                PosielanieNaCLienta(serverdata.buffer, serverdata.newsockfd);
            }
        }
        
        ulozenieServer(serverPole, vyska, sirka);

        //PosielanieNaCLienta("Ulozenie na server: \n",serverdata.n,newsockfd);
    }

}


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



    //-------------------------------------------Main Kód---------------------------------
    while (true) {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            return 3;
        }

        cout << "SERVER pripojil klienta" << endl;
        thread pripojenie(clientServer, newsockfd);
        pripojenie.detach();

    }
}

