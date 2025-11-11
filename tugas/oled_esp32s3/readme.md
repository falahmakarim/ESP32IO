
---

## OLED README.md

```markdown
# OLED ESP32-S3 Experiment

##  Deskripsi = 
Program ini menguji OLED menggunakan ESP32-S3 di Wokwi Simulator.  
OLED dijalankan menggunakan FreeRTOS Task yang bisa diatur ke core 0 atau core 1.

---

##  Koneksi Peripheral

| Peripheral | Pin ESP32-S3 | Keterangan |
|------------|-------------|------------|
| VCC        | 3.3V          | Supply tegangan 3.3V ke modul |
| GND        | GND         | Ground Reference |
| SCL       | 20           | Clock I2C, digunakan ESP32 untuk komunikasi data ke OLED |
| SDA      | 21         | Data I2C, digunakan ESP32 untuk komunikasi data ke OLED |

---

## 🧪 Langkah Percobaan

1. Buka website wokwi.com, kemudian pilih ESP32. kemudian pada starter template, pilih ESP32-S3
2. Tambahkan komponen yang akan digunakan. bila diperlukan, install project libraries. klik tab library manager, kemudian Klik tombol “+” lalu cari library yang sesuai setelah itu pilih dan tambahkan. 
3. sambungkan komponen ke ESP32-S3 sesuai pin yang benar
4. Masukkan program tersebut ke bagian sketch.ino, kemudian klik tombol "start the simulation" dan amati hasilnya.
5. Ubah program itu menjadi core 1/core 0, lalu amati hasilnya


---
