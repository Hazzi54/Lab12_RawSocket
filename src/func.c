#include "func.h"

void handle_error(char *mess) {
    perror(mess);
    exit(EXIT_FAILURE);
}

struct header_mask_udp {
    short int src;
    short int dest;
    short int len;
    short int sum;
};

struct header_mask_ip {
    char verIHL;
    char DS;
    short int len;
    short int Id;
    short int flOffset;
    char TTL;
    char TransProto;
    short int sum;
    int src;
    int dest;
};

struct header_mask_eth {
    char dest[6];
    char src[6];
    short int type;
};

void fill_header_transp(char *buf, int size) {
    struct header_mask_udp *m = (struct header_mask_udp *)buf;
    m->src = htons(RAW_PORT);
    m->dest = htons(PORT);
    m->len = htons(size);
    m->sum = 0;
}

void fill_header_ip(char *buf, int size) {
    struct header_mask_ip *m = (struct header_mask_ip *)buf;
    m->verIHL = 0x45;
    m->DS = 0;
    m->len = htons(size);
    m->Id = 0;
    m->flOffset = 0;
    m->TTL = 255;
    m->TransProto = IPPROTO_UDP;
    m->sum = 0;
    m->src = inet_addr("192.168.0.120");
    m->dest = inet_addr(IP);

    m->sum = checkSum(buf);
}

void fill_header_eth(char *buf) {
    struct header_mask_eth *m = (struct header_mask_eth *)buf;
    m->dest[0] = 0x50;
    m->dest[1] = 0x5B;
    m->dest[2] = 0xC2;
    m->dest[3] = 0x55;
    m->dest[4] = 0x94;
    m->dest[5] = 0xD3;

    m->src[0] = 0xE0;
    m->src[1] = 0xB9;
    m->src[2] = 0xA5;
    m->src[3] = 0x02;
    m->src[4] = 0x19;
    m->src[5] = 0x40;

    m->type = htons(0x800);
}

short int checkSum(char *buf) {
    int sum = 0;
    unsigned short *ptr = (unsigned short *)buf, i = 0;
    while(i < 10) {
        sum += *ptr;
        ptr++;
        i++;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}
