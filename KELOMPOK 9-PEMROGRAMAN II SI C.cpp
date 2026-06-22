#include <stdio.h>
#include <string.h>

// STRUCT & STRING
struct Emas {
    char id[10];
    char nama[30];
    int berat;
    int harga;
    int stok;
};

// ARRAY GLOBAL
Emas toko[200];
int jumlah = 0;

// --- FUNGSI FILE HANDLING ---
// Diletakkan paling atas agar bisa dipanggil oleh fungsi lain di bawahnya
// Fungsi untuk menyimpan data yang telah di input ke file .txt
void simpanData() {
    FILE *file = fopen("data_toko.txt", "w");
    for (int i = 0; i < jumlah; i++) {
        fprintf(file, "%s,%s,%d,%d,%d\n", toko[i].id, toko[i].nama, toko[i].berat, toko[i].harga, toko[i].stok);
    }
    fclose(file);
}

// Fungsi untuk membaca data yang ada di file .txt
void bacaData() {
    FILE *file = fopen("data_toko.txt", "r");
    if (file != NULL) {
        jumlah = 0;
        while (fscanf(file, "%[^,],%[^,],%d,%d,%d\n", toko[jumlah].id, toko[jumlah].nama, &toko[jumlah].berat, &toko[jumlah].harga, &toko[jumlah].stok) != EOF) {
            jumlah++;
        }
        fclose(file);
    }
}

// --- FUNGSI FITUR TOKO ---
// Fungsi untuk mengurutkan data berdasarkan harga termurah (ascending)
// Algoritma yang digunakan: Bubble Sort (karena sederhana dan mudah dipahami)
void urutHarga() {
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (toko[j].harga > toko[j+1].harga) {
                Emas temp = toko[j];
                toko[j] = toko[j+1];
                toko[j+1] = temp;
            }
        }
    }
    printf("\nData telah diurutkan berdasarkan harga termurah!\n");
}

// Fungsi untuk mengurutkan data berdasarkan ID (ascending)
// Algoritma yang digunakan: Bubble Sort (karena sederhana dan mudah dipahami)
void urutID() {
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (strcmp(toko[j].id, toko[j+1].id) > 0) {
                Emas temp = toko[j];
                toko[j] = toko[j+1];
                toko[j+1] = temp;
            }
        }
    }
    printf("\nData telah diurutkan berdasarkan ID!\n");
}

// Fungsi untuk mencari data berdasarkan ID
// Algoritma yang digunakan: Linear Search (karena data belum tentu terurut saat pencarian)
void cari() {
    char cariID[10];
    printf("\nMasukkan ID yang dicari: "); 
    scanf(" %[^\n]", cariID);
    
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(toko[i].id, cariID) == 0) { 
            printf("Ditemukan: %s (Rp%d) - Stok: %d\n", toko[i].nama, toko[i].harga, toko[i].stok);
            return; 
        }
    }
    printf("Data tidak ditemukan!\n");
}

// Fungsi untuk menambah data baru ke toko
// Proses pengecekan duplikat:
// 1. Cek jika ID sama (karena ID harus unik)
// 2. Cek jika Nama DAN Berat sama (karena bisa jadi ada perhiasan dengan nama yang sama tapi berat berbeda, atau sebaliknya)
// Algoritma yang digunakan: Linear Search untuk pengecekan duplikat (karena data belum tentu terurut saat penambahan)
void tambah() {
    printf("\nID Emas : "); scanf(" %[^\n]", toko[jumlah].id);
    printf("Nama    : "); scanf(" %[^\n]", toko[jumlah].nama);
    printf("Berat   : "); scanf("%d", &toko[jumlah].berat);
    printf("Harga   : "); scanf("%d", &toko[jumlah].harga);
    printf("Stok    : "); scanf("%d", &toko[jumlah].stok);

    // --- PROSES PENGECEKAN DUPLIKAT ---
    for (int i = 0; i < jumlah; i++) {
        // 1. Cek jika ID sama
        if (strcmp(toko[i].id, toko[jumlah].id) == 0) {
            printf("\n=> INPUT GAGAL: ID '%s' sudah digunakan!\n", toko[jumlah].id);
            return;
        }
        
        // 2. Cek jika Nama DAN Berat sama
        if (strcmp(toko[i].nama, toko[jumlah].nama) == 0 && toko[i].berat == toko[jumlah].berat ) {
            printf("\n=> INPUT GAGAL: %s dengan berat %d sudah ada di sistem!\n", toko[jumlah].nama, toko[jumlah].berat);
            return;
        }
    }

    jumlah++;
    simpanData(); 
    printf("\nData berhasil ditambahkan!\n");
}

// Fungsi untuk menghapus data berdasarkan ID
// Algoritma yang digunakan: Linear Search untuk mencari ID, lalu proses memundurkan data
// Proses memundurkan data (shifting):
// 1. Setelah data yang mau dihapus ditemukan, kita timpa data tersebut dengan data di depannya (data berikutnya)
// 2. Proses ini diulang terus sampai data paling akhir, sehingga data yang mau dihapus akan tertimpa dan hilang dari daftar
// 3. Setelah proses memundurkan selesai, kita kurangi total data (jumlah) karena sudah dihapus 1
// 4. Jangan lupa untuk simpan data ke file lagi agar perubahan juga tercermin di notepad
void hapus() {
    if (jumlah == 0) { 
        printf("Data kosong!\n"); 
        return; 
    }

    char hapusID[10];
    printf("\nMasukkan ID Emas yang mau dihapus: "); 
    scanf(" %[^\n]", hapusID);
    
    for (int i = 0; i < jumlah; i++) {
        // Cari ID-nya dulu
        if (strcmp(toko[i].id, hapusID) == 0) { 
            printf("Data ditemukan: %s (%s). Menghapus data...\n", toko[i].id, toko[i].nama);
            
            // --- PROSES MEMUNDURKAN DATA (SHIFTING) ---
            // Timpa data saat ini dengan data di depannya, terus berulang sampai data paling akhir
            for (int j = i; j < jumlah - 1; j++) {
                toko[j] = toko[j + 1];
            }
            
            jumlah--; // Kurangi total data karena udah dihapus 1
            simpanData(); // Auto-save biar di notepad juga kehapus
            printf("Data berhasil dihapus!\n");
            return; 
        }
    }
    printf("Hapus dibatalkan: Data dengan ID '%s' tidak ditemukan!\n", hapusID);
}

