#include "func.h"

int main(int argc, char *argv[]) {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in sender, receiver;

    if(sock == -1)
        handle_error("socket");

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(PORT);
    receiver.sin_addr.s_addr = inet_addr(IP);

    socklen_t size_addr = sizeof(struct sockaddr_in);
    if(bind(sock, (struct sockaddr *) &receiver, size_addr) == -1)        
        handle_error("bind");

    ssize_t numBytes;
    char buf[MAXSIZE];
    int i;

    size_addr = sizeof(sender);
    numBytes = recvfrom(sock, buf, MAXSIZE, 0, (struct sockaddr *) &sender, &size_addr);
    if(numBytes == -1)
        handle_error("recvfrom");
    puts(buf);

    i = 0;
    while(i < numBytes) {
        buf[i] = toupper((unsigned char) buf[i]);
        i++;
    }
    puts(buf);
        
    if(sendto(sock, buf, numBytes, 0, (struct sockaddr *) &sender, size_addr) != numBytes)
        handle_error("sendto");

    shutdown(sock, SHUT_RDWR);
    return 0;
}
