/* 
 * 
 * ?post2cell=baris;kolom;nilai
 * ?get2cell=baris;kolom
 * ?impor2tabel=nama_bahan;baris_tabel;bahan_Min;bahan_Persen;bahan_Max
 * ?hitung
 * ?hapus
 *
 * Bahan : Nama bahan (string).
 * BahanMin : Batasan bahan minimal (tidak di implementasikan)
 * BahanPersen : Kuantitas komposisi tiap bahan 
 * BahanMax : Batasan bahan maksimal (tidak di implementasikan)
 * Harga : Harga dalam rupiah per kilogram (dalam satuan rupiah).
 * BK (Bahan Kering) : Persentase bahan kering dari total berat bahan (dalam %).
 * Abu : Persentase kandungan abu dari bahan (dalam %).
 * PK (Protein Kasar) : Persentase protein kasar dalam bahan (dalam %).
 * LK (Lemak Kasar) : Persentase lemak kasar dalam bahan (dalam %).
 * SK (Serat Kasar) : Persentase serat kasar dalam bahan (dalam %).
 * BetaN : Persentase Beta-Nitrogen dalam bahan (dalam %).
 * ME (Metabolizable Energy) : Energi metabolisme dalam bahan, dalam satuan kkal/kg.
 " Ca (Kalsium) : Persentase kandungan kalsium (Ca) dalam bahan (dalam %).
 * P (Fosfor) : Persentase kandungan fosfor (P) dalam bahan (dalam %).
 * Pavl (Fosfor Avalilable)**: Persentase fosfor yang tersedia (dalam %).
 * Lys (Lisin) : Persentase kandungan lisin (dalam %).
 *  Met (Metionin) : Persentase kandungan metionin (dalam %).
 *
 * Repo: https://github.com/aos92/ESP32-FORSUM
 * Simulasi: https://wokwi.com/projects/406553788968332289
 * Sumber:  https://youtu.be/X_HKer3MMQ0?si=7y0mK--LYillmNh1
 *
*/

#include <Arduino.h>

