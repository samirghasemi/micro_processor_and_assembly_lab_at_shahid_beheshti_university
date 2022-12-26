#include <stm32f4xx.h>
#include <stdio.h>
#define MASK(x) (1UL << (x))
#define RS 1UL  << 9    /* PA9 mask for reg select */
#define EN 1UL << 7     /* PC7 mask for enable */

void delayMs(int n);
void LCD_nibble_write(char data, unsigned long control);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_init(void);
void PORTS_init(void);

void TIM2Config(void);
void TIM3Config(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
char * convertIntegerToChar(uint32_t N);

uint32_t i = 0;
int clicked = 0;

int main(void) {
    /* initialize LCD controller */
    LCD_init();

    __disable_irq(); /* global disable IRQs */
    RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
    EXTI -> IMR |= 0x0001; /* unmask EXTI0 */
    EXTI -> RTSR |= 0x0001; /* select rising edge trigger */
    NVIC_EnableIRQ(EXTI0_IRQn); /* enable interrupt in NVIC */
    __enable_irq(); /* global enable IRQs */

    while(1) {
        // /* clear LCD display */
        // uint32_t j = a;
        // int count = 0;
        // LCD_command(1);
        // while (j > 0) {
        //     j = j / 10;
        //     count = count + 1;
        // }
        // char * arr = convertIntegerToChar(a);
        // for (int t = 0; t < count; t++) {
        //     LCD_data(arr[t]);
        // }
        // delayMs(1000);
        // a=a+3;
    }
}

/* initialize GPIOB/C then initialize LCD controller */
void LCD_init(void) {
    PORTS_init();

    
    delayMs(20);                /* LCD controller reset sequence */
    LCD_nibble_write(0x30, 0);
    delayMs(5);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    

    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delayMs(1);
    LCD_command(0x28);          /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);          /* move cursor right */
    LCD_command(0x01);           /* clear screen, move cursor to home */
    LCD_command(0x0F);          /* turn on display, cursor blinking */
}

void PORTS_init(void) {
    RCC->AHB1ENR |=  0x07;          /* enable GPIOA/B/C clock */
		
    GPIOA->MODER &= 0x00000C00;
    GPIOA->MODER |= 0x00050000;
    
    GPIOB->MODER &= 0x00002800;    /* clear pin mode */
    GPIOB->MODER |=  0x00100550;    /* set pin output mode */ // you have to change it to 0x0010500 on real work
    //GPIOB->BSRR = 0x00800000;       /* turn off EN */
		
    //GPIOC->ODR &= !EN;

    /* PC4-PC7 for LCD D4-D7, respectively. */
    GPIOC->MODER &=  0x0000FF00;    /* clear pin mode */
    GPIOC->MODER |=  0x00004000;    /* set pin output mode */
		GPIOC->BSRR   =  0x00800000;
	
}

