#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PORT 5030

#define N (100)

typedef struct {
	int type;
	int len;
	int mas[N];
} message;


int main(int argc, char *argv[]) {

	int listenDesc = 0;
	int currentDesc = 0;
	
    	struct sockaddr_in serv_addr;

	int n;
	int q;
	
	message msg;

	srand(time(NULL));

	listenDesc = socket(AF_INET, SOCK_STREAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(NUM_PORT);

	bind(listenDesc, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenDesc, 1);

	while(1) {
		
		currentDesc = accept(listenDesc, (struct sockaddr*)NULL, NULL);

		recv(currentDesc, &msg, sizeof(message), 0);
		
		n = msg.mas[2];
		q = msg.mas[1];
		msg.type = 2;
		
		if(msg.mas[0] == 0) {
			msg.type = -1;
			send(currentDesc, &msg, sizeof(message), 0);
			continue;
		}
			
		
		do {
			for (msg.len = 1; msg.len < n && msg.len < N; ++msg.len)
				msg.mas[msg.len] = msg.mas[msg.len - 1] * q;
				
			n -= N;
			if(n <= 0)
				msg.type = 0;
				
			send(currentDesc, &msg, sizeof(message), 0);
			
			msg.mas[0] = msg.mas[msg.len - 1];
			
		} while (msg.type == 2);
		

		close(currentDesc);
	}
	return 0;
}
