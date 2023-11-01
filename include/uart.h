#ifndef UART_H
#define UART_H

// Function to initialize UART communication
void uart_init(void);

// Function to retrieve a character from UART
char uart_getc(void);

// Function to send a character via UART
void uart_putc(char c);

// Function to send a string via UART
void uart_puts(char* ptr);

// Function to process received UART data
void process_uart_data(char data);

// Function to prompt and enter a name using UART
void enter_name(void);

#endif // UART_H
