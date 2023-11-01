#ifndef DISPLAY_MACROS_H
#define DISPLAY_MACROS_H 1

// Segments for a 7-segment display
#define DISP_SEG_B 0b01101111
#define DISP_SEG_C 0b01111011
#define DISP_SEG_E 0b01111110
#define DISP_SEG_F 0b00111111
#define DISP_SEG_G 0b01110111

// Binary representations for numbers on a 7-segment display
#define DISP_0 0b00001000 // 0
#define DISP_1 0b01101011 // 1
#define DISP_2 0b01000100 // 2
#define DISP_3 0b01000001 // 3
#define DISP_4 0b00100011 // 4
#define DISP_5 0b00010001 // 5
#define DISP_6 0b00110000 // 6
#define DISP_7 0b01001011 // 7
#define DISP_8 0b01111111 // 8
#define DISP_9 0b00000001 // 9


// Button display patterns
#define DISP_VERT_BAR_LEFT (DISP_SEG_E & DISP_SEG_F)
#define DISP_VERT_BAR_RIGHT (DISP_SEG_B & DISP_SEG_C)

// Control macros for display
#define DISP_OFF 0b01111111
#define DISP_ON 0b00000000

// Left-hand side bitmask
#define DISP_LHS (1 << 7)

#endif
