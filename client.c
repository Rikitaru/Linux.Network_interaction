
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


#define NUM_PORT 5030

#define N (100)

typedef struct {
	int type;
	int len;
	int mas[N];
} message;

int main(int argc, char *argv[]) {

	int conDesc = 0;
	int i;
	
	message msg;

	struct sockaddr_in serv_addr;

	if(argc != 2) {
		printf("Usage: %s <ip of server>\n", argv[0]);
		return 1;
	}

	if((conDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error : Could not create socket\n");
		return 1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(NUM_PORT);


	
	printf("Введите b0: ");
	scanf("%d", &msg.mas[0]);
	printf("Введите q: ");
	scanf("%d", &msg.mas[1]);
	printf("Нужное число членов: ");
	scanf("%d", &msg.mas[2]);

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		printf("inet_pton error occured\n");
		return 1;
	}
	if(connect(conDesc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Error : Connect Failed\n");
		return 1;
	}
	
	send(conDesc, &msg, sizeof(message), 0);

	printf("Ответ сервера:\n");
	
	recv(conDesc, &msg, sizeof(message), 0);
	
	if(msg.type != -1) {
		for (i = 0; i < msg.len; ++i)
			printf("%d ", msg.mas[i]);
	}
	else
		printf("Ошибка, b0 равен нулю\n");
	
	while (msg.type == 2) {
		recv(conDesc, &msg, sizeof(message), 0);
		for (i = 0; i < msg.len; ++i)
			printf("%d ", msg.mas[i]);
	}
	printf("\n");

	return 0;
}
