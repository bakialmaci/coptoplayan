#include <Servo.h>
Servo kapak_sag;
Servo kapak_sol;
Servo servokol;


//Sol ON Mesafe Sensörü
#define solons_echo 35
#define solons_trig 34
//Sol ARKA Mesafe Sensörü
#define solarkas_echo 38
#define solarkas_trig 39
//Hazne Mesafe Sensörüd
#define hs_echo 32
#define hs_trig 33
//Duvar Mesafe Sensörü
#define dvr_echo 48
#define dvr_trig 49
//ARKA Mesafe Sensörü
#define arka_echo 36
#define arka_trig 37
//Motorlar
#define sag_motor1 42 // İLERİ sağ
#define sag_motor2 40 //GERİ sağ
#define sag_pwm 8 // SAG pwm
#define sol_motor1 43 //SOL İLERİ
#define sol_motor2 41 //SOL GERİ
#define sol_pwm 9 //SOL pwm
//Hazne Renk Sensörü
#define hs_S0 22
#define hs_S1 24
#define hs_S2 26
#define hs_S3 28
#define hs_SOut 30
//Çöp Kutu Renk Sensörü
#define ks_S0 23
#define ks_S1 25
#define ks_S2 27
#define ks_S3 29
#define ks_SOut 31

#define pwm 60
#define kp 6
#define ideal 7
unsigned long t = 0;
int fark;
int fark2 = 0;
int hata = 0;
int arka;
int solon;
int solarka;
int duvar;
int hazne;
int state = 0;
int pid;
int solonmod;
int solarkamod;

void duvartakip(int x)
{
  digitalWrite(sol_motor1, LOW);
  digitalWrite(sol_motor2, HIGH);
  digitalWrite(sag_motor1, LOW);
  digitalWrite(sag_motor2, HIGH);
  analogWrite(sol_pwm, pwm - x);
  analogWrite(sag_pwm, pwm + x); //+ 5 i sildik 20.05
}
void ileri() { //TAMAM

  digitalWrite(sol_motor1, HIGH);
  digitalWrite(sol_motor2, LOW);
  digitalWrite(sag_motor1, HIGH);
  digitalWrite(sag_motor2, LOW);
  analogWrite(sol_pwm, pwm);
  analogWrite(sag_pwm, pwm);
}
void geri() { //TAMAM
  digitalWrite(sol_motor1, LOW);
  digitalWrite(sol_motor2, HIGH);
  digitalWrite(sag_motor1, LOW);
  digitalWrite(sag_motor2, HIGH);
  analogWrite(sol_pwm, pwm);
  analogWrite(sag_pwm, pwm);
}
void dur() { //TAMAM
  digitalWrite(sol_motor1, LOW);
  digitalWrite(sol_motor2, LOW);
  digitalWrite(sag_motor1, LOW);
  digitalWrite(sag_motor2, LOW);
  analogWrite(sol_pwm, 0);
  analogWrite(sag_pwm, 0);
}
void fren() { //TAMAM
  digitalWrite(sol_motor1, !digitalRead(sol_motor1));
  digitalWrite(sol_motor2, !digitalRead(sol_motor2));
  digitalWrite(sag_motor1, !digitalRead(sag_motor1));
  digitalWrite(sag_motor2, !digitalRead(sag_motor2));
  analogWrite(sol_pwm, 200);
  analogWrite(sag_pwm, 200);
  delay(20);
}
void sol() { //TAMAM
  digitalWrite(sol_motor1, LOW);
  digitalWrite(sol_motor2, HIGH);
  digitalWrite(sag_motor1, HIGH);
  digitalWrite(sag_motor2, LOW);
  analogWrite(sol_pwm, pwm - 5);
  analogWrite(sag_pwm, pwm - 5);
}
void sag() { //TAMAM
  digitalWrite(sol_motor1, HIGH);
  digitalWrite(sol_motor2, LOW);
  digitalWrite(sag_motor1, LOW);
  digitalWrite(sag_motor2, HIGH);
  analogWrite(sol_pwm, pwm);
  analogWrite(sag_pwm, pwm);
}

