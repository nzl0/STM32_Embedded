//                 !!!!!!!!!!!! BU KOD USER BUTON BASILI KALDIĞI SÜRECE LED4'Ü YAKAR!!!!!!!!!!!!!!

#include <stdint.h>

#define SCB_CPACR (*(volatile unsigned int*)0xE000ED88)
void FPU_Enable(void) {
    SCB_CPACR |= ((3UL << 20) | (3UL << 22));}

int main(void)
{
	FPU_Enable();
	 //0x40023800 adres bloğuyla başlayan RCC'nin, AHB1ENR ilgili offseti 0x30'dur.
	 //0x40023830 adresindeki RCC_AHB1ENR register'ı tanımlanır.
    volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t*)0x40023830;

    *RCC_AHB1ENR |= (1<<3);//GPIOD'nin clock'u açıldı.
    *RCC_AHB1ENR |= (1<<0);//GPIOA'nın clock'u açıldı. (User buton için)--> PA0 pini

    //GPIOD'nin ve GPIOA'nın modunu belirleyecek register MODER, tanımlandı.
    volatile uint32_t *GPIOD_MODER = (volatile uint32_t*)0x40020C00;
    volatile uint32_t *GPIOA_MODER = (volatile uint32_t*)0x40020000;

    *GPIOD_MODER &= ~(3<<(12*2)); //İlgili bitler temizlenir.
    *GPIOD_MODER |= (1<<(12*2)); //MODER output olması için 01 yapılır.

    *GPIOA_MODER &= ~(3<<0); //MODER input olması için 00 yapıldı.
    *GPIOA_MODER |= (0<<0);
    //GPIOD'nin ODR register'ı ve GPIOA'nın IDR register'ıtanımlanır.
    volatile uint32_t *GPIOD_ODR = (volatile uint32_t*)0x40020C14;
    volatile uint32_t *GPIOA_IDR = (volatile uint32_t*)0x40020010;


    while(1){
    	if(*GPIOA_IDR & (1<<0)){
    		*GPIOD_ODR |= (1<<12);
    	}
    	else{
    		*GPIOD_ODR &= ~(1<<12);
    	}
    }


}
