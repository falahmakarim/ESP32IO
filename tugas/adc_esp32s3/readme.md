
---

## Potensio README.md

```markdown
# Potensio ESP32-S3 Experiment

##  Deskripsi = 
Program ini menguji ADC Potensio menggunakan ESP32-S3 di Wokwi Simulator.  
ADC Potensio dijalankan menggunakan FreeRTOS Task yang bisa diatur ke core 0 atau core 1.

---

##  Koneksi Peripheral

| Peripheral | Pin ESP32-S3 | Keterangan |
|------------|-------------|------------|
| VCC        | 3.3V           | Supply tegangan 3.3V  |
| GND        | GND         | Ground        |
| SIG        | 4          | Sinyal output analog ADC  |
---

## 🧪 Langkah Percobaan

1. Buka website wokwi.com, kemudian pilih ESP32. kemudian pada starter template, pilih ESP32-S3
2. Tambahkan komponen yang akan digunakan. bila diperlukan, install project libraries dengan klik tab library manager, kemudian Klik tombol “+” lalu cari library yang sesuai setelah itu pilih dan tambahkan. 
3. sambungkan komponen ke ESP32-S3 sesuai pin yang benar
4. Masukkan program tersebut ke bagian sketch.ino, kemudian klik tombol "start the simulation" dan amati hasilnya.
5. Ubah program itu menjadi core 1/core 0, lalu amati hasilnya


---
Hasil Percobaan 
Core 0 : https://youtu.be/4QfVJnRZUFU
Core 1 : https://youtu.be/vFdJW0Ynj-k