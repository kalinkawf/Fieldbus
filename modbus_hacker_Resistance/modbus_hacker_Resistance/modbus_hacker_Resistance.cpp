// MODtoWIN.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <stdio.h>
#include <modbus.h>
#include <chrono>
#include <windows.h>

using namespace std;


void print_context(modbus_t* ctx)
{
    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        //return -1;
    }
    else
    {
        printf("Libmodbus context created\n");
    }
}

void print_connection(modbus_t* ctx)
{
    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        //return -1;
    }
    else
    {
        printf("Connection established\n");
    }
}

void print_slave(modbus_t* ctx, int slave)
{
    int rc = modbus_set_slave(ctx, slave);
    if (rc == -1)
    {
        fprintf(stderr, "Invalid slave ID\n");
        modbus_free(ctx);
        //return -1;
    }
    else
    {
        printf("Slave set successfully\n");
    }
}

void print_get_serial_mode(modbus_t* ctx)
{
    int serial_mode = modbus_rtu_get_serial_mode(ctx);
    if (serial_mode == MODBUS_RTU_RS485)
    {
        printf("RS-485");
    }
    else if (serial_mode == MODBUS_RTU_RS232)
    {
        printf("RS-232");
    }
    else if (serial_mode == -1)
    {
        fprintf(stderr, "Unable to resolve serial mode\n");
    }
    else
    {
        fprintf(stderr, "What the heck?!");
    }
}

void print_set_serial_mode(modbus_t* ctx)
{
    int mode = MODBUS_RTU_RS485;
    if (modbus_rtu_set_serial_mode(ctx, mode))
    {
        fprintf(stderr, "Unable to set serial mode\n");
    }
}

void print_serial_mode(modbus_t* ctx)
{
    print_get_serial_mode(ctx);
    print_set_serial_mode(ctx);
    print_get_serial_mode(ctx);
}

void print_read_screen(modbus_t* ctx)
{
    int adr = 0x0000;
    int nb = 1;
    uint16_t* data;
    //dest = malloc(nb * sizeof(* dest));
    data = new uint16_t[nb];
    int num_of_regs = modbus_read_registers(ctx, adr, nb, data);
    if (num_of_regs == -1)
    {
        fprintf(stderr, "Unable to read the given registers\n");
    }
    else
    {
        printf("Current value on the screen is: ");
        printf("%hu\n", *data);
    }
}

void print_read(modbus_t* ctx)
{
    int adr = 0x0000;
    int nb = 1;
    uint16_t* data;
    //dest = malloc(nb * sizeof(* dest));
    data = new uint16_t[nb];
    int num_of_regs = modbus_read_registers(ctx, adr, nb, data);
    if (num_of_regs == -1)
    {
        fprintf(stderr, "Unable to read the given registers\n");
    }
    else
    {
        printf("Current value on the screen is: ");
        for (int i = 0; i < nb; i++)
        {
            printf("%hu\n", data[i]);
        }
    }
}

void print_init(modbus_t* ctx, int slave)
{
    print_context(ctx);
    print_connection(ctx);
    print_slave(ctx, slave);
    print_read(ctx);
}

void write(modbus_t* ctx, uint16_t val)
{
    int adr = 0x0000;
    int nb = 1;
    uint16_t* value = new uint16_t;
    *value = val;
    const uint16_t* values = value;
    modbus_write_registers(ctx, adr, nb, values);
}

void print_write(modbus_t* ctx, uint16_t val)
{
    int adr = 0x0000;
    int nb = 1;
    uint16_t* value = new uint16_t;
    *value = val;
    const uint16_t* values = value;
    int wr_succ = modbus_write_registers(ctx, adr, nb, values);
    printf("wr_succ: %d\n", wr_succ);
    if (wr_succ == -1)
    {
        fprintf(stderr, "Writing operation failed: %s\n", modbus_strerror(errno));
    }
    else
    {
        printf("Writing operation succeeded\n");
    }
}

void display_values(modbus_t* ctx)
{
    int value;
    printf("\nEnter value to be displayed on the screen:\n");
    while (true)
    {
        cin >> value;
        write(ctx, value);
    }
}

void display_loop(modbus_t* ctx)
{
    printf("\nDisplaying subsequent numbers from 0 to 999 in an infinite loop\n");
    const int numb = 1;
    uint16_t data[numb];

    while (true)
    {
        for (int i = 0; i < 1000; i++)
        {
            printf("%d\n", i);
            write(ctx, i);
            Sleep(500);
        }
    }
}

int main()
{
    // init
    modbus_t* ctx = NULL;
    const char* device = "COM5"; // TODO: initialize
    int baud = 19200;
    char parity = 'N';
    int num_data_bits = 8;
    int num_stop_bits = 1;
    int slave = 1;

    ctx = modbus_new_rtu(device, baud, parity, num_data_bits, num_stop_bits);
    print_init(ctx, slave);

    //display_values(ctx); // main part
    display_loop(ctx);

    modbus_free(ctx);
}

