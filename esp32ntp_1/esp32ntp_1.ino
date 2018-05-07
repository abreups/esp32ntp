/*

Como configurar o protocolo ntp no ESP32 LoRa
Autor: Paulo S. Abreu
04-maio-2018

Referências:
https://github.com/arduino-libraries/NTPClient
Sobre dia, mês e ano:
https://github.com/arduino-libraries/NTPClient/issues/36
e o cálculo pode ser encontrado em:
https://github.com/taranais/NTPClient/blob/master/NTPClient.cpp


*/

// biblioteca para conexão Wi-Fi
#include <WiFi.h>
//biblioteca para comunicação com o display
#include "SSD1306.h"
// bibliotecas para usar protocolo ntp
#include <NTPClient.h>
#include <WiFiUdp.h>

// construtor do objeto do display
// parametros: address,SDA,SCL
// (Documentação da placa WiFi LoRa 32)
SSD1306 tela(0x3c, 4, 15); 

// ssid e senha da rede Wi-Fi à qual
// você pretende se conectar.
const char *ssid     = "ssid";
const char *password = "senha do wifi";

// Cria uma instância UDP para permitir enviar e receber pacotes UDP
WiFiUDP ntpUDP;
int16_t utc = -3; //UTC -3:00 Brazil
// NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
// define servidor ntp do NIC.br
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", utc*3600, 60000);

void setup(){
  Serial.begin(115200);   // para debug apenas
  // Preparação do display

  //configura GPIO16 como saida
  pinMode(16,OUTPUT);       // GPIO16 como saída
  digitalWrite(16, LOW);    // reseta o OLED
  delay(10);                // aguarda pelo menos 5 ms
  digitalWrite(16, HIGH);   // mantém GPIO16 em 1 durante uso do OLED
  tela.init();              // inicializa o display
  tela.clear();             // limpa a tela do display

  // Coloca aviso de que vamos tentar nos conectar ao Wi-Fi
  tela.drawString(0, 0, "Conectando ao Wi-Fi");
  tela.display();

  // Conexão ao Wi-Fi
  WiFi.begin(ssid, password);   // inicializa o Wi-Fi

  // aguarda a conexão à rede Wi-Fi
  // Sai do loop se 60 * 500ms = 30s se passarem sem
  // que conexão seja estabelecida.
  int count = 0;
  while ( WiFi.status() != WL_CONNECTED && count < 60) {
    delay ( 500 );
    count++;
    tela.drawString(count, 10, "o");  // prepara o texto.
    tela.display();                   // mostra o texto no display.
  }
  if (count == 60) {
    tela.clear();   // apaga o texto que está no display
    tela.drawString(0, 0, "Falha tentando conectar ao Wi-Fi");
    tela.display(); // mostra o texto preparado no display
  } else {
    tela.clear();   // apaga o texto que está no display
    tela.drawString(0, 0, "Conectado ao Wi-Fi");
    // mostra o ssid da rede wifi
    tela.drawString(0, 10, WiFi.SSID());
    // mostra o endereço IP recebido
    tela.drawString(0, 20, WiFi.localIP().toString());
    tela.display(); // mostra o texto preparado no display
  }

  timeClient.begin();

}

void loop() {
  timeClient.update();
  int dia_da_semana = ((timeClient.getEpochTime() / 86400L) + 4) %7;
  switch (dia_da_semana) {
   case 0:
     Serial.println("Domingo");
   break;

   case 1:
     Serial.println("Segunda-feira");
   break;

   case 2:
     Serial.println("Terça-feira");
   break;

   case 3:
     Serial.println("Quarta-feira");
   break;

   case 4:
     Serial.println("Quinta-feira");
   break;

   case 5:
     Serial.println("Sexta-feira");
   break;

   case 6:
     Serial.println("Sábado");
   break;
   }
  
  Serial.println(timeClient.getFormattedTime());
  delay(2000);
}