/*
   Format Tabel 1
   Ingredient : Kolom 0
   Harga : Kolom 1
   BK : Kolom 2
   Abu : Kolom 3
   PK : Kolom 4
   LK : Kolom 5
   SK : Kolom 6
   BetaN : Kolom 7
   ME : Kolom 8
   Ca : Kolom 9
   P : Kolom 10
   Pavl : Kolom 11
   Lys : Kolom 12
   Met : Kolom 13
*/
String tabel1[25][14] = {
  {"JAGUNG-LOKAL", "3000", "83.0", "1.76", "8.00", "3.80", "2.20", "84.24", "3300", "0.020", "0.280", "0.090"},
  {"JAGUNG-IMPORT", "3200", "88.0", "1.76", "8.00", "3.80", "2.20", "84.24", "3350", "0.020", "0.280", "0.090"},
  {"DEDEK-HALUS", "1500", "89.7", "7.88", "11.32", "13.00", "8.00", "59.80", "3100", "0.070", "1.500", "0.500"},
  {"BEKATUL", "2000", "88.3", "1.17", "8.90", "1.00", "0.81", "88.12", "3090", "0.090", "0.080", "0.030"},
  {"POLLARD", "2100", "87.4", "4.00", "12.68", "4.00", "15.00", "65.32", "1300", "0.140", "1.150", "0.200"},
  {"GAPLEK", "3000", "87.0", "2.50", "2.50", "0.70", "4.60", "89.70", "2900", "0.150", "0.100", "0.030"},
  {"BUNGKIL-KEDELAI-BRAZIL", "5500", "87.2", "5.59", "46.50", "1.27", "5.60", "41.04", "2550", "0.250", "0.630", "0.210"},
  {"BUNGKIL-KEDELAI-INDIA", "5400", "86.4", "7.00", "42.00", "1.90", "7.00", "42.10", "2290", "0.270", "0.560", "0.190"},
  {"BUNGKIL-KEDELAI-USA", "5600", "87.7", "6.25", "47.50", "1.50", "3.10", "41.65", "2616", "0.250", "0.630", "0.210"},
  {"CGM-LOKAL", "6700", "93.0", "1.40", "64.00", "3.80", "0.81", "29.99", "3500", "0.050", "0.500", "0.170"},
  {"CGM-IMPOR", "7000", "90.0", "2.68", "64.00", "2.91", "0.80", "29.55", "3720", "0.050", "0.500", "0.170"},
  {"BUNGKIL-KACANG-TANAH", "4000", "92.1", "7.37", "43.00", "1.21", "17.40", "31.02", "2650", "0.160", "0.560", "0.060"},
  {"BUNGKIL-KELAPA", "2500", "86.3", "6.65", "18.00", "6.00", "17.33", "52.02", "1500", "0.070", "0.470", "0.150"},
  {"TEPUNG-IKAN", "7500", "91.9", "26.00", "55.00", "7.52", "0.70", "10.78", "2830", "7.190", "2.880", "2.880"},
  {"TEPUNG-IKAN-CP60", "7200", "91.5", "22.00", "60.00", "10.00", "0.70", "7.30", "2730", "5.000", "2.500", "2.500"},
  {"MBM", "7000", "92.1", "6.88", "43.00", "10.93", "2.46", "36.73", "2375", "9.780", "4.500", "4.500"},
  {"MINYAK-IKAN", "10000", "99.0", "0.50", "0.00", "99.00", "0.00", "0.00", "8700", "0.000", "0.000", "0.000"},
  {"CPO", "4500", "99.0", "0.50", "0.00", "99.00", "0.00", "0.00", "7500", "0.000", "0.000", "0.000"},
  {"MOLASSES", "1000", "70.9", "4.00", "4.00", "0.10", "1.00", "90.90", "2280", "0.800", "0.080", "0.040"},
  {"KAPUR", "500", "99.0", "0.00", "0.00", "0.00", "0.00", "0.00", "0", "38.000", "0.000", "0.000"},
  {"CACO3", "750", "100.0", "0.00", "0.00", "0.00", "0.00", "0.00", "0", "40.000", "0.000", "0.000"},
  {"DCP", "10500", "93.5", "0.00", "0.00", "0.00", "0.00", "0.00", "0", "22.720", "17.680", "17.680"},
  {"GARAM", "1150", "99.6", "0.00", "0.00", "0.00", "0.00", "0.00", "0", "0.000", "0.000", "0.000"},
  {"L-LYSIN", "47000", "98.0", "0.00", "95.80", "0.00", "0.00", "3.70", "3990", "0.000", "0.000", "0.000"},
  {"DL-METHIONINE", "50000", "99.0", "0.20", "58.00", "0.00", "0.00", "41.80", "5020", "0.000", "0.000", "0.000"}
};

/*
   Format Tabel 2 dan tabel 3
   Ingredient : Kolom 0
   IngredientMin : Kolom 1
   IngredientPercent : Kolom 2
   IngredientMax : Kolom 3
   Harga : Kolom 4
   BK : Kolom 5
   Abu : Kolom 6
   PK : Kolom 7
   LK : Kolom 8
   SK : Kolom 9
   BetaN : Kolom 10
   ME : Kolom 11
   Ca : Kolom 12
   P : Kolom 13
   Pavl : Kolom 14
   Lys : Kolom 15
   Met : Kolom 16
*/
String tabel2[25][17];
String tabel3[25][17];

