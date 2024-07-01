/*
(Input Output Stream)
library utk operasi stream pada input & output
*/
#include <iostream>

/*
(File Stream)
library utk stream pada file seperti read/write
*/
#include <fstream>

/*
(String Stream)
library utk melakukan operasi stream pada string
*/
#include <sstream> 

/* 
(Input Output Manipulation)
library utk memanipulasi input/output
*/
#include <iomanip> 

// library utk generate nomor acak
#include <random>

// library utk memanipulasi string
#include <string>

// library utk tipe data array dinamis
#include <vector>

// library utk tipe data key-value
#include <map>

// memakai nama std di seluruh scope
using namespace std;

// deklarasi ulang fungsi
string formatAngka(int number);
void printTabel(const vector<vector<string>>& tabel, ofstream& output);

// scope utama
int main() {

  // Deklarasi variabel file input stream
  ifstream fileInput("input.csv");

  // Deklararsi variabel file output stream
  ofstream fileOutput;
  fileOutput.open("output.txt");

  // Memastikan file tidak kosong
  if(!fileInput) {
		cerr << "[\x1b[31mERROR\x1b[0m] : file 'input.csv' tidak terdeteksi!" << endl;
		exit(1);
	}

  // Variabel utk menampung isi string sementara
  string str;

  // Deklarasi variabel array 2 dimensi (baris & kolom) utk dijadikan tabel
  vector<vector<string>> tabelHasil;

  /*
  Deklarasi variabel key-value untuk menyimpan banyak suara
  key (kata kunci)-nya berbentuk string yang memiliki value nomor paslon
  valuenya berbentuk integer untuk menampung jumlah suara yg didapat paslon 
  */ 
  map<string, int> suaraKandidat;

  // variabel untuk menyimpan nama kandidat
  vector<string> kandidat;

  // variabel untuk menyimpan banyak suara keseluruhan
  int totalSuara;

  // variabel untuk menyimpan total maksimal suara di satu tps
  int batasMaksimalSuaraTps = 300;

  // variabel untuk menyimpan baris data yang sedang dibaca
  size_t noBaris = 1;

  /*
  Loop yang membaca stream dari file input per baris.
  teks setiap baris akan disimpan kedalam variabel 'str'
  */
  while (getline(fileInput, str)) {

    // variabel untuk menyimpan baris file input
    string isiBaris;

    // variabel untuk membagi variable 'str'
    stringstream ss(str);

    // array untuk menyimpan kolom baris
    vector<string> input;

    // memisahkan isi baris dengan koma dan memasukkannya ke dalam array
    while (getline(ss, isiBaris, ',')) {
      input.push_back(isiBaris);
    }

    // masukkan baris pertama menjadi header (nama kandidat)
    if (noBaris == 1) {
      for (auto& kolom : input) {
        suaraKandidat[kolom] = 0;
        kandidat.push_back(kolom);
      }
      noBaris++;
      continue;
    }

    // variabel untuk menyimpan status validasi input
    bool inputValid = true;

    // variabel untuk menyimpan total suara di satu tps
    int totalSuaraTps = 0;

    // membaca tiap kolom dari baris
    for (auto& kolom : input) {

      // block try-catch untuk menangkap exception / error
      try {
        // totalSuaraTps ditambah nilai kolom yang telah di konversi dari string ke integer
        totalSuaraTps += stoi(kolom);
      }
      // apabila exception / error tedeteksi
      catch (const exception& e) {
        // ubah status validasi menjadi false
        inputValid = false;
      } 
    }

    // ubah status validasi menjadi false apabila jumlah kolom input beda dengan jumlah kandidat
    if (input.size() != kandidat.size()) { inputValid = false; }

    // mengeluarkan pesan error apabila input tidak valid dan totalSuaraTps melebih batas maksimal
    if (!inputValid || totalSuaraTps > batasMaksimalSuaraTps) {
      cerr << "[\x1b[31mERROR\x1b[0m] : baris ke-" << noBaris << " di file 'input.csv' tidak valid!" << endl;
      continue;
    }
    
    // membaca tiap data kandidat
    for (size_t i = 0; i < kandidat.size(); i++) {

      // variabel yang menyimpan nilai kolom yang dikonversi ke integer
      int nilaiKolom = stoi(input[i]);

      // menambah suara di data kandidat
      suaraKandidat[kandidat[i]] += nilaiKolom;

      // memasukkan suara tambahan apabila kandidat adalah "Paslon 02" dan batas maksimal tps tidak terpenuhi
      if (kandidat[i] == "Paslon 02" && totalSuaraTps < batasMaksimalSuaraTps) {
        // variabel utk menyimpan jumlah kekurangan suara tps
        int maksimal = 300 - totalSuaraTps;

        // variabel utk menyimpan setengah dari kekurangan suara tps
        int minimal = maksimal / 2;
      }
    }

    // menambah total suara keseluruhan dengan total suara di tps
    totalSuara += totalSuaraTps;

    // tambah 1 ke nomor baris
    noBaris++;
  }

  // menutup file input setelah selesai membaca
  fileInput.close();

  // variabel array yang menyimpan baris utk tabel hasil
  vector<string> suara, persentaseSuara;

  /*
  Looping array 'kandidat'
  'auto' mendeteksi tipe data secara otomatis
  variabel 'paslon' mewakili tiap data dari array 'kandidat' satu per satu saat looping
  */
  for (auto& kandidat : suaraKandidat) {

    // variabel untuk  menambah simbol '%' di belakang string
    ostringstream persentase;

    // variabel 'persen' berbentuk float (desimal)
    float persen;

    // menyimpan jumlah suara paslon yg sedang di-loop
    int jumlahSuara = kandidat.second;

    /*
    Menghitung persentase suara kandidat yang sedang di-loop
    'static_cast<float>' digunakan utk konversi integer (bulat) ke tipe data float (desimal)
    */
    persen = (static_cast<float>(jumlahSuara) / totalSuara) * 100;

    /*
    Me-format variabel 'persentase' dengan 1 digit desimal dan simbol '%'
    'setprecision(1)' menyatakan bahwa presisi desimalnya adalah 1 (hanya ada 1 digit di belakang koma)
    'fixed menyatakan pemakaian notasi tetap, bukan notasi scientific
    apabila 'fixed tidak dipakai... angka "123000" akan dinyatakan sebagai "1.23e-05"
    */
    persentase << fixed << setprecision(1) << persen << "%";

    // masukkan hasil kalkulasi ke variabel array 'persentaseSuara'
    persentaseSuara.push_back(persentase.str());

    // masukkan jumlah suara ke variabel arrray 'suara'
    str = formatAngka(jumlahSuara) + " suara";
    suara.push_back(str);
  }

  /*
  Memasukkan tiap baris ke dalam variabel array 'tabelHasil'
  */
  // baris 1 = ["01", "02", "03"]
  tabelHasil.push_back(kandidat);
  // baris 2 = ["X suara", "X suara", "X suara"]
  tabelHasil.push_back(suara);
  // baris 3 = ["X.X%", "X.X%", X.X%]
  tabelHasil.push_back(persentaseSuara);

  // keluarkan isi variabel 'tabelHasil' padaa file output dalam bentuk tabel
  printTabel(tabelHasil, fileOutput);

  // tutup file output.txt
  fileOutput.close();

  // mengindikasikan eksekusi sukses
  return 0;
}