// Fungsi untuk mengedit data berdasarkan ID
// Algoritma yang digunakan: Linear Search untuk mencari ID, lalu proses update data
// Proses update data:
// 1. Setelah data yang mau diedit ditemukan, kita langsung update data tersebut dengan inputan baru
// 2. Setelah proses update selesai, kita simpan data ke file lagi agar perubahan juga tersimpan di notepad
void edit() {
    if (jumlah == 0) { 
        printf("Data kosong!\n"); 
        return; 
    }

    char editID[10];
    printf("\nMasukkan ID Emas yang mau diedit: "); 
    scanf(" %[^\n]", editID);
    
    for (int i = 0; i < jumlah; i++) {
        // Cari ID-nya dulu
        if (strcmp(toko[i].id, editID) == 0) { 
            printf("Data ditemukan: %s (%s). Mengedit data...\n", toko[i].id, toko[i].nama);

            printf("Nama Baru  : "); scanf(" %[^\n]", toko[i].nama);
            printf("Berat Baru : "); scanf("%d", &toko[i].berat);
            printf("Harga Baru : "); scanf("%d", &toko[i].harga);
            printf("Stok Baru  : "); scanf("%d", &toko[i].stok);

            simpanData(); // Auto-save biar di notepad juga kehapus
            printf("Data berhasil diedit!\n");
            return; 
        }
    }
    printf("Edit dibatalkan: Data dengan ID '%s' tidak ditemukan!\n", editID);
}

// Fungsi untuk menampilkan semua data perhiasan dan menghitung total aset toko
// Algoritma yang digunakan: Iterasi
// Proses perhitungan total aset:
// 1. Kita iterasi semua data perhiasan yang ada di toko
// 2. Untuk setiap perhiasan, kita hitung nilai totalnya dengan rumus
//    Total Nilai Perhiasan = Harga * Stok
// 3. Kita jumlahkan semua nilai total perhiasan untuk mendapatkan Total Aset Toko
// 4. Setelah iterasi selesai, kita tampilkan semua data perhiasan beserta estimasi total aset toko
void tampil() {
    if (jumlah == 0) { printf("Data kosong!\n"); return; }
    
    int totalAset = 0; 
    
    printf("\n--- DATA PERHIASAN ---\n");
    for (int i = 0; i < jumlah; i++) {
        totalAset += (toko[i].harga * toko[i].stok); 
        printf("%s | %s (%d gram) | Harga: Rp%d | Stok: %d\n", toko[i].id, toko[i].nama, toko[i].berat, toko[i].harga, toko[i].stok);
    }
    printf("-> Estimasi Total Aset Toko: Rp%d\n", totalAset);
}

// Fungsi untuk login admin
// Algoritma yang digunakan: Pengecekan sederhana dengan string (karena hanya ada 1 user admin)
// Proses login:
// 1. User diminta untuk input username dan password
// 2. Inputan username dan password dibandingkan dengan string "admin"
// 3. Jika cocok, login berhasil (return 1), jika tidak cocok, login gagal (return 0) dan program utama akan berhenti
int login() {
    char user[20], pass[20];
    printf("\n--- LOGIN ADMIN ---\nUsername: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);
    
    if (strcmp(user, "admin") == 0 && strcmp(pass, "admin") == 0){
        return 1;
    }else {
        printf("-----------\n");
        printf("Login gagal\n");
        printf("-----------\n");
        return 0;
    }

}

// --- FUNGSI UTAMA (MAIN) ---
int main() {
    bacaData(); // Membaca file saat program pertama kali berjalan
    if (login() == 0) {
        return 0; // Kalau gagal (0), paksa program utama (main) berhenti di sini
    }

    // --- MENU UTAMA ---
    // 1. Menu utama akan terus muncul sampai user memilih opsi keluar (8)
    // 2. Setiap opsi menu akan memanggil fungsi yang sesuai dengan fitur yang dipilih
    // 3. Setelah user memilih opsi keluar (8), program akan menyimpan data ke file dan menampilkan pesan perpisahan sebelum berhenti
    int pilihan;
    do {
        printf("\n--- MENU TOKO EMAS ---\n");
        printf("1. Tambah Data\n2. Hapus Data\n3. Edit Data\n4. Tampilkan & Total Aset\n5. Urutkan (ID)\n6. Urutkan (Harga)\n7. Cari (ID)\n8. Keluar\nPilih: ");
        scanf("%d", &pilihan);

        if (pilihan == 1) tambah();
        else if (pilihan == 2) hapus();
        else if (pilihan == 3) edit();
        else if (pilihan == 4) tampil();
        else if (pilihan == 5) urutID();
        else if (pilihan == 6) urutHarga();
        else if (pilihan == 7) cari();
        
    } while (pilihan != 8);
    
    simpanData(); 
    printf("\nData tersimpan di 'data_toko.txt'. Sampai jumpa!\n");
    return 0;
}

// Renaldi Chandra - 825250116 - Pemrograman II (C) 