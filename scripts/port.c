#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MESSAGE_HEIGHT 16
#define MESSAGE_LENGTH 120
#define MESSAGE_SIZE MESSAGE_HEIGHT * MESSAGE_LENGTH

#define HEADER_SIZE 4
#define FOOTER_SIZE 12
#define PACKET_SIZE (MESSAGE_SIZE / 8) + HEADER_SIZE + FOOTER_SIZE

uint8_t message[MESSAGE_SIZE];
uint8_t packet[PACKET_SIZE];

//Debug only, feel free to remove
void printmessage(uint8_t *msg, int size, uint8_t split, uint8_t hex) {
    for (int i = 0; i < size; i++) {
        if(split > 0) {
            if(i % split == 0) {
            printf( "\n");
            }
        }
        if(hex == 0) {
            printf( "%c", msg[i]);
        } else {
            printf( "%02x", msg[i]);
        }
    }
}

void rot90(uint8_t *msg) {
    uint8_t padbuffer[MESSAGE_SIZE];

    memcpy(padbuffer, msg, MESSAGE_SIZE);

    for (int y = 0; y < MESSAGE_HEIGHT; y++) {
        for (int x = 0; x < MESSAGE_LENGTH; x++) {
            msg[x * MESSAGE_HEIGHT + (MESSAGE_HEIGHT - y - 1)] = padbuffer[y * MESSAGE_LENGTH + x];
        }
    }

}

void roll(uint8_t *msg) {
    uint8_t padbuffer[MESSAGE_SIZE];

    memcpy(padbuffer, msg, MESSAGE_SIZE);

    for (int y = 0; y < MESSAGE_LENGTH; y++) {
        int row = y * MESSAGE_HEIGHT;
        for (int x = 0; x < 8; x++) {
            msg[row + x] = padbuffer[row + x + 8];
        }
        for (int x = 0; x < 8; x++) {
            msg[row + x + 8] = padbuffer[row + x];
        }
    }
}

void pack(uint8_t *msg, uint8_t *packedmsg, int size) {

    for (int i = 0; i < size / 8; i++) {
        for (int a = 0; a < 8; a++) {
            uint8_t byte = packedmsg[i] << 1;
            if (msg[i * 8 + a] == '0') { // Pixel is set
                byte = byte | 1;
            }
            packedmsg[i] = byte;

        }
    }

}

uint8_t checksum(int position, int page) {
    //Filthy hack to avoid passing in command code
    uint8_t check = 0xF0 - ((page - 1) << 4);
    for(int i = (position - 16); i < position; i++) {
        check -= packet[i];
    }
    return check;
}

void print_packet() {
    int page = 0;
    for(int i = 0; i < PACKET_SIZE; i++) {
        if(i % 16 == 0) {
            if(i > 0) {
                printf("%02X\n", checksum(i, page));
            }
            printf(":100%02X000", page);
            page++;
        }
        printf( "%02X", packet[i]);

    }
    printf("%02X\n", checksum(PACKET_SIZE, page));
}

void generate(uint8_t *msg) {
    rot90(message);
    roll(message);
    // printmessage(message, MESSAGE_SIZE, 16, 0);

    memset(packet, 0, PACKET_SIZE);

    //Header
    packet[0] = 0x01;
    packet[1] = 0x12;

    //Footer (padding)
    for (int i = (PACKET_SIZE - FOOTER_SIZE); i < PACKET_SIZE; i++) {
        packet[i] = 0xff;
    }

    pack(message, packet + HEADER_SIZE, MESSAGE_SIZE);
    //printmessage(packet, PACKET_SIZE, 16, 1);

    print_packet();

}

int main(int argc, char *argv[]) {
    if ( argc != 2 ) {
        printf( "Usage: %s bitmap array filename\n", argv[0] );
    } else {
        FILE *file = fopen( argv[1], "r" );

        if (file == 0) {
            printf( "Could not open file\n" );
        } else {
            int x;
            uint8_t *pMessage = message;
            while ( ( x = fgetc( file ) ) != EOF ) {
                *pMessage = x;
                pMessage++;
            }
            fclose( file );
            generate(message);
            printf( "\n" );
        }
    }
    return 0;
}

