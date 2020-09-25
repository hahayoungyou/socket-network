/*
 ���ϸ� : wecho_client.c
 ��  �� : echo ���񽺸� �䱸�ϴ� TCP(������) Ŭ���̾�Ʈ
 ������ : cc -o echo_client echo_client.c
 ���� : wecho_client [host] [port]
*/
#include <winsock.h>
#include <signal.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

WSADATA wsadata;
int	main_socket;

void exit_callback(int sig)
{
	closesocket(main_socket);
	WSACleanup();
	exit(0);
}

void init_winsock()
{
	WORD sversion;
	u_long iMode = 1;

	// winsock ����� ���� �ʼ�����
	signal(SIGINT, exit_callback);
	sversion = MAKEWORD(1, 1);
	WSAStartup(sversion, &wsadata);
}

#define ECHO_SERVER "127.0.0.1"
#define ECHO_PORT "30000"
#define BUF_LEN 128

int main(int argc, char* argv[]) {
	int s, n, len_in, len_out;
	char num[BUF_LEN + 1] = { 0 };
	char wel[BUF_LEN + 1] = { 0 };
	struct sockaddr_in server_addr;
	char* ip_addr = ECHO_SERVER, * port_no = ECHO_PORT;
	char buf[BUF_LEN + 1] = { 0 };
	char id[BUF_LEN + 1] = { 0 };
	char pass[BUF_LEN + 1] = { 0 };
	char error[BUF_LEN + 1] = { 0 };
	if (argc == 3) {
		ip_addr = argv[1];
		port_no = argv[2];
	}

	init_winsock();

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("can't create socket\n");
		exit(0);
	}
	main_socket = s;



	/* echo ������ �����ּ� ����ü �ۼ� */
	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip_addr);
	server_addr.sin_port = htons(atoi(port_no));

	/* �����û */
	printf("Connecting %s %s\n", ip_addr, port_no);

	if (connect(s, (struct sockaddr*) & server_addr, sizeof(server_addr)) < 0) {
		printf("can't connect.\n");
		exit(0);
	}
	if ((n = recv(s, wel, BUF_LEN, 0)) < 0) {
		printf("recv error\n");
		exit(0);
	}
	buf[n] = '\0'; // ���ڿ� ���� NULL �߰�
	printf("Received len=%d : %s", n, wel);

	while (1) {
		printf("ID �Է�:");
		if (fgets(id, BUF_LEN, stdin)) { // gets(buf);
			len_out = strlen(id);
			id[BUF_LEN] = '\0';
		}
		else {
			printf("fgets error\n");
			exit(0);
		}
		printf("PASSWORD �Է�");
		if (fgets(pass, BUF_LEN, stdin)) { // gets(buf);
			len_out = strlen(pass);
			pass[BUF_LEN] = '\0';
		}
		else {
			printf("fgets error\n");
			exit(0);
		}

		if ((send(s, id, BUF_LEN, 0) < 0)) {
			printf("send error\n");
			exit(0);
		}
		//printf("%s",buf);
		if (send(s, pass, BUF_LEN, 0) < 0) {
			printf("send error\n");
			exit(0);
		}

		if ((n = recv(s, (char*)&error, BUF_LEN, 0)) < 0) {
			printf("recv error\n");
			exit(0);
		}
		//buf[n] = '\0'; // ���ڿ� ���� NULL �߰�
		printf("Received len=%d : %s", n, error);
		if (strcmp(error, "welcome hansung\n") == 0) {
			break;
		}

	}
	while (1) {
		char* s2 = num;

		/* Ű���� �Է��� ���� */
		printf("***��ҹ��� ��ȯ �޴� �Դϴ�.\n");
		printf("(1)��� �빮�� ��ȯ\n");
		printf("(2)��� �ҹ��� ��ȯ\n");
		printf("(3)��->��, ��->�� ��ȯ\n");
		printf("(4)����\n");
		printf("�����ϼ���: ");

		if (fgets(num, BUF_LEN, stdin)) { // gets(buf);
			len_out = strlen(num);
			num[BUF_LEN] = '\0';
		}
		else {
			printf("fgets error\n");
			exit(0);
		}


		if (s2[0] == ('4')) {
			send(s, num, BUF_LEN, 0);
			break;

		}


		printf("Input string : ");
		if (fgets(buf, BUF_LEN, stdin)) { // gets(buf);
			len_out = strlen(buf);
			buf[BUF_LEN] = '\0';
		}
		else {
			printf("fgets error\n");
			exit(0);
		}





		/* echo ������ �޽��� �۽� */
		//printf("Sending len=%d : %s", len_out, buf);
		if (send(s, num, BUF_LEN, 0) < 0) {
			printf("send error\n");
			exit(0);
		}
		if ((send(s, buf, BUF_LEN, 0) < 0)) {
			printf("send error\n");
			exit(0);
		}
		//printf("%s",buf);
		
		if ((n = recv(s, buf, BUF_LEN, 0)) < 0) {
			printf("recv error\n");
			exit(0);
		}
		buf[n] = '\0'; // ���ڿ� ���� NULL �߰�
		printf("Received len=%d : %s", n, buf);
	}
	closesocket(s);
	return(0);
}

