# UM Servidor HTTP simples para acender e apagar LEDs

Essa aplicação utiliza o [platformio](https://platformio.org/) e um dev kit nodemcu **esp8266**

Uma simples tutorial que ensina como instalar e usar o PlatformIO: [arduino-vscode-platformio](https://embarcados.com.br/arduino-vscode-platformio/)


crie um arquivo __secrets.h__ e coloquei os valores do  _SSID_ e _PASSWORD_ para a sua rede local

```C++
//secrets.h
#define SSID "SSID"
#define PASSWORD "PASSWORD"

```

Foi montando um circuito onde o led Azul está na porta _D1_ e um led amarelo na porta _D2_
do esp8266

```C++
#define BLUE_LED D1
#define YELLOW_LED D2
....
```

Se tudo der certo, verifica-se se o esp se encontra na
página [esp8266.local](http://esp8266.local/). Considerando que
seu sistema Operacional Suporta **Multicast DNS**. Caso estiver
usando Archlinux, instale e configure o Multicast DNS.

```bash
sudo pacman -S avahi nss-mdns
```

```
Modifique o /etc/nsswitch.conf trocando hosts: files dns myhostname para hosts: files mdns_minimal

hosts: mymachines resolve [!UNAVAIL=return] files mdns_minimal [NOTFOUND=return] dns myhostname
```

```bash
sudo systemctl start avahi-daemon
sudo systemctl enable avahi-daemon
```