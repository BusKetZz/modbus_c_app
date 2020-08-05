/*
 * Author: Jakub Standarski
 * Date: 31.07.2020
 *
 */



/*****************************************************************************/
/* HEADERS */
/*****************************************************************************/

#include <errno.h>
#include <modbus.h>
#include <stdio.h>



/*****************************************************************************/
/* MAIN */
/*****************************************************************************/

int main(void)
{
    const char ip_address[] = "127.0.0.1";

    int tcp_port = 0;
    printf("Input TCP port number: ");
    scanf("%d", &tcp_port);

    modbus_t *modbus_context = NULL;
    modbus_context = modbus_new_tcp(ip_address, tcp_port);
    if (modbus_context == NULL) {
        fprintf(stderr, "Initializing modbus context failed: %s\n",
            modbus_strerror(errno));
        return -1;
    }

    int error_code = -1;
    error_code = modbus_connect(modbus_context);
    if (error_code == -1) {
        fprintf(stderr, "Connection establishment failed: %s\n",
            modbus_strerror(errno));
        modbus_free(modbus_context);
        return -1;
    }
    printf("Connection established successfully:\t");
    printf("IP address: %s\t TCP port: %d\n", ip_address, tcp_port);


    int registers_start_address = 0;
    while (1) {
        printf("Input registers start address [0 - %d]: ",
            MODBUS_MAX_READ_REGISTERS - 1);
        scanf("%d", &registers_start_address);
        if (registers_start_address >= 0 && registers_start_address <
            MODBUS_MAX_READ_REGISTERS) {
            break;
        } else {
            fprintf(stderr, "Invalid address...\n");
        }
    }

    int registers_count = 0;
    while (1) {
        printf("Input number of registers to read [1 - %d]: ",
            MODBUS_MAX_READ_REGISTERS - registers_start_address);
        scanf("%d", &registers_count);
        if ( registers_count > 0 && (registers_count <=
            (MODBUS_MAX_READ_REGISTERS - registers_start_address))) {
            break;
        } else {
            fprintf(stderr, "Invalid number...\n");
        }
    }

    uint16_t registers_table[MODBUS_MAX_READ_REGISTERS] = {0};
    int registers_read_count = -1;
    registers_read_count = modbus_read_registers(modbus_context,
        registers_start_address, registers_count, registers_table);
    if (registers_read_count == -1) {
        fprintf(stderr, "Reading registers failed: %s\n",
            modbus_strerror(errno));
        modbus_close(modbus_context);
        modbus_free(modbus_context);
        return -1;
    }

    for (int register_index = 0; register_index < registers_count;
        register_index++) {
        printf("Register[%d] value: %u\n", register_index,
            registers_table[register_index]);
    }
    
    modbus_close(modbus_context);
    modbus_free(modbus_context);
    return 0;
}

