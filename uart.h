#ifndef UART_H
#define UART_H

typedef struct
{
    int8 sync_byte;
    int8 cmd;
    int8 length;
    int8 data1;
    int8 data2;

} Packet;

void uart_khbit(void);
void Packet_solve(void);

void Packet_response(void);

#endif
