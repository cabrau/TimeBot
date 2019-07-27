# TimeBot
Este código utiliza a placa Wemos para controlar 4 relés através de um bot do telegram. Através do bot, pode-se configurar os horários que os relés estarão ligados e desligados, através de mensagens. É necessário criar um bot utilizando o serviço Botfather. Para mais informaçes, https://core.telegram.org/bots.

## Alterações necessárias no código:
### Linha 7: token do seu bot 
### Linha 17: nome da sua rede WiFi
### Linha 18: senha da sua rede WiFi
### Linha 198: seu ID no Telegram

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













