#include "wifi_tcp.h"
#define HOST_IP_ADDR "192.168.2.188"
#define PORT         3355
int tcpsock = 0;
struct sockaddr_in dest_addr;
TaskHandle_t xtcpRecvTask = NULL;
void tcp_recv_data(void *pvParameters);
xTaskHandle xHandle;
void init_socket() {
	tcpsock = socket(AF_INET, SOCK_STREAM, 0);
	printf("sock:%d\n", tcpsock);
	if (tcpsock < 0) {
		printf("Unable to create socket: errno %d", errno);
		return;
	}
	printf("Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);
	dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
}

void connect_socket(int reConflag) {
	init_socket();
	int err = connect(tcpsock, (struct sockaddr * ) &dest_addr,
			sizeof(dest_addr));
	if (err != 0) {
		printf("Socket unable to connect: errno %d", errno);
		close(tcpsock);
		tcpsock = -1;
		vTaskDelay(3000 / portTICK_PERIOD_MS);
		connect_socket(reConflag);
	} else {
		printf("Successfully connected\n");
		if (!reConflag) {
			xTaskCreate(tcp_recv_data, "tcp_recv_data",
					OPENSSL_DEMO_THREAD_STACK_WORDS, NULL,
					OPENSSL_DEMO_THREAD_PRORIOTY, &xHandle);
		}

	}

}


void tcp_reconnect(int reConflag) {
	printf("zhaoming-------tcp_reconnect-----");
	connect_socket(reConflag);

}

void tcp_recv_data(void *pvParameters) {
	uint8_t rx_buffer[1024] = { 0 };
	while (1) {
		int len = recv(tcpsock, rx_buffer, sizeof(rx_buffer) - 1, 0);
		if (len > 0) {
			rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
			printf("Received %d bytes from %s:\n", len, rx_buffer);
//			int err = send(tcpsock, rx_buffer, len, 0);
//			if (err < 0) {
//				printf("Error occurred during sending: errno %d", errno);
//				return;
//			}
//			printf("Message sent");
		} else {
			close(tcpsock);
			tcpsock = -1;
			tcp_reconnect(1);

		}
	}

}

void tcp_ini_client(void) {
	connect_socket(0);
}