#include "stm32f4xx.h"
#include "math.h"
#define RS 0x20     /* Pin mask for reg select (pin 5) */
#define RW 0x40     /* Pin mask for read/write (pin 6) */
#define EN 0x80     /* Pin mask for enable     (pin 7) */
void delay_ms(int n); 
void LCD_command(unsigned char command); 
void LCD_data(unsigned char data);
char* convertIntegerToChar(uint32_t N);
void TIM3Config (void);
int i=0;

void Button_INT(void){
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 
  SYSCFG->EXTICR[0] |=SYSCFG_EXTICR1_EXTI0_PB;    
  EXTI->IMR |= 0x0001;                /* unmask EXTI0 */
	EXTI->RTSR |= 0x0001;               /* select rising edge trigger */
 NVIC_EnableIRQ(EXTI0_IRQn);     /* enable interrupt in NVIC */
 __enable_irq();                     /* global enable IRQs */
}
int main(){
  //ports init
  RCC -> AHB1ENR |= 1 << 1;
  GPIOB -> PUPDR |= 0X00000002;
  GPIOB -> MODER |= 0x00005500; /* Initialize needed GPIOs and set ports mode appropriately  */
  RCC -> AHB1ENR |= 1 << 2;
  GPIOC -> MODER = 0x00555555;
  
  //lcd init
  delay_ms(20); /* initialization sequence */
  LCD_command(0x30);
  delay_ms(5);
  LCD_command(0x30);
  delay_ms(1);
  LCD_command(0x30);
  delay_ms(1);
  LCD_command(0x30);
  delay_ms(1);
  LCD_command(0x38); /* set 8-bit data, 2-line, 5x7 font */
  LCD_command(0x06); /* move cursor right after each char */
  LCD_command(0x01); /* clear screen, move cursor to home */
  LCD_command(0x0F); /* turn on display, cursor blinking */

  //button init
  RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
  EXTI -> IMR |= 0x0001; /* unmask EXTI0 */
  EXTI -> RTSR |= 0x0001; /* select rising edge trigger */
  
  NVIC_EnableIRQ(EXTI0_IRQn); /* enable interrupt in NVIC */
  __enable_irq(); /* global enable IRQs */
	
  TIM3Config();
	LCD_command(1);
  while(1){}
}

void EXTI0_IRQHandler(void) {
i=i+1;
}

void TIM3_IRQHandler(void){
	uint32_t j=i;
	int count=0;
	LCD_command(1);
	while (j>0){
	j=j/10;
	count=count+1;
}

char* arr = convertIntegerToChar(i);
	for (int t = 0; t < count; t++){
		LCD_data(arr[t]);
	}
}

void TIM3Config (void) {
__disable_irq();
	RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 1600-1;  // 16MHz/16000000 = 1 HZ
	TIM3->ARR = 10000-1;  // MAX ARR value
	TIM3->CR1 |= (1<<0); // Enable the Counter
	TIM3->CNT=0;
	TIM3->DIER |=TIM_DIER_UIE;
  TIM3->CR1 |= (1<<0); // Enable the Counter
	NVIC_EnableIRQ(TIM3_IRQn);
	__enable_irq();
}

void LCD_command(unsigned char command) {
    GPIOB->ODR &= ~(1<<6)  ;                         /* RS = 0, R/W = 0 */
	  GPIOB->ODR &= ~(1<<5)  ;
		GPIOC->BSRR = 0X00FF0000;
	  GPIOC->ODR  = command;                           /* put command on data bus */
    delay_ms(0);
	  GPIOB->ODR |= (1<<7);                           /* pulse EN high */
    delay_ms(0);
    GPIOB->ODR &= ~(1<<7);                             /* clear EN */

    if (command < 4)
        delay_ms(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delay_ms(1);         /* all others 40 us */
}

void LCD_data(unsigned char data) {
		//LCD_ready();
    GPIOB->ODR |= (1<<5)  ;                    /* RS = 1 */
    GPIOB->ODR &= ~(1<<6)  ;                    /* R/W = 0 */
		GPIOC->BSRR = 0X00FF0000;
		GPIOC->ODR = data  ;                 /* put data on data bus */
    GPIOB->ODR |= (1<<7);                   /* pulse EN high */
    delay_ms(0);              
    GPIOB->ODR &= ~(1<<7);                   /* clear EN */
    delay_ms(1);
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

void delay_ms(int n) {
		int j;
    int i;
    for (; n>0; n--)
        for (i = 0; i < 3195; i++) {}
}