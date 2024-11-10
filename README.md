# Proje3
# Bu GitHub'daki üçüncü projem.
# #
Bu proje, ESP32-CAM modülünü kullanarak, bir doküman okuma ve saklama sisteminin temel işlevlerini gerçekleştirir. Proje, QR kodlarını okuyan bir okuma modülü ve Bluetooth üzerinden veri iletimi sağlamak için Bluetooth Serial kullanarak bir doküman saklama sistemine dönüştürülmüştür.

 ESP32-CAM modülünün QR kodları okuma yeteneği ve Bluetooth ile veri iletimi özelliklerini kullanarak, her okunan dokümanı sayar, saklar ve kontrol eder. Bu yazılım, bir motorun çalıştırılmasıyla birlikte saklanan dokümanın işlemiyle ilgili bir süreölçer ve zamanlayıcı yönetimi sağlar.

Özellikler
QR Kod Okuma: QR kodlarını okumak için  QR okuma modülü kullanılır.
Bluetooth İletişimi: Bluetooth Serial kullanılarak cihazdan bilgisayar veya telefon gibi başka cihazlara veri iletimi yapılır.
Motor Kontrolü: Saklanan her dokümanla birlikte motor çalıştırılarak bir işlem gerçekleştirilir.
EEPROM Veri Saklama: Okunan doküman sayıları EEPROM içinde saklanır ve cihaz yeniden başlatıldığında bile bu veriler korunur.
LED ve Buzzer Uyarıları: Sistem durumuna göre LED ışıkları ve/veya buzzer kullanılarak görsel ve sesli uyarılar yapılır.
Sistem Zamanlayıcısı: QR kodları okunduktan sonra motor işlemleri belirli bir süre için çalıştırılır ve sonrasında motor durdurulur.
Kullanılan Donanımlar
ESP32-CAM: Dokümanları okuma ve veri işleme görevlerini yerine getiren ana mikrodenetleyici.
Bluetooth Modülü: Bluetooth üzerinden veri iletimi yapmak için BluetoothSerial kütüphanesi kullanılır.
QR Okuma Modülü: QR kodlarını okur ve ESP32'ye iletir.
Motor ve Röle: Doküman işlemi sırasında motorun çalıştırılması için kontrol edilir.
LED ve Buzzer: Sistem durumlarına göre uyarı vermek için kullanılır.
Bağlantılar
QR Modülü (RX/TX): ESP32-CAM modülünün GPIO 1 (TX) ve GPIO 3 (RX) pinlerine bağlanır.
Bluetooth Seri Bağlantı: Bluetooth modülü ile veri alışverişi yapabilmek için ESP32'nin dahili Bluetooth Serial kullanılır.
Motor Kontrol: Motor, 12 ve 14 numaralı GPIO pinlerine bağlı motor aracılığıyla kontrol edilir.
LED ve Buzzer: LED ışığı ve buzzer, sırasıyla GPIO 33 ve GPIO 4 pinlerine bağlanır.
Yazılımın Yapısı
1. QR Kod Okuma Fonksiyonu (QRReadFunction)
QR kodları okuma işlemi için ESP32'nin UART arayüzü kullanılır. QR okuma modülünden gelen veriler, belirli bir formatta alınır ve işlenir. Okunan veriler, Bluetooth üzerinden iletilmek üzere bir kuyruğa eklenir.

2. Bluetooth Veri İletimi (BluetoothFunction)
Bluetooth Serial kullanılarak okunan veriler, cihazdan başka bir Bluetooth destekli cihaza (örneğin telefon veya bilgisayar) gönderilir. Sistem, Bluetooth üzerinden alınan komutlara göre motoru çalıştırır, sayacı sıfırlar veya belirli işlemleri gerçekleştirir.

3. Motor Kontrol Fonksiyonu (MotorRun)
Motor, gelen QR kodu okuma talimatlarına göre çalıştırılır. Motorun durumu (FORWARD, REVERSE, STOP) GPIO pinleriyle kontrol edilir. Motor çalışma süreleri, MotorTimeoutT1 ile izlenir ve belirli bir süre sonra motor otomatik olarak durdurulur.

4. EEPROM Veri Saklama
Okunan her doküman, bir sayaç aracılığıyla sayılır ve EEPROM belleğe kaydedilir. EEPROM'da saklanan bu veri, cihaz yeniden başlatıldığında bile korunur. Bu özellik, sistemin her çalıştırıldığında kaldığı yerden devam etmesini sağlar.

5. Zamanlayıcılar ve Durum Yönetimi
Sistem, her QR kod okuma işlemi arasında motor çalıştırma işlemlerini yönetmek için zamanlayıcılar kullanır. Her QR okuma ve motor çalışma durumu, belirli bir süre sonunda otomatik olarak sıfırlanır veya bir sonraki adıma geçilir.

Gereksinimler
ESP32-CAM modülü
QR Kod Okuma Modülü (RS232/UART tabanlı bir modül)
Bluetooth Bağlantısı olan bir cihaz (telefon veya bilgisayar)
Arduino IDE ile programlama (ESP32 desteği eklenmiş olmalı)
Kurulum Adımları
Arduino IDE Kurulumu:

ESP32 modülünü Arduino IDE'ye eklemek için ESP32 Arduino kurulum kılavuzunu takip edebilirsiniz.
Gerekli kütüphaneleri yükleyin: BluetoothSerial, EEPROM, ve diğer bağımlı kütüphaneler.
Donanım Bağlantıları:

QR Kod Modülü: ESP32'nin RX ve TX pinlerine bağlanmalıdır.
Motor Kontrolü: Motorlar, 12 ve 14 numaralı GPIO pinlerine bağlı röleler aracılığıyla çalıştırılır.
Bluetooth: ESP32'nin dahili Bluetooth modülü kullanılır.
Yazılımın Yüklenmesi:

Arduino IDE üzerinden bu projeyi açın ve ESP32'yi seçin.
Kodu derleyin ve ESP32 modülüne yükleyin.
Çalıştırma:

ESP32 cihazınızı çalıştırın.
Bluetooth cihazınızı bağlayın ve bağlantıyı kontrol edin.
QR kodlarını okutun, motoru çalıştırın ve işlemlerin Bluetooth üzerinden iletildiğini kontrol edin.