void duvarayaklas(int x) {
  digitalWrite(sol_motor1, LOW);
  digitalWrite(sol_motor2, HIGH);
  digitalWrite(sag_motor1, LOW);
  digitalWrite(sag_motor2, HIGH);
  analogWrite(sol_pwm, pwm - x);
  analogWrite(sag_pwm, pwm + x);//+ 5 i sildik 20.05
  digitalWrite(sol_motor1, LOW);
  digitalWrite(sol_motor2, HIGH);
  digitalWrite(sag_motor1, LOW);
  digitalWrite(sag_motor2, HIGH);

}
int haznesensor() { //TAMAM
  delay(10);
  long sure, mesafe;
  digitalWrite(hs_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(hs_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(hs_trig, LOW);
  sure = pulseIn(hs_echo, HIGH);
  mesafe = (sure / 2) / 29.1;
  if (mesafe > 200 || mesafe < 0) {
    Serial.println("Menzil Disi");
  } else {
    Serial.print("Hazne=");
    Serial.print(mesafe);
    Serial.println(" cm");
    return mesafe;
  }
}
int duvarsensor() { //TAMAM
  delay(10);
  long sure, mesafe;
  digitalWrite(dvr_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(dvr_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(dvr_trig, LOW);
  sure = pulseIn(dvr_echo, HIGH);
  mesafe = (sure / 2) / 29.1;
  if (mesafe > 200 || mesafe < 0) {
    Serial.println("Menzil Disi");
  } else {
    Serial.print("Duvar=");
    Serial.print(mesafe);
    Serial.println(" cm");
    return mesafe;
  }
}
int arkasensor() { //TAMAM
  delay(10);
  long sure, mesafe;
  digitalWrite(arka_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(arka_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(arka_trig, LOW);
  sure = pulseIn(arka_echo, HIGH);
  mesafe = (sure / 2) / 29.1;
  if (mesafe > 200 || mesafe < 0) {
    Serial.println("Menzil Disi");
    return 200;
  } else {
    Serial.print("Arka sensor=");
    Serial.print(mesafe);
    Serial.println(" cm");
    return mesafe;
  }
}
int solonsensor() { //TAMAM
  delay(10);
  long sure, mesafe;
  digitalWrite(solons_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(solons_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(solons_trig, LOW);
  sure = pulseIn(solons_echo, HIGH);
  mesafe = (sure / 2) / 29.1;
  digitalWrite(solons_echo, LOW);
  if (mesafe > 200 || mesafe < 0) {
    Serial.println("Menzil Disi");
  } else {
    Serial.print("Sol Ön=");
    Serial.print(mesafe);
    Serial.println(" cm");
    return mesafe;
  }
}
int solarkasensor() { //TAMAM
  delay(10);
  long sure, mesafe;
  digitalWrite(solarkas_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(solarkas_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(solarkas_trig, LOW);
  sure = pulseIn(solarkas_echo, HIGH);
  mesafe = (sure / 2) / 29.1;
  digitalWrite(solarkas_echo, LOW);
  if (mesafe > 200 || mesafe < 0) {
    Serial.println("Menzil Disi");
  } else {
    Serial.print("Sol arka=");
    Serial.print(mesafe);
    Serial.println(" cm");
    return mesafe;
  }
}
/*
  void bes_cm(int deger){
  int mesafe = arkasensor();

  while((mesafe - deger) < 3){
    mesafe = arkasensor();
    ileri();
    }
  }
*/
int renkoku(int x) {
  int green, red, blue;
  if (x == 0) { // x 0 ise hazne sensor
    for (int i = 0; i < 9; i++) {
      digitalWrite(hs_S2, LOW);
      digitalWrite(hs_S3, LOW);
      //count OUT, pRed, RED
      red = red + pulseIn(hs_SOut, digitalRead(hs_SOut) == HIGH ? LOW : HIGH);
      digitalWrite(hs_S3, HIGH);
      //count OUT, pBLUE, BLUE
      blue = blue + pulseIn(hs_SOut, digitalRead(hs_SOut) == HIGH ? LOW : HIGH);
      digitalWrite(hs_S2, HIGH);
      //count OUT, pGreen, GREEN
      green = green + pulseIn(hs_SOut, digitalRead(hs_SOut) == HIGH ? LOW : HIGH);
      delay(20);
    }
    red = red / 10;
    green = green / 10;
    blue = blue / 10;
    Serial.print("R Intensity:");
    Serial.print(red, DEC);
    Serial.print(" G Intensity: ");
    Serial.print(green, DEC);
    Serial.print(" B Intensity : ");
    Serial.print(blue, DEC);
    if (red < blue && red < green && red < 210 && green > 300)
    {
      Serial.println(" - (Red Color)");
      return 0; // Kırmızıysa 0
    }

    else if (blue < red && blue < green && blue < 260 && green > 300)
    {
      Serial.println(" - (Blue Color)");
      return 2; // Maviyse 2
    }

    else if (green < red && green - blue < 30 && green < 300 )
    {
      Serial.println(" - (Green Color)");
      return 1; // Yeşilse 1
    }
    else if ((blue - red) > 15 && green < 300) {
      Serial.println(" - (Yellow Color)");
      return 3; // Sarıysa 3
    }
    else {
      Serial.println();
      return 4; // Beyazsa 4
    }
    delay(20);
  }
  if (x == 1) { //x 1 ise kutu sensor
    for (int i = 0; i < 9; i++) {
      digitalWrite(ks_S2, LOW);
      digitalWrite(ks_S3, LOW);
      //count OUT, pRed, RED
      red = red + pulseIn(ks_SOut, digitalRead(ks_SOut) == HIGH ? LOW : HIGH);
      digitalWrite(ks_S3, HIGH);
      //count OUT, pBLUE, BLUE
      blue = blue + pulseIn(ks_SOut, digitalRead(ks_SOut) == HIGH ? LOW : HIGH);
      digitalWrite(ks_S2, HIGH);
      //count OUT, pGreen, GREEN
      green = green + pulseIn(ks_SOut, digitalRead(ks_SOut) == HIGH ? LOW : HIGH);
      delay(20);
    }
    red = red / 10;
    green = green / 10;
    blue = blue / 10;
    Serial.print("R Intensity:");
    Serial.print(red, DEC);
    Serial.print(" G Intensity: ");
    Serial.print(green, DEC);
    Serial.print(" B Intensity : ");
    Serial.print(blue, DEC);
    if (red < blue && red < green && red < 80 && blue > 65 && green > 80)
    {
      Serial.println(" - (Red Color)");
      return 0; // Kırmızıysa 0
    }

    else if (blue < red && blue < green && blue < 80 && (red - blue) > 40 && (red - green) > 20)
    {
      Serial.println(" - (Blue Color)");
      return 2; // Maviyse 2
    }

    else if (green < red && green < blue )
    {
      Serial.println(" - (Green Color)");
      return 1; // Yeşilse 1
    }
    else if ((green && red) < blue && (blue - red) > 15 && (blue - green) > 15) {
      Serial.println(" - (Yellow Color)");
      return 3; // Sarıysa 3
    }
    else {
      Serial.println();
      return 4; // Beyazsa 4
    }
    delay(20);
  }
}
void kapakac() {
  for (int i = 1; i <= 150; i++) {
    kapak_sag.write(i); // servo kapağın ilk pozisyonu
    kapak_sol.write(180 - i); // servo kapağın ilk pozisyonu
    delay(10);
  }
}
void copal() { // kapak kapat
  for (int i = 1; i <= 150; i++) {
    kapak_sag.write(150 - i); // servo kapağın ilk pozisyonu
    kapak_sol.write(i + 30); // servo kapağın ilk pozisyonu
    delay(10);
  }
}
void copara() {
  int a = 0;
  while (1) {
    if (!(duvarsensor() <= 13) && haznesensor() - duvarsensor() <= 6) { //Önünde engel yokken
      ileri();
      if ((haznesensor() - duvarsensor()) <= 6) { // önünde çöp gördüğünde
        copal();

        break;
      }
    }
    if (duvarsensor() <= 13 && (haznesensor() - duvarsensor()) <= 6) { // duvara geldiğinde
      while (1) {
        sag();
        if ((solonsensor() - solarkasensor() < 2) || (solarkasensor() - solonsensor() < 2)) {
          sag();
          delay(400);
          fren();
          dur();
          delay(250);

          if ((solonsensor() - solarkasensor() < 2) && (solarkasensor() < 15) && (solonsensor() < 15)) {
            break;
          }

        }
      }
    }
  }
}
void kutuara() {
  arka = arkasensor();
  //  delay(10);
  //  solon = solonsensor();
  //  delay(10);
  //  solarka = solarkasensor();
  if (arka <= 10) { // tamam
    fren();
    // bes_cm(arka);
    sol();
    delay(600);
    while (1) {
      arka = arkasensor() % 30;
      delay(5);
      solon = solonsensor() % 30;
      delay(5);
      solarka = solarkasensor() % 30;
      sol();
      if ((solon - solarka < 2) || (solarka - solon < 2)) {
        sol();
        delay(250);
        fren();
        dur();
        delay(100);
        arka = arkasensor();
        delay(1);
        solon = solonsensor();
        delay(1);
        solarka = solarkasensor();

        if (!(arka < 15) && (solarka < 15) && (solon < 15)) {
          break;
        }

      }
    }
  } else {
    if (solarkasensor() - solonsensor() < 25) {
      if (solarkasensor() <= ideal) {
        fark = solarkasensor() - solonsensor() + 1;
        Serial.print("FARK");

        Serial.println(fark);

        fark = fark * kp;
        duvartakip(fark);
      } else {
        int sensor = solarkasensor();
        t = millis();
        fark =  ideal - sensor ;
        hata = ((fark) * 8 + 10 * ((fark - fark2) / t)) / 5;
        fark2 = fark;
        duvarayaklas(-hata);
        delay(200);
        duvarayaklas(hata);
        delay(100);
      }
    } else {
      solonmod = solonsensor() % 30;
      solarkamod = solarkasensor() % 30;
      while (state == 0) { // Duvardan sol arka sensorü boşa çıkartırken
        solonmod = solonsensor() % 30;
        solarkamod = solarkasensor() % 30;
        pid = (solarkamod - solonmod) * 5;
        duvartakip(-pid);
        if (solonmod - solarkamod < 3) {
          Serial.println("MOD");
          //geri();
          //delay(900);
          state++;

          fren();
          dur();
          break;
        }
      }
      while (duvarsensor() < 32) {
        solonmod = solonsensor()%30;
        solarkamod=solarkasensor()%30;
        pid=(solarkamod-solonmod)*20;
        duvartakip(-pid);
      }
      fren();
      dur();
      delay(1000);
      sag();
      delay(800);
      while (state == 1) { // Sağa dönme
        sag();
        delay(400);
        fren();
        dur();
        delay(100);
        solonmod = solonsensor() % 30;
        solarkamod = solarkasensor() % 30;
        if ((solonmod - solarkamod <= 2 || solarkamod - solonmod <= 2 ) && arkasensor() < 40) {
          state = 0;
          
          while(arkasensor()<10){
            pid = (solarkasensor() % 30 - solonsensor() % 30)*5;
            duvartakip(pid);
          }
          fren();
          dur();
          break;
        }

      }
    }
  }
}


void setup() {
  pinMode(sag_pwm, OUTPUT);
  pinMode(sag_motor1, OUTPUT);
  pinMode(sag_motor2, OUTPUT);
  pinMode(sol_pwm, OUTPUT);
  pinMode(sol_motor1, OUTPUT);
  pinMode(sol_motor2, OUTPUT);                        //for motor driver with 2 dir pins
  pinMode(hs_trig, OUTPUT);
  pinMode(hs_echo, INPUT);
  pinMode(dvr_trig, OUTPUT);
  pinMode(dvr_echo, INPUT);
  pinMode(arka_trig, OUTPUT);
  pinMode(arka_echo, INPUT);

  pinMode(solons_trig, OUTPUT);
  pinMode(solons_echo, INPUT);
  pinMode(solarkas_trig, OUTPUT);
  pinMode(solarkas_echo, INPUT);

  pinMode(hs_S0, OUTPUT);
  pinMode(hs_S1, OUTPUT);
  pinMode(hs_S2, OUTPUT);
  pinMode(hs_S3, OUTPUT);
  pinMode(hs_SOut, INPUT);
  digitalWrite(hs_S0, HIGH);
  digitalWrite(hs_S1, LOW);

  pinMode(ks_S0, OUTPUT);
  pinMode(ks_S1, OUTPUT);
  pinMode(ks_S2, OUTPUT);
  pinMode(ks_S3, OUTPUT);
  pinMode(ks_SOut, INPUT);
  digitalWrite(ks_S0, HIGH);
  digitalWrite(ks_S1, LOW);
  servokol.attach(51);
  servokol.write(0);  // servonun ilk pozisyon açısı
  kapak_sag.attach(46);
  kapak_sag.write(0); // servo kapağın ilk pozisyonu
  kapak_sol.attach(47);
  kapak_sol.write(180); // servo kapağın ilk pozisyonu
  servokol.attach(51);
  servokol.write(150);  // servonun ilk pozisyon açısı
  // delY 50 HERBİR İ DEN SONRA FORUN İÇİNDE
  Serial.begin(9600);
}
void loop() {
  kutuara();
}




