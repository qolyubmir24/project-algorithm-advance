# project-algorithm-advance
IF-H Market - Sistem Manajemen TokoMore actions
===================================

IF-H Market adalah aplikasi manajemen toko berbasis C++ yang mendukung layanan toko offline dan online.
Aplikasi ini mencakup manajemen barang, transaksi, akun pelanggan, serta login untuk berbagai peran
karyawan (kasir, gudang, manajer, dan kurir).

---------------------------------------------------
Langkah Awal (WAJIB):
---------------------------------------------------

Sebelum menjalankan program, buat terlebih dahulu folder bernama:

    order

Gunakan perintah berikut di terminal / CMD:

    mkdir order

Folder 'order' digunakan untuk menyimpan file transaksi pembelian pelanggan.

---------------------------------------------------
Fitur Utama
---------------------------------------------------

1. Toko Online:
   - Pendaftaran akun pelanggan
   - Login pelanggan
   - Lihat semua barang
   - Cari barang
   - Beli barang dan simpan nota

2. Toko Offline:
   - Login sesuai role (Kasir, Gudang, Manajer, Kurir)
   - Kasir: Tambah transaksi pelanggan offline
   - Gudang: Kelola barang dan stok
   - Manajer: Lihat semua data barang & transaksi
   - Kurir: Lihat dan cari transaksi pelanggan online

---------------------------------------------------
File yang Digunakan
---------------------------------------------------

- goods.txt             : Daftar barang (format khusus)
- employee.txt          : Data karyawan
- customer_data.txt     : Data pelanggan
- order/                : Folder tempat penyimpanan struk transaksi

---------------------------------------------------
Format File
---------------------------------------------------

1. goods.txt (per barang):
   ID
   Nama Barang
   Stok
   Harga
   ---

2. employee.txt (5 baris per karyawan):
   ID
   Nama
   Password
   Role  (contoh: kasir, gudang, manajer, kurir)
   ---

---------------------------------------------------
Menjalankan Program
---------------------------------------------------

1. Pastikan file berikut ada di direktori:
   - goods.txt
   - employee.txt

2. Buat folder order:
   mkdir order

3. Kompilasi program:
   g++ -o ifh_market main.cpp

4. Jalankan program:
   ./ifh_market

---------------------------------------------------
Oktavian - Mirwan
---------------------------------------------------

Dikembangkan untuk simulasi sistem manajemen toko oleh kelompok 5  IF-H Market.
