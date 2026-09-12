STM32_Embedded

STM32 ile yaptığım, gömülü yazılım tekniklerini öğrendiğim maker projelerimi paylaşıyorum.

#03Repo_Button_Interrupt

STM32CubeIDE'de user butona basıldığında (yani karta dışarıdan interrupt gönderildiğinde) LED4’ü yakan veya tekrar basılması durumunda söndüren bare-metal düzeyde register level kodlama yaptım. 

KAVRAMSAL HARİTA:

Interrupt nedir  :  İşlemci çalışırken, fiziksel veya yazılım kaynaklı olup işlemcinin normal akışının dışından gelen, işlemcinin çalışmasını anlık olarak aniden durduran uyarı sinyali. Eğer bir interrupt gelmişse işlemci o an ne işi varsa ara verir, interrupt doğrultusunda işlemi yerine getirir ve bir önceki ara verdiği işine devam eder. 

EXTI (External Interrupt) nedir  :  GPIO dahil birçok kaynaktaki sinyal değişimlerini yakalayan STM32 periferali. Kaynaklardaki 1’den 0’a veya 0’dan 1’e geçişleri yakalayıp bunları interrupt talebi haline getiren çip içi elektronik bir devre.

NVIC (Nested Vectored Interrupt Controller) nedir  :  Periferaller ile CPU arasında iletişim kuran çekirdek içindeki elektronik bir devre . Periferalden gelen taleplerin CPU’ya iletip iletilmeyeceğine veya hangi öncelikte iletileceğine karar verir. 

Vector Table nedir  :  Interrupt geldiğinde CPU’nun interrupt’u işlemesi için hangi adrese zıplaması gerektiğini tutan bellekte saklı veri yapısı. 

SYSCFG nedir  :  Periferaller arasında yönlendirme yapan multiplexer benzeri bir tür periferal. Örnek olarak EXTI0 hattının hangi GPIO portundan (PA0 mı, PB0 mı …) gelen sinyali dinleyeceğinin ayarlamasının yapıldığı yer. 

IRQ (Interrupt Request) nedir  :   Bir donanım biriminin (periferalin) CPU'ya gönderdiği kesme talebi. Her IRQ kaynağına, onu diğerlerinden ayırt etmek için sabit bir IRQ numarası atanmış (örn. EXTI0_IRQn = 6). Bu numara, NVIC_ISER register'ında hangi bitin o kaynağa karşılık geldiğini belirlemek için kullanılır. Numarayı register'a değer olarak yazmayız, o numaraya karşılık gelen biti set ederiz.

EXTI_IMR  :   EXTI’nin hangi hattının dinlenmesi gerektiğinin ayarlandığı register.

EXTI_RTSR pull-down bağlantılarda, EXTI_FTSR ise pull-up bağlantılarda aktif olan registerlar. Yani dinlenen hat hangi durumdaysa talep oluşturulmalı kararının verildiği yer.

EXTI_PR  :  İşlenmeyi bekleyen interrupt talebinin olup olmadığını tutar. Bu register sıfırlanmaması interrupt talebininn olduğunu söyler. Interrupt’u işledikten sonra CPU’nun bu register’ı temizlemesi gerekir. 

!!!Bir interrupt akışında;

1. Bir olay gerçekleşir (donanımsal sinyal değişimi ya da yazılımsal tetikleme)

2. [Sadece GPIO kaynaklı EXTI0-15 için] SYSCFG_EXTICR, hangi GPIO 
   portunun ilgili EXTI hattına bağlı olduğunu önceden belirlemiştir.

3. İlgili EXTI hattında (IMR ile "unmask" edilmiş, RTSR/FTSR ile 
   tetikleme yönü seçilmişse) bir kesme talebi oluşur, EXTI bunu 
   NVIC'e iletir.

4. NVIC, bu talebin önceliğini değerlendirir, CPU'nun şu an 
   çalıştırdığı işten daha yüksek öncelikliyse CPU'yu bilgilendirir.

5. CPU, o an çalıştırdığı komutu tamamladıktan sonra mevcut 
   context'ini (register durumları) stack'e kaydeder.

6. CPU, vector table'da ilgili IRQ numarasına karşılık gelen 
   adresi okur, Program Counter'ı bu adrese ayarlar.

7. İlgili interrupt handler fonksiyonu çalışır (senin yazdığın kod).

8. Handler, gerekiyorsa pending bit'i temizler (EXTI_PR gibi — 
   SysTick'te bu adım otomatik).

9. Handler biter, CPU stack'ten sakladığı context'i geri yükler, 
   kaldığı yerden normal akışa devam eder.

