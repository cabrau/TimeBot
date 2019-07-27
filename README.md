# TimeBot
O código telegram_bot_ntp.ino código utiliza a placa Wemos para controlar o tempo em que estarão ligados 4 relés através de um bot do Telegram e do protocolo NTP. Enviando mensagens para o bot, pode-se configurar os horários que os relés estarão ligados e desligados. Este projeto foi inicialmente desenvolvido para controlar o tempo de irragação de um jardim, mas pode-se utilizar em qualquer aplicação onde seja necessário controlar o tempo de relés, como lampadas. É necessário criar um bot utilizando o serviço Botfather. Para mais informaçes sobre criação de bots: https://core.telegram.org/bots.

## Alterações necessárias no código:
#### Linha 7: token do seu bot 
#### Linha 17: nome da sua rede WiFi
#### Linha 18: senha da sua rede WiFi
#### Linha 198: seu ID no Telegram

Algumas bibliotecas externas precisam ser instaladas. Basta copiar as pastas para o diretório "libraries" do diretório do Arduino. São essas:

NTPClient
ESP8266WiFi
UniversalTelegramBot

Pinos utilizados: D5, D6, D7 e D8.

## Formato das mensagens: 
RELE1 00:00 23:59

Onde, 
1 -> número do relé (1 a 4)
00:00 -> hora e minutos de início (a partir desse horário liga)
23:59 -> hora e minutos do fim (a partir desse horário desliga)