void cetakTabel3() {
  Serial.println("Data Dalam Tabel:");
  Serial.println("--------------------------------------------------------------------------------------------");
  Serial.println("Bahan | Min | Qty | Max | Harga | BK | Abu | PK | LK | SK | BetaN | ME | Ca | P | Pavl | Lys | Met");
  Serial.println("--------------------------------------------------------------------------------------------");

  for (int i = 0; i < 25; i++) {
    // Mencetak setiap baris dari tabel 3
    for (int j = 0; j < 17; j++) {
      Serial.print(tabel3[i][j]);
      Serial.print(" | ");
    }
    Serial.println();
  }

  Serial.println("--------------------------------------------------------------------------------------------");

  // Kalkulasi total dari setiap kolom
  float totalBahan = 0;
  unsigned long totalHarga = 0;
  float totalBK = 0;
  float totalAbu = 0;
  float totalPK = 0;
  float totalLK = 0;
  float totalSK = 0;
  float totalBetaN = 0;
  unsigned long totalME = 0;
  float totalCa = 0;
  float totalP = 0;
  float totalPavl = 0;
  float totalLys = 0;
  float totalMet = 0;

  for (int i = 0; i < 25; i++) {
    totalBahan += tabel3[i][2].toFloat();
    totalHarga += tabel3[i][4].toInt();
    totalBK += tabel3[i][5].toFloat();
    totalAbu += tabel3[i][6].toFloat();
    totalPK += tabel3[i][7].toFloat();
    totalLK += tabel3[i][8].toFloat();
    totalSK += tabel3[i][9].toFloat();
    totalBetaN += tabel3[i][10].toFloat();
    totalME += tabel3[i][11].toInt();
    totalCa += tabel3[i][12].toFloat();
    totalP += tabel3[i][13].toFloat();
    totalPavl += tabel3[i][14].toFloat();
    totalLys += tabel3[i][15].toFloat();
    totalMet += tabel3[i][16].toFloat();
  }

  // Cetak hasil total ke Serial
  Serial.print("Total Bahan: ");
  Serial.print(totalBahan);
  Serial.println(" %");
  Serial.print("Total Harga: ");
  Serial.print(totalHarga);
  Serial.println(" /Kg");
  Serial.print("Total BK: ");
  Serial.print(totalBK);
  Serial.println(" %");
  Serial.print("Total Abu: ");
  Serial.print(totalAbu);
  Serial.println(" %");
  Serial.print("Total PK: ");
  Serial.print(totalPK);
  Serial.println(" %");
  Serial.print("Total LK: ");
  Serial.print(totalLK);
  Serial.println(" %");
  Serial.print("Total SK: ");
  Serial.print(totalSK);
  Serial.println(" %");
  Serial.print("Total BetaN: ");
  Serial.print(totalBetaN);
  Serial.println(" %");
  Serial.print("Total ME: ");
  Serial.print(totalME);
  Serial.println(" kcal/kg"); 
  Serial.print("Total Ca: ");
  Serial.print(totalCa);
  Serial.println(" %");
  Serial.print("Total P: ");
  Serial.print(totalP);
  Serial.println(" %");
  Serial.print("Total Pavl: ");
  Serial.print(totalPavl);
  Serial.println(" %");
  Serial.print("Total Lys: ");
  Serial.print(totalLys);
  Serial.println(" %");
  Serial.print("Total Met: ");
  Serial.print(totalMet);
  Serial.println(" %");
}

void aturUlangTabel2() {
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 17; j++) {
      tabel2[i][j] = ""; 
    }
  }
  Serial.println("Tabel telah direset ke nilai default.");
}


String DataDiterima = "", semuaPerintah = "", perintah = "", str1 = "", str2 = "", str3 = "", str4 = "", str5 = "", str6 = "", str7 = "", str8 = "", str9 = "";

