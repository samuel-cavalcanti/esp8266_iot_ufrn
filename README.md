# UM Servidor HTTP simples para acender e apagar LEDs

Essa aplicação utiliza o [platformio](https://platformio.org/) e um dev kit nodemcu **esp8266**


Mude o _SSID_ e _PASSWORD_ para a sua rede local

```C++
void connect_to_my_wifi()
{
  auto ssid = "SSID";
  auto password = "PASSWORD";

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Trying to connect ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(700);
    Serial.print(".");
  }

  Serial.print("connected to  ");
  Serial.println(ssid);

  return;
}

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