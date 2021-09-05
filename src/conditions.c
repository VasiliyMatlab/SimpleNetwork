#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "conditions.h"
#include "internal.h"

// Вывод на экран состояния клиента
void print_client_state(id_t id, pid_t pid, state_t state) {
    unsigned int vol = number_of_states(state);
    char buf[16];
    if (vol == 1) {
        switch (state) {
            case OFF:
                sprintf(buf, "OFF");
                break;
            case RED:
                sprintf(buf, "RED");
                break;
            case YELLOW:
                sprintf(buf, "YELLOW");
                break;
            case GREEN:
                sprintf(buf, "GREEN");
                break;
            case ALL:
                sprintf(buf, "ALL");
                break;
            default:
                fprintf(stderr, "State error: invalid client state\n");
                exit(EXIT_FAILURE);
        }
        printf("[%d] Client #%d is in state %s\n", pid, id, buf);
    }
    else if (vol > 1) {
        printf("[%d] Client #%d is in %d states: ", pid, id, vol);
        for (char i = 0; i < 3; i++) {
            if (state & (1 << i)) {
                switch (i) {
                    case 0:
                        printf("RED, ");
                        break;
                    case 1:
                        printf("YELLOW, ");
                        break;
                    case 2:
                        printf("GREEN, ");
                        break;
                    default:
                        fprintf(stderr, "State error: unknown state\n");
                        exit(EXIT_FAILURE);
                }
            }
        }
        printf("\b \b\b \b\n");
    }
    else {
        fprintf(stderr, "State error: invalid combination of states\n");
        exit(EXIT_FAILURE);
    }
}

// Вычисление количества состояний клиента
unsigned int number_of_states(state_t state) {
    if (state == OFF)
        return 1;
    if (state == ALL)
        return 1;
    unsigned int res = 0;
    for (char i = 0; i < 3; i++)
        if (state & (1 << i))
            res++;
    return res;
}

// Удаление клиента из логфайла
void remove_client_from_log(int fd, id_t id) {
    lseek(fd, 0, SEEK_SET);
    char buffer[BUFSIZ];
	Read(fd, buffer, BUFSIZ-1);
	char str[9];
	int ptr = 0;
    int row = -1;
	for (int i = 0; i < strlen(buffer); i++) {
		if (i % 9 == 0) {
			ptr = 0;
			row++;
			memset(str, 0, 9);
		}
		str[ptr++] = buffer[i];
		if (i % 9 == 8) {
			int num;
			sscanf(str, "Client %d\n", &num);
			if (id == num) {
				removerow(buffer, row);
				clearfile(fd);
				Write(fd, buffer, strlen(buffer));
				return;
			}
		}
	}
    fprintf(stderr, "Remove error: no such client in logfile\n");
    exit(EXIT_FAILURE);
}

// Удаление строки с клиентом из буфера
void removerow(char *buffer, int row) {
	for (int i = row * 9; i < strlen(buffer); i++) {
		buffer[i] = buffer[i+9];
	}
}

// Очистка файла
void clearfile(int fd) {
	lseek(fd, '\0', SEEK_SET);
	char cl[BUFSIZ];
	memset(cl, 0, BUFSIZ);
	Write(fd, cl, BUFSIZ-1);
	lseek(fd, 0, SEEK_SET);
}