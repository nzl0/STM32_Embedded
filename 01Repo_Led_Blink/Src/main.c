

                              // !!!!!!!  BU KOD LED5'İ BLINK YAPAR.!!!!!!!!!!

#include <stdint.h>

#define SCB_CPACR (*(volatile unsigned int*)0xE000ED88)
void FPU_Enable(void) {
    SCB_CPACR |= ((3UL << 20) | (3UL << 22));}

void delay(volatile int loop){
	for (volatile int i = loop;i>=0;i--);}

int main(void)
{
    FPU_Enable();//Kartta FPU donanımı kapalı halde gelir. Yazılımla açmak gerekir.

    //0x40023800 adres bloğuyla başlayan RCC'nin, AHB1ENR ilgili offseti 0x30'dur.
    //0x40023830 adresindeki RCC_AHB1ENR register'ı tanımlanır.
    volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t*)0x40023830;

    *RCC_AHB1ENR |= (1<<3);//LED5 (PD14) için GPIOD portunun clock'u açıldı.

    //GPIOD'nin hangi modda olduğunu (input,output,analog...) belirleyen register tanımlandı.
    volatile uint32_t *GPIOD_MODER = (volatile uint32_t*)0x40020C00;

    *GPIOD_MODER &= ~(3<<(14*2));//Her ihtimale karşı iki bit temizlenir.
    *GPIOD_MODER |= (1<<(14*2));//MODER register'ı mod olarak output (01) yapıldı.

    //GPIOD'nin modunun 0 veya 1 olduğunu gösterecek ODR register'ı tanımlandı.
    volatile uint32_t *GPIOD_ODR = (volatile uint32_t*)0x40020C14;

    while(1){
    	*GPIOD_ODR |= (1<<14);//pini aç, led yanar.
    	delay(1000000);//1000000 kez döngüyü çalıştır.CPU BEKLER!
    	*GPIOD_ODR &= ~(1<<14);//pini kapat, led söner.
    	delay(1000000);//1000000 kez döngüyü çalıştır.CPU BEKLER!
    }





}
