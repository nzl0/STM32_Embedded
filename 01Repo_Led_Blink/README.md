STM32_Embedded

STM32 ile yaptığım, gömülü yazılım tekniklerini öğrendiğim maker projelerimi paylaşıyorum.

#01Repo_LedBlink

STM32CubeIDE'de LED5’i yakıp söndürmek için STM32F407-DISC1 üzerinden bare-metal kodlama yaptım. Bu kod LED5’i yakıp söndürüyor ve bunu sonsuz bir döngüde tekrar ediyor.

KAVRAMSAL HARİTA:

Clock neden var? 

Dijital devreler senkronize bir şekilde çalışır. Bu nedenle, dijital devre elemanlarının nasıl ve ne zaman birlikte çalışacağını düzenlemek için clock sinyallerine ihtiyaç duyulur. İlk olarak, (çip sıfırlanmış haldeyken) tüm çevre birimlerinin clock sinyalleri kapatılır. Bunun nedeni, zaten çalışmayan çevre birimlerine clock sinyali gönderilmemesidir. Bu nedenle, çalışmadan önce clock sinyallerinin manuel olarak açılması gerekir.

RCC nedir ve neden gereklidir?

RCC (Reset Clock and Control) bir çevre birimidir ve içinde birçok kayıt içerir. Görevi, hangi çevre biriminin clock sinyalinin açık olup olmadığını gösteren bir listeyi yönetmek ve tutmaktır.

AHB1ENR nedir?

AHB1ENR, AHB1 veri yoluna bağlı çevre birimlerini açan veya kapatan bir anahtar gibi düşünülebilir. Örneğin, AHB1 veri yolu üzerinde bir GPIOD portu (vb.) varsa, GPIOD portunun clock sinyalini açmak için AHB1ENR kaydı kullanılmalıdır.

MODER ve ODR nedir? 

MODER, GPIO portlarının hangi durumda olduğunu belirleyen bir kayıttır. Her pin 2 bit kaplar. Çünkü 4 durum vardır: 00_giriş, 01_çıkış, 10_alternatif fonksiyon, 11_analog ve 2^2=4. ODR kaydı, MODER'in çıkış modunun son durumunu temsil eder. 1 bit kaplar: 0 veya 1.

KARŞILAŞTIĞIM HATALAR

"FPU başlatılmadı, ancak proje bir FPU için derleniyor. Lütfen kullanmadan önce FPU'yu başlatın." uyarısı :  GPIO çevre birimleri gibi, FPU da çip sıfırlandığında varsayılan olarak kapalı bir şekilde gelir. Bu nedenle, çalışma zamanından önce FPU donanımda manuel olarak açılmalıdır.

Delay fonksiyonu : Delay fonksiyonunun içindeki loop ve i değerleri unsigned yapılırsa led blink olmaz. Çünkü for içinde >=0 şartı var. Unsigned bir sayı hiçbir zaman negatif bir sayı olamayacağından led uzunca bir süre yanar, hiç sönemez.

KNOW-HOW

GPIOD->MODER &= ~(3 << (14*2)); Bu kodda 3 yazılmıştır. Çünkü amaç 2 biti ayarlamaktır ve 3 ikili formatta 11 anlamına gelir.

Kodda kullanılan busy wait delay tam zamanı temsil etmez. For döngüsünün içindeki sayı, 1000000 gibi bir for döngüsü turudur. Bu turlar, clock hızına ve işlemcinin MHz'sine göre farklı zaman miktarlarını ifade eder. Ayrıca, kesinti gerçekleşirse CPU komutu işlemek için döngüyü kırar. Bu nedenle, daha fazla zamana ihtiyaç duyulabilir. Ayrıca, bu taşınabilir değildir. Aynı kod başka bir çipe aktarıldığında, süre değişir. Çünkü tekrar sayısı farklı donanımlarda farklı süre miktarlarını ifade eder. Yani, bu şekilde bir determinizm yoktur.

EK BİLGİ

Bare-metal kodlama ve register-level kodlama aynı mıdır? 

Bunlar aynı değildir. Bare-metal kodlama, işletim sistemi (RTOS, Linux vb.) olmadan doğrudan donanımda çalışan koddur. Zamanlayıcı, çekirdek veya görev yöneticisi yoktur. Register-level kodlamada, HAL (Donanım Soyutlama Katmanı) olmadan doğrudan register adresleri aracılığıyla çevre birimlerine ulaşılır. HAL'de, register'lara doğrudan erişmek yerine yerleşik fonksiyonlar çağrılır.

Neden register-level kodlama? Maksimum hız, netlik ve kontrol elde etmek için gereklidir.

Neden bare-metal kodlama? İşletim sistemi kullanmak yerine, zamanlama belirsizliği olmadan doğrudan deterministik bir şekilde çalışmak için gereklidir.