void dapatPerintah() {
  DataDiterima = ""; semuaPerintah = ""; perintah = ""; str1 = ""; str2 = ""; str3 = ""; str4 = ""; str5 = ""; str6 = ""; str7 = ""; str8 = ""; str9 = "";
  byte StatusPenerimaan = 0, StatusPerintah = 1, StatusString = 1, StatusTanya = 0, StatusSamaDengan = 0, StatusTitikKoma = 0;
  if (Serial.available()) {
    while (Serial.available()) {
      char c = Serial.read();
      DataDiterima += String(c);

      if (c == '?') StatusPenerimaan = 1;
      if ((c == ' ') || (c == '\r') || (c == '\n')) StatusPenerimaan = 0;

      if (StatusPenerimaan == 1) {
        semuaPerintah += String(c);

        if (c == '=') StatusPerintah = 0;
        if (c == ';') StatusString++;
        if ((StatusPerintah == 1) && ((c != '?') || (StatusTanya == 1))) perintah += String(c);
        if ((StatusPerintah == 0) && (StatusString == 1) && ((c != '=') || (StatusSamaDengan == 1))) str1 += String(c);
        if ((StatusPerintah == 0) && (StatusString == 2) && (c != ';')) str2 += String(c);
        if ((StatusPerintah == 0) && (StatusString == 3) && (c != ';')) str3 += String(c);
        if ((StatusPerintah == 0) && (StatusString == 4) && (c != ';')) str4 += String(c);
        if ((StatusPerintah == 0) && (StatusString == 5) && (c != ';')) str5 += String(c);
        if ((StatusPerintah == 0) && (StatusString == 6) && (c != ';')) str6 += String(c);
        if ((StatusPerintah == 0) && (StatusString == 7) && (c != ';')) str7 += String(c);
        if ((StatusPerintah == 0) && (StatusString == 8) && (c != ';')) str8 += String(c);
        if ((StatusPerintah == 0) && (StatusString >= 9) && ((c != ';') || (StatusTitikKoma == 1))) str9 += String(c);

        if (c == '?') StatusTanya = 1;
        if (c == '=') StatusSamaDengan = 1;
        if ((StatusString >= 9) && (c == ';')) StatusTitikKoma = 1;
      }
      delay(1);
    }
  }
}

