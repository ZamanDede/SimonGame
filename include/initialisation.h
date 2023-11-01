#ifndef INITIALISATION_H
#define INITIALISATION_H 1

#include <avr/io.h>

/**
 * @brief Enable pull-up resistor for pushbuttons.
 */
static inline void button_init(void)
{
    // Enable pull-up resistors
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;

    // Configure timer for PB sampling
    cli();
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
    TCB0.CCMP = 3333;
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLA = TCB_ENABLE_bm;
    sei();
}

/**
 * @brief Initialize the PWM settings for the application.
 *
 * This function sets up the required registers and values to
 * establish a PWM signal.
 */
void pwm_init(void) {
    // Set PB0 as output for PWM generation
    PORTB.DIRSET = PIN0_bm;

    // Set up TCA0 in single slope PWM mode, enable the timer, and use system clock without division
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;

    // Define the period for the PWM signal
    uint16_t period = 10416;
    TCA0.SINGLE.PER = period;

    // Define the compare value for the PWM signal
    uint16_t compare_val = 1875;
    TCA0.SINGLE.CMP1 = compare_val;

    // Set the default compare value for CMP0
    TCA0.SINGLE.CMP0 = 0;
}


/**
 * @brief Initialize ADC0 to read the potentiometer value.
 */
static inline void adc_init(void)
{
    ADC0.CTRLA = ADC_ENABLE_bm;
    ADC0.CTRLB = ADC_PRESC_DIV2_gc;
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;
    ADC0.CTRLE = 64;
    ADC0.CTRLF = ADC_FREERUN_bm | ADC_LEFTADJ_bm;
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;
}


/**
 * @brief Configure SPI for 7-segment display.
 */
static inline void spi_init(void)
{
    // Configure pins
    // DISP LATCH (PA1)
    PORTA.OUTCLR = PIN1_bm;
    PORTA.DIRSET = PIN1_bm;
    // SPI CLK (PC0), SPI MOSI (PC2)
    PORTC.DIRSET = PIN0_bm | PIN2_bm;

    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;   // Select alternate pin configuration for SPI
    SPI0.CTRLB = SPI_SSD_bm;                    // Disable client select line
    SPI0.INTCTRL = SPI_IE_bm;                   // Enable interrupt
    SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm; // Enable SPI as master
}

/**
 * @brief Configure TCB1 for updating the 7-segment display and
 * pushbutton debouncing.
 */
static inline void timer_init(void)
{
    // Interrupt every 5ms
    TCB1.CCMP = 3333;
    TCB1.INTCTRL = TCB_CAPT_bm;
    TCB1.CTRLA = TCB_ENABLE_bm;

    // for delay
    TCB0.CCMP = 3333;
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
}


/**
 * @brief Initialize UART communication.
 */
void uart_init(void)
{
    // Output enable USART0 TXD (PB2)
    PORTB.DIRSET = PIN2_bm;

    USART0.BAUD = 1389;                           // 9600 baud @ 3.3 MHz
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Enable Tx/Rx

    USART0.CTRLA |= USART_RXCIE_bm;
}

#endif
