#include <EEPROM.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define BOTtoken "your token"//Define o Token do bot

String id, text;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", -3 * 60 * 60);

const char *ssid     = "your network";
const char *password = "your password";

int this_min;
int this_hour;

int timeBeginMatrix[4][2] = { {15, 30},
  {15, 30},
  {15, 30},
  {15, 30}
};

int timeEndMatrix[4][2] = { {16, 0},
  {16, 0},
  {16, 0},
  {16, 0}
};

int state_relay[4] = {0, 0, 0, 0};

//int relayPin[4] = {5, 6, 7, 8};


void connect()//Funçao para Conectar ao wifi e verificar à conexao.
{
  if (WiFi.status() != WL_CONNECTED)//Caso nao esteja conectado ao WiFi, Ira conectar-se
  {
    WiFi.begin(ssid, password);//Insira suas informaçoes da rede
    Serial.print ( "." );
    delay(2000);
  } else {
    Serial.println("conectado!");
  }
}

void statusMsg(int rele, String on_off) {
  String m = "RELE";
  m += (rele + 1);
  m += " ";
  m += on_off;
  bot.sendMessage(id, m, "");
}

void checkPins() {

  if (state_relay[0] == 1) {
    digitalWrite(D5, HIGH);
  }
  else {
    digitalWrite(D5, LOW);
  }

  if (state_relay[1] == 1) {
    digitalWrite(D6, HIGH);
  }
  else {
    digitalWrite(D6, LOW);
  }

  if (state_relay[2] == 1) {
    digitalWrite(D7, HIGH);
  }
  else {
    digitalWrite(D7, LOW);
  }

  if (state_relay[3] == 1) {
    digitalWrite(D8, HIGH);
  }
  else {
    digitalWrite(D8, LOW);
  }

}

void checkTime() {
  timeClient.update();
  this_min = timeClient.getMinutes();
  this_hour = timeClient.getHours();

  String this_time = "";
  this_time += this_hour;
  this_time += ":";
  this_time += this_min;
  Serial.println(this_time);

  for (int i = 0; i < 4; i++) {
    int begin_hour = timeBeginMatrix[i][0];
    int begin_min = timeBeginMatrix[i][1];
    int end_hour = timeEndMatrix[i][0];
    int end_min = timeEndMatrix[i][1];

    int begin_time = begin_hour * 60 + begin_min;
    int end_time = end_hour * 60 + end_min;
    int this_time = this_hour * 60 + this_min;

    if (this_time >= begin_time && this_time <= end_time && state_relay[i] == 0) {

      state_relay[i] = 1;
      statusMsg(i, "LIGADO!");

    }

    if ((this_time < begin_time || this_time > end_time) && state_relay[i] == 1) {

      state_relay[i] = 0;
      statusMsg(i, "DESLIGADO!");

    }

  }

}

void parseString(String text, int rele) {
  String time_on = text.substring(6);
  if (time_on.length() != 11 && time_on.length() != 0) {
    String m = "";
    m += "Formato inválido! Exemplo: RELE";
    m += rele;
    m += " 10:35 10:40";
    bot.sendMessage(id, m, "");
  }
  else if (time_on.length() == 0) {
    String m = "RELE";
    m += rele;
    m += " ";
    m +=  timeBeginMatrix[rele - 1][0];
    m += ":";
    m += timeBeginMatrix[rele - 1][1];
    m += " ";
    m +=  timeEndMatrix[rele - 1][0];
    m += ":";
    m += timeEndMatrix[rele - 1][1];

    bot.sendMessage(id, m, "");
    //bot.sendMessage(id, "RELE" + rele + " " + timeBeginMatrix[rele - 1][0] + ":" + timeBeginMatrix[rele - 1][1] + " " + timeEndMatrix[rele - 1][0] + ":" + timeBeginMatrix[rele - 1][1], "");
  }
  else {
    int b_hour = time_on.substring(0, 2).toInt();
    int b_min = time_on.substring(3, 5).toInt();
    int e_hour = time_on.substring(6, 8).toInt();
    int e_min = time_on.substring(9, 11).toInt();

    b_hour = constrain(b_hour, 0, 23);
    b_min = constrain(b_min, 0, 59);
    e_hour = constrain(e_hour, 0, 23);
    e_min = constrain(e_min, 0, 59);

    timeBeginMatrix[rele - 1][0] = b_hour;
    timeBeginMatrix[rele - 1][1] = b_min;
    timeEndMatrix[rele - 1][0] = e_hour;
    timeEndMatrix[rele - 1][1] = e_min;

    writeEEPROM(rele);

    String m = "NOVO HORÁRIO RELE";
    m += rele;
    m += " ";
    m +=  timeBeginMatrix[rele - 1][0];
    m += ":";
    m += timeBeginMatrix[rele - 1][1];
    m += " ";
    m +=  timeEndMatrix[rele - 1][0];
    m += ":";
    m += timeEndMatrix[rele - 1][1];

    Serial.println(e_min);
    bot.sendMessage(id, m, "");
  }

}


void readTel()//Funçao que faz a leitura do Telegram.
{
  int newmsg = bot.getUpdates(bot.last_message_received + 1);

  for (int i = 0; i < newmsg; i++)//Caso haja X mensagens novas, fara este loop X Vezes.
  {
    id = bot.messages[i].chat_id;//Armazenara o ID do Usuario à Váriavel.
    if (id != "31352624") {
      bot.sendMessage(id, "Você não tem permissão para falar aqui! Vá para casa merendar!", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      continue;
    }
    text = bot.messages[i].text;//Armazenara o TEXTO do Usuario à Váriavel.
    text.toUpperCase();//Converte a STRING_TEXT inteiramente em Maiuscúla.
    text.trim();

    if (text.indexOf("RELE1") > -1)
    {
      parseString(text, 1);

    } else if (text.indexOf("RELE2") > -1) {

      parseString(text, 2);

    } else if (text.indexOf("RELE3") > -1) {

      parseString(text, 3);

    } else if (text.indexOf("RELE4") > -1) {

      parseString(text, 4);
    }

    else//Caso o texto recebido nao for nenhum dos acima, Envia uma mensagem de erro.
    {
      bot.sendMessage(id, "Formato inválido! Exemplo: RELE1 10:35 10:40", "");
    }
  }

}

void readEEPROM() {
  int address = 0;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 2; j++) {
      timeBeginMatrix[i][j] =  EEPROM.read(address);
      address++;
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 2; j++) {
      timeEndMatrix[i][j] =  EEPROM.read(address);
      address++;
    }
  }

}

void writeEEPROM(int rele) {
  int begin_hour = timeBeginMatrix[rele - 1][0];
  int begin_min = timeBeginMatrix[rele - 1][1];
  int end_hour = timeEndMatrix[rele - 1][0];
  int end_min = timeEndMatrix[rele - 1][1];

  int begin_addr = (rele - 1) * 2;
  int end_addr = (rele + 3) * 2;

  EEPROM.write(begin_addr, begin_hour);
  begin_addr++;
  EEPROM.write(begin_addr, begin_min);

  EEPROM.write(end_addr, end_hour);
  end_addr++;
  EEPROM.write(end_addr, end_min);

  EEPROM.commit();

}


void setup() {
  Serial.begin(115200);
  EEPROM.begin(16);
  readEEPROM();
  WiFi.mode(WIFI_STA);//Define o WiFi como Estaçao
  connect();
  timeClient.begin();

  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
}

void loop() {
  connect();//Funçao para verificar se ainda há conexao
  readTel();//Funçao para ler o telegram
  checkTime();
  checkPins();
  delay(1000);

}