void eksekusiPerintah() {
  Serial.println("");
  Serial.println("Data Diterima: " + DataDiterima);
  Serial.println("perintah= " + perintah + " ,str1= " + str1 + " ,str2= " + str2 + " ,str3= " + str3 + " ,str4= " + str4 + " ,str5= " + str5 + " ,str6= " + str6 + " ,str7= " + str7 + " ,str8= " + str8 + " ,str9= " + str9);
  Serial.println("");

  if (perintah == "post2cell") {
    int baris = str1.toInt() - 1;  
    int kolom = str2.toInt() - 1; 
    String nilai = str3;

    if (baris >= 0 && baris < 25 && kolom >= 0 && kolom < 17) {
      tabel2[baris][kolom] = nilai;
      Serial.println("Ok, Berhasil diperbarui [" + String(baris + 1) + "][" + String(kolom + 1) + "] untuk " + nilai);
    } else {
      Serial.println("Ops. Baris atau kolom tidak valid.");
    }

  } else if (perintah == "get2cell") {
    int baris = str1.toInt() - 1;  
    int kolom = str2.toInt() - 1; 

    if (baris >= 0 && baris < 25 && kolom >= 0 && kolom < 17) {
      String nilai = tabel2[baris][kolom];
      Serial.println("Pencarian Data tabel [" + String(baris + 1) + "][" + String(kolom + 1) + "] berisi " + nilai);
    } else {
      Serial.println("Ops. Baris atau kolom tidak valid.");
    }

  } else if (perintah == "impor2tabel") {
    int baris = str2.toInt() - 1; 
    if (baris >= 0 && baris < 25) {
      String namaBahan = str1;
      bool bahanDitemukan = false;
      for (int i = 0; i < 25; i++) {
        if (tabel1[i][0] == namaBahan) {
          tabel2[baris][0] = namaBahan; // Nama Bahan Baku
          tabel2[baris][1] = str3;  // Batasan bahan Minimal
          tabel2[baris][2] = str4;  // Kuantitas komposisi bahan
          tabel2[baris][3] = str5;  // Batasan bahan Maksimal
          tabel2[baris][4] = tabel1[i][1];  // Harga
          tabel2[baris][5] = tabel1[i][2];  // BK
          tabel2[baris][6] = tabel1[i][3];  // Abu
          tabel2[baris][7] = tabel1[i][4];  // PK
          tabel2[baris][8] = tabel1[i][5];  // LK
          tabel2[baris][9] = tabel1[i][6];  // SK
          tabel2[baris][10] = tabel1[i][7]; // BetaN
          tabel2[baris][11] = tabel1[i][8]; // ME
          tabel2[baris][12] = tabel1[i][9]; // Ca
          tabel2[baris][13] = tabel1[i][10]; // P
          tabel2[baris][14] = tabel1[i][11]; // Pavl
          tabel2[baris][15] = tabel1[i][12]; // Lys
          tabel2[baris][16] = tabel1[i][13]; // Met
          Serial.println("Data berhasil diimpor ke tabel pada baris [" + String(baris + 1) + "]");
          bahanDitemukan = true;
          break;
        }
      }
      if (!bahanDitemukan) {
        Serial.println("Bahan tidak ditemukan pada penyimpan internal.");
      }
    } else {
      Serial.println("Ops. Indeks baris tidak valid untuk impor ke tabel.");
    }

  } else if (perintah == "hitung") {
    for (int i = 0; i < 25; i++) {
      // Salin Bahan dari table2 ke tabel 1
      tabel3[i][0] = tabel2[i][0]; // Nama bahan
      tabel3[i][1] = tabel2[i][1]; // Batasan bahan Minimal
      tabel3[i][2] = tabel2[i][2]; // Kuantitas komposisi bahan
      tabel3[i][3] = tabel2[i][3]; // Batasan bahan Maksimal

      // Kalkulasi Harga, BK, Abu, PK, LK, SK, BetaN, ME, Ca, P, Pavl, Lys, Met
      unsigned long harga = tabel2[i][4].toInt();
      unsigned long me = tabel2[i][11].toInt();
      tabel3[i][4]  = String((tabel2[i][2].toFloat() / 100) * harga);  // Harga
      tabel3[i][5]  = String(tabel2[i][2].toFloat() / 100 * tabel2[i][5].toFloat());  // BK
      tabel3[i][6]  = String(tabel2[i][2].toFloat() / 100 * tabel2[i][6].toFloat());  // Abu
      tabel3[i][7]  = String(tabel2[i][2].toFloat() / 100 * tabel2[i][7].toFloat());  // PK
      tabel3[i][8]  = String(tabel2[i][2].toFloat() / 100 * tabel2[i][8].toFloat());  // LK
      tabel3[i][9]  = String(tabel2[i][2].toFloat() / 100 * tabel2[i][9].toFloat());  // SK
      tabel3[i][10] = String(tabel2[i][2].toFloat() / 100 * tabel2[i][10].toFloat()); // BetaN
      tabel3[i][11] = String((tabel2[i][2].toFloat() / 100) * me); // ME
      tabel3[i][12] = String(tabel2[i][2].toFloat() / 100 * tabel2[i][12].toFloat()); // Ca
      tabel3[i][13] = String(tabel2[i][2].toFloat() / 100 * tabel2[i][13].toFloat()); // P
      tabel3[i][14] = String(tabel2[i][2].toFloat() / 100 * tabel2[i][14].toFloat()); // Pavl
      tabel3[i][15] = String(tabel2[i][2].toFloat() / 100 * tabel2[i][15].toFloat()); // Lys
      tabel3[i][16] = String(tabel2[i][2].toFloat() / 100 * tabel2[i][16].toFloat()); // Met
    }
    cetakTabel3();

    } else if (perintah == "hapus") {
    aturUlangTabel2();  
  } else {
    Serial.println("Ops. Perintah Tidak Ditemukan.");
  }
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  dapatPerintah();
  if (semuaPerintah.length() > 0) {
    eksekusiPerintah();
  }
}