/*
Function untuk menambah koma sebagai pemisah digit
*/
string formatAngka(int number) {
  
  // convert variabel number ke tipe data string
  string str = to_string(number);
  
  // ambil jumlah karakter variabel str 
  int len = str.length();

  // jumlah digit yg akan dipisahkan
  int dLen = 3;

  // tambahkan kutip setiap 3 digit
  while (len > dLen) {
    str.insert(len - dLen, 1, ',');
    dLen += 4;
    len += 1;
  }
  return str;
}

/*
Function untuk membuat tabel secara otomatis.
Menerima argumen berbentuk array string 2 dimensi & output filestream
*/
void printTabel(const vector<vector<string>>& tabel, ofstream& output) {
  /* 
  Menghitung lebar kolom dengan mencari
  string yang paling panjang di kolom 
  */

  /*
  Deklarasi variabel array 'lebarKolom' dgn ukuran yg sama dgn jumlah kolom tabel yg diterima
  dan isi dengan nilai default "0".
  'size_t' adalah integer yg hanya bisa menyimpan bilangan positif
  */
  vector<size_t> lebarKolom(tabel[0].size(), 0);

  /*
  Looping tabel.
  Loop luar = looping baris tabel
  Loop dalam = looping kolom tabel
  */
  for (const auto& baris : tabel) {
    for (size_t i = 0; i < baris.size(); ++i) {

      // pilih lebar kolom yang lebih besar antara lebar kolom sebelumnya dan lebar data baru
      lebarKolom[i] = max(lebarKolom[i], baris[i].size());
    }
  }

  // Cetak border atas
  for (size_t i = 0; i < lebarKolom.size(); ++i) {
    
    // cetak border kolom dengan "-" sebagai garis dan "+" sebagai pemisah antara kolom
    output << "+" << string(lebarKolom[i] + 2, '-');
  }
  output << "+" << endl;

  /*
  Cetak konten tabel
  */

  // Deklarasi variabel index tabel
  auto indeksTabel = tabel.begin();

  // Looping tiap baris tabel
  for (const auto& baris : tabel) {

    // looping tiap kolom baris
    for (size_t i = 0; i < baris.size(); ++i) {

      // Gunakan "| " sebagai pemisah kolom dan set lebar kolom menggunakan variabel 'lebarKolom'
      output << "| " << setw(lebarKolom[i] + 1) << left << baris[i];
    }
    output << "|" << endl;

    // apabila loop adalah baris pertama maka cetak border bawah utk menandakan baris header
    if (indeksTabel == tabel.begin()) {
      for (size_t i = 0; i < lebarKolom.size(); ++i) {
        output << "+" << string(lebarKolom[i] + 2, '-');
      }
      output << "+" << endl;
    }

    // tambah index tabel
    indeksTabel++;
  }

  // Cetak border bawah
  for (size_t i = 0; i < lebarKolom.size(); ++i) {
    output << "+"<< string(lebarKolom[i] + 2, '-');
  }
  output << "+";
}