//                            !!!!!!!!!!!!! BU KOD USER BUTONA BASILDIĞINDA LED4'Ü YAKAR VEYA SÖNDÜRÜR !!!!!!!!!!!!!!

#include <stdint.h>

#define SCB_CPACR (*(volatile unsigned int*)0xE000ED88)
void FPU_Enable(void) {
    SCB_CPACR |= ((3UL << 20) | (3UL << 22));
}

//Butona basıldığında PD12 yanacağı ve buton da aktif olacağı için GPIOA ve GPIOD port ayarlamaları yapıldı.
void Configure_LED(void){
	volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t*)0x40023830;
	*RCC_AHB1ENR |= (1<<0) | (1<<3);
	volatile uint32_t *GPIOD_MODER = (volatile uint32_t*)0x40020C00;
	*GPIOD_MODER &= ~(3<<(12*2));
	*GPIOD_MODER |= (1<<(12*2));
}

void EXTI0_IRQHandler(void){
	volatile uint32_t *GPIOD_ODR = (volatile uint32_t*)0x40020C14;
	*GPIOD_ODR ^= (1<<12);

	volatile uint32_t *EXTI_PR = (volatile uint32_t*)0x40013C14;
	*EXTI_PR |= (1<<0);
}
int main(void)
{
    FPU_Enable();

    //SYSCFG : APB2 bus'ına bağlı bir periferal, kendi adresi var.
    //RCC : Bir periferal, kendi adresi var.
    //APB2ENR : RCC içinde bir register. Görevi APB2 bus'ına bağlı periferallerin clock'larını açıp kapatmak.

    // RCC_APB2ENR register'ı tanımlandı. SYSCFG, APB2 hattına bağlı bir periferal
    // olduğu için clock'u bu register üzerinden açılacak.
    volatile uint32_t *RCC_APB2ENR = (volatile uint32_t*)0x40023844;

    // SYSCFG'nin(periferal) clock'u, APB2ENR'in 14. bitine (SYSCFGEN) 1 yazılarak açıldı.
    *RCC_APB2ENR |= (1<<14);

    //PA0 portu EXTI0'a bağlı, bu yüzden SYSCFG_EXTICR1 register'ı tanımlandı.
    volatile uint32_t *SYSCFG_EXTICR1 = (volatile uint32_t*)0x40013808;

    //PA0 portuna bağlamak için register içine 0000 yazıldı.
    *SYSCFG_EXTICR1 &= ~((0xF)<<0);

    //EXTI0 hattına gelen interrupt'ların dinlenmesi için EXTI_IMR register'ı tanımlandı.
    volatile uint32_t *EXTI_IMR = (volatile uint32_t*)0x40013C00;

    *EXTI_IMR |= (1<<0);//EXTI_IMR register'ı set edildi.

    //STM32'de PA0 butonu pull_down olduğu için EXTI_RSTR register'ı aktif edilecek.
    //EXTI_RTSR register'ı tanımlandı.Bu register, PA0 0V'dan 3.3V'a değişikliği tetikleme sayar.
    volatile uint32_t *EXTI_RTSR = (volatile uint32_t*)0x40013C08;

    *EXTI_RTSR |= (1<<0);//Register aktif edildi.

    //Periferalden gelen interrupt direkt olarak işlemciye geçmeden NVIC'e uğraması gerekir,bu işlemci içerisindedir.
    //NVIC_ISER register'ı , hangi interrupt'lara izin verilip verilmeyceğini kontrol eden register.
    //PA0'dan gelen(yani EXTI0'dan gelen) interrupt'un numarası 6 olduğu için (vector table'dan) NVIC_ISER0 tanımlandı.
    volatile uint32_t *NVIC_ISER0 = (volatile uint32_t*)0xE000E100;

    *NVIC_ISER0 |= (1<<6);

    Configure_LED();
    while(1){

    }



}
