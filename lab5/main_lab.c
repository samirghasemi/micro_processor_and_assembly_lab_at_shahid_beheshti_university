#define RCC_AHB1ENR (*((volatile unsigned int*)0x40023830))
#define GPIOA_MODER (*((volatile unsigned int*)0x40020000))
#define GPIOA_BSRR  (*((volatile unsigned int*)0x40020018))

void delayMs(int n);

int main(void) {
    RCC_AHB1ENR |=  1;              /* enable GPIOA clock */
    
    GPIOA_MODER &= ~0x0000000C ;     /* clear pin mode */
    GPIOA_MODER |=  0x00000001 ;     /* set pin to output mode */
    
    while(1) {
        GPIOA_BSRR = 0x00000001;    /* turn on LED */
        delayMs(500);
        GPIOA_BSRR = 0x00010000;    /* turn off LED */
        delayMs(500);
    }
}

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 4000; i++) ;
}