#include "stm32f4xx.h"
#define RS 0x20     /* Pin mask for reg select (pin 5) */
#define RW 0x40     /* Pin mask for read/write (pin 6) */
#define EN 0x80     /* Pin mask for enable     (pin 7) */
void delayMs(int n); 
void LCD_command(unsigned char command); 
void LCD_data(unsigned char data);
void LCD_init(void);
void PORTS_init(void);
int main(){
LCD_init();
LCD_command(1);
volatile uint32_t input=GPIOB->IDR;
uint32_t i=0;
LCD_data(i+'0');
while(1){
while ((GPIOB->IDR)== input );
LCD_command(1);
if (i<10) { LCD_data(i+'0');
delayMs(150);
input=GPIOB->IDR;
i=i+1 ;}
else {
uint32_t j=i;
while(j>9){
j=j/10;
LCD_data('0'+(j));

}
LCD_data((i%10)+'0');
delayMs(150);
input=GPIOB->IDR;
i=i+1 ;
}
}
}
void LCD_init(void) {
    PORTS_init();
    delayMs(20);            /* initialization sequence */
    LCD_command(0x30);
    delayMs(5);
    LCD_command(0x30);
    delayMs(1);
    LCD_command(0x30);
		delayMs(1);
    LCD_command(0x30);
	  delayMs(1);
    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

void PORTS_init(void) {
		RCC->AHB1ENR |= 1<<1   ;	
		GPIOB->PUPDR |= 0X00000002;
    GPIOB->MODER |= 0x00005500;               /* Initialize needed GPIOs and set ports mode appropriately  */
		RCC->AHB1ENR |= 1<<2;
		GPIOC->MODER = 0x55555555;
}

void LCD_command(unsigned char command) {
    GPIOB->ODR &= ~(1<<6)  ;                         /* RS = 0, R/W = 0 */
	  GPIOB->ODR &= ~(1<<5)  ;
		GPIOC->BSRR = 0X00FF0000;
	  GPIOC->ODR  = command;                           /* put command on data bus */
    delayMs(0);
	  GPIOB->ODR |= (1<<7);                           /* pulse EN high */
    delayMs(0);
    GPIOB->ODR &= ~(1<<7);                             /* clear EN */

    if (command < 4)
        delayMs(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(unsigned char data) {
		//LCD_ready();
    GPIOB->ODR |= (1<<5)  ;                    /* RS = 1 */
    GPIOB->ODR &= ~(1<<6)  ;                    /* R/W = 0 */
		GPIOC->BSRR = 0X00FF0000;
		GPIOC->ODR = data  ;                 /* put data on data bus */
    GPIOB->ODR |= (1<<7);                   /* pulse EN high */
    delayMs(0);              
    GPIOB->ODR &= ~(1<<7);                   /* clear EN */
    delayMs(1);
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n) {
		int j;
    int i;
    for (; n>0; n--)
        for (i = 0; i < 3195; i++) {}
}

