#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>



int main(int argc, char **argv) {
    printf("TFTP Client\n");

    if (argc != 4) {
        printf("Usage: %s <adresse_du_serveur> <port> <nom_du_fichier>\n", argv[0]);
        exit(EXIT_FAILURE);
    }



    struct addrinfo hints;
    struct addrinfo * result;
    memset(&hints,0,sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
     hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    int status = getaddrinfo(argv[1],argv[2],&hints,&result);
      if (status != 0) {
        fprintf(stderr, "Erreur de résolution de l'adresse : %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

     int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sd == -1) {
        perror("Erreur lors de la création du socket");
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    char rrq[128] = {0};
    rrq[0] = 0;
    rrq[1] = 1;
    sprintf(rrq + 2, "%s", argv[3]);
    sprintf(rrq + 3 + strlen(argv[3]), "octet");


      ssize_t bytes_sent = sendto(sd, rrq, strlen(argv[3]) + 9, 0, result->ai_addr, result->ai_addrlen);
    if (bytes_sent == -1) {
        perror("Erreur lors de l'envoi de la requête");
        close(sd);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

     printf("Requête RRQ envoyée avec succès pour le fichier '%s'.\n", argv[3]);


    char buffer[516];
    struct sockaddr_storage server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    ssize_t data_size;
    
     FILE *file = fopen("output.txt", "wb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        freeaddrinfo(result);
        close(sd);
        exit(EXIT_FAILURE);
    }
    do {
        // Réception d'un paquet DAT
        data_size = recvfrom(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &server_addr_len);
        if (data_size == -1) {
            perror("Erreur lors de la réception du paquet");
            break;
        }

        // Vérification de l'opcode
        if (buffer[0] != 0 || buffer[1] != 3) { // Opcode 3 pour DAT
            fprintf(stderr, "Paquet reçu invalide (pas un paquet DAT).\n");
            break;
        }
        uint16_t block_num = (buffer[2] << 8) | buffer[3];
        printf("Réception du bloc %d (%zd octets).\n", block_num, data_size - 4);

        fwrite(buffer + 4, 1, data_size - 4, file);

        // Construction et envoi de l'ACK
        char ack[4] = {0};
        ack[0] = 0;
        ack[1] = 4; // Opcode pour ACK
        ack[2] = block_num >> 8;
        ack[3] = block_num & 0xFF;

        ssize_t ack_sent = sendto(sd, ack, sizeof(ack), 0, (struct sockaddr *)&server_addr, server_addr_len);
        if (ack_sent == -1) {
            perror("Erreur lors de l'envoi de l'ACK");
            break;
        }

        
        if (data_size < 516) {
            printf("Dernier paquet reçu. Transfert terminé.\n");
            break;
        }

    } while (data_size > 0);

    fclose(file);
    freeaddrinfo(result);
    close(sd);

    printf("Fichier sauvegardé sous 'output.txt'.\n");
    return EXIT_SUCCESS;
}