# İlginç Bilgiler - Arduino Projesi

Biyoloji ve doğa hakkında ilginç bilgileri 16x2 LCD ekranda gösteren otonom bir Arduino projesi.

![Arduino](https://img.shields.io/badge/Platform-Arduino-00979D?logo=arduino&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green)

## 📋 Proje Hakkında

Bu proje, 32 farklı biyoloji gerçeğini sırayla LCD ekranda görüntüler. Her bilgi gösterildikten sonra LED ve buzzer ile dikkat çekici bir sinyal verir. EEPROM kullanarak hangi bilgilerin gösterildiğini takip eder, böylece Arduino yeniden başlatılsa bile kaldığı yerden devam eder.

### ✨ Özellikler

- **32 İlginç Bilgi**: Biyoloji ve doğa hakkında Türkçe bilgiler
- **Yazma Animasyonu**: Bilgiler karakter karakter yazılır
- **Sesli/Işıklı Uyarı**: Her yeni bilgi için buzzer ve LED sinyali
- **Kalıcı Hafıza**: EEPROM ile gösterilen bilgiler takip edilir
- **Otomatik Döngü**: Tüm bilgiler gösterildikten sonra başa döner
- **Uyku Modu**: Bilgi bulunamazsa otomatik reset

## 🔧 Donanım Gereksinimleri

| Bileşen | Açıklama |
|---------|----------|
| Arduino | Uno, Nano veya uyumlu |
| LCD Ekran | 16x2 I2C (Adres: 0x27) |
| Buzzer | Aktif buzzer (Pin 8) |
| LED | Herhangi bir renk (Pin 7) |
| Direnç | LED için 220Ω |
| Breadboard | Bağlantılar için |
| Jumper Kablolar | Bağlantılar için |

## 📌 Pin Bağlantıları

```
Arduino       Bileşen
--------      --------
Pin 8    →    Buzzer (+)
Pin 7    →    LED (Anot, 220Ω ile)
GND      →    Buzzer (-), LED (Katot)
A4 (SDA) →    LCD SDA
A5 (SCL) →    LCD SCL
5V       →    LCD VCC
GND      →    LCD GND
```

## 📚 Kütüphane Gereksinimleri

Arduino IDE'de aşağıdaki kütüphaneleri yükleyin:

1. **Wire.h** - I2C iletişimi (Arduino ile gelir)
2. **LiquidCrystal_I2C** - LCD kontrolü
3. **EEPROM.h** - Kalıcı hafıza (Arduino ile gelir)

## 🚀 Kurulum

1. Arduino IDE'yi açın
2. Gerekli kütüphaneleri yükleyin
3. `project-b-v0.1.ino` dosyasını açın
4. Donanım bağlantılarını yapın
5. Arduino'yu bilgisayara bağlayın
6. Kodu yükleyin

## 📖 Örnek Bilgiler

Projede yer alan bazı ilginç bilgiler:

- 🦑 Mürekkep balığı renk değiştirir
- 🐻 Tardigrat 30 yıl uyuyabilir
- 🍄 Mantar plastikle beslenebilir
- 🧠 Beyin 20 watt ile çalışır
- 🦠 DNA'nın %8'i virüs kodudur
- 🐜 Karınca mantar çiftliği kurar

## ⚡ Teknik Detaylar

- **Bilgi Gösterim Süresi**: 10 saniye
- **LCD Adresi**: 0x27 (I2C)
- **EEPROM Kullanımı**: 35 byte
  - 0-31: Bilgi durumları
  - 33: Mevcut bilgi indeksi
  - 34: Başlatma bayrağı

## 🔄 Çalışma Mantığı

```
Başlat → Bileşen Kontrolü → Bilgi Seç → Göster → Bekle → Tekrar
                                ↓
                    Tüm bilgiler gösterildi?
                          ↓ Evet
                    EEPROM Sıfırla → Reset
```

## 📝 Lisans

Bu proje MIT lisansı altında lisanslanmıştır.

## 👤 Geliştirici

**Tuana** tarafından okul projesi olarak geliştirilmiştir.

---

*"Bunu biliyor muydunuz?" - İlginç Bilgiler Projesi*