void LCD_nibble_write(char data, unsigned long control) {
    /* populate data bits */
    //GPIOC->BSRR = 0x00F00000;       /* clear data bits */
    //GPIOC->BSRR = data & 0xF0;      /* set data bits */
    /*
		GPIOB->ODR = (  ((data & 0) << 5)
			      | ((data & 1) << 2) // change it to 4 on real work
			      | ((data & 2) << 10)
			     );
     */
	GPIOC->BSRR = 0xFFFF0000;
	GPIOA->BSRR = 0xFFFF0000;
	GPIOB->BSRR = 0xFFFF0000;
	
	if ( data & 0x10)
		GPIOB->BSRR = 0x00000020;
	if ( data & 0x20)
		GPIOB->BSRR = 0x00000010;
	if ( data & 0x40)
		GPIOB->BSRR = 0x00000400;
	
	if ( data & 0x80)
		GPIOA->BSRR = 0x00000100;
	
	 if (control & RS)
       GPIOA->BSRR = 0x00000200;
    else
       GPIOA->BSRR = 0x02000000;
		
		/* pulse E */
   GPIOC->BSRR = 0x00000080;
   delayMs(2);
   GPIOC->BSRR =  0x00800000;
   /*unsigned long tmp = GPIOB->ODR;
   tmp &= !((1UL << 5)
	       | (1UL << 2) // change it to 4 on real work
	       | (1UL << 10)
	       );
   if (data & 1UL << 4)
			tmp |= 1UL << 5;
   if (data & 1UL << 5)
			tmp |= 1UL << 2; // change it to 4 on real work
   if (data & 1UL << 6)
			tmp |= 1UL << 10;
	 */
	 
   //GPIOB->ODR = tmp;
      
   /*if (data & 1UL << 7)
			GPIOA->ODR |= 1UL << 8;
   else
			GPIOA->ODR &= !(1UL << 8);
      
    // set RS bit 
    if (control & RS)
        GPIOA->ODR |= RS;
    else
        GPIOA->ODR &= !RS; */

    /* pulse E */
    /*GPIOC->ODR &= !EN;
    delayMs(1);
    GPIOC->ODR |= EN;
    delayMs(1);
    GPIOC->ODR &= !EN;
    delayMs(1);*/
}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delayMs(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(char data) {
    LCD_nibble_write(data & 0xF0, RS);      /* upper nibble first */
    LCD_nibble_write(data << 4, RS);        /* then lower nibble */

    delayMs(1);
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
	//for (i=0; i < 319500*5; i++);
}

char * convertIntegerToChar(uint32_t N) {
  int m = N;
  int digit = 0;
  while (m) {
    digit++;
    m /= 10;
  }
  char * arr;
  char arr1[digit];
  arr = (char * ) malloc(digit);
  int index = 0;
  while (N) {
    arr1[++index] = N % 10 + '0';

    N /= 10;
  }
  int i;
  for (i = 0; i < index; i++) {
    arr[i] = arr1[index - i];
  }
  arr[i] = '\0';
  return (char * ) arr;
}

void TIM2Config(void) {
  __disable_irq();
  RCC -> APB1ENR |= (1 << 0); // Enable the timer2 clock
  TIM2 -> PSC = 1600 - 1; // 16MHz/1600 = 10 kHZ
  TIM2 -> ARR = 2 - 1; // MAX ARR value
  TIM2 -> CR1 |= (1 << 0); // Enable the Counter
  TIM2 -> CNT = 0;
  TIM2 -> DIER |= TIM_DIER_UIE;
  TIM2 -> CR1 |= (1 << 0); // Enable the Counter
  NVIC_EnableIRQ(TIM2_IRQn);
  __enable_irq();
}

void TIM3Config(void) {
  __disable_irq();
  RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;
  TIM3 -> PSC = 1600 - 1; // 16MHz/16000000 = 1 HZ
  TIM3 -> ARR = 10000 - 1; // MAX ARR value
  TIM3 -> CR1 |= (1 << 0); // Enable the Counter
  TIM3 -> CNT = 0;
  TIM3 -> DIER |= TIM_DIER_UIE;
  TIM3 -> CR1 |= (1 << 0); // Enable the Counter
  NVIC_EnableIRQ(TIM3_IRQn);
  __enable_irq();
}
void EXTI0_IRQHandler(void) {
  if (clicked == 0) {
    TIM2Config();
    TIM3Config();
    clicked = 1;
  } else if (clicked == 1) {
    TIM2 -> CR1 = 0; // Disabling the Counter
    TIM3 -> CR1 = 0; // Disabling the Counter
  }
}
void TIM2_IRQHandler(void) {
  i = i + 1;
}

void TIM3_IRQHandler(void) {
  uint32_t j = i;
  int count = 0;
  LCD_command(1);
  while (j > 0) {
    j = j / 10;
    count = count + 1;
  }
  char * arr = convertIntegerToChar(i);
  for (int t = 0; t < count; t++) {
    LCD_data(arr[t]);
  }
}