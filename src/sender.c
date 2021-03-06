#include "func.h"

int main(int argc, char *argv[]) {
	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sock == -1)
		handle_error("socket");

	struct sockaddr_ll receiver;
	memset(&receiver, 0, sizeof(struct sockaddr_ll));
	receiver.sll_family = AF_PACKET;
	receiver.sll_ifindex = if_nametoindex("wlan0");
	receiver.sll_halen = ETH_ALEN;

	char buff[MAXSIZE], buf2[MAXSIZE], *ptr, *ptr_string, msg[MAXSIZE - 42];
	int i = 1;
	unsigned short *pShort;
	socklen_t sizeAddr;
 
	while(i < 10) {
		memset(buff, 0, MAXSIZE);
		memset(buf2, 0, MAXSIZE);
		memset(msg, 0, MAXSIZE - 42);
		ptr = buff;
		fill_header_eth(ptr);
		ptr += 14;
		fill_header_ip(ptr, MAXSIZE - 14);
		ptr += 20;
		fill_header_transp(ptr, MAXSIZE - 34);
		ptr += 8;
		printf("Enter a string:\n");
		fgets(ptr, MAXSIZE - 42, stdin);

		sizeAddr = sizeof(struct sockaddr_ll);
		if(sendto(sock, buff, sizeof(buff), 0, (struct sockaddr *)&receiver, sizeAddr) == -1)
			handle_error("sendto");
			
		while(1) {
			if(recvfrom(sock, buf2, MAXSIZE, 0, (struct sockaddr *)&receiver, &sizeAddr) == -1)
				handle_error("recvfrom");
			pShort = (unsigned short *)buf2;
			pShort += 18;
			if(ntohs(*pShort) == RAW_PORT)
				break;
		}

		ptr_string = buf2;
		ptr_string += 42;
		strcpy(msg, ptr_string);
		printf("%s", msg);
		i++;
	}
	shutdown(sock, SHUT_RDWR);
	return 0;
}
