# Laboratorijska vježba 6: Protokoli CAN i CANopen #

## Priprema za vježbu ##
Prije realizacije vježbe je prvo potrebno potvrditi da je instaliran softverski paket *Linux-CAN / SocketCAN*. Ako je prethodno primijenjen *patch* neophodan za rad sa slLIN modulom, vratiti stanje repozitorijuma na inicijalno stanje komandom:

```
git reset --hard
```

a zatim ponovo kompajlirati projekat komandama

```
make
sudo make install
```

## Zadatak 1: Instalacija i testiranje CAN drajvera ##
Da bi mogao da se koristi CAN interefejs na *Raspberry Pi* platformi, neophodno je obezbijediti odgovarajući hardverski modul koji je povezan na jedan od interfejsa koje ova platforma nudi. U našem slučaju, koristićemo modul *CAN SPI click* proizvođača Mikroelektronika. Ovaj modul sadrži CAN kontroler MCP2515 i CAN transiver MCP2551, koji omogućavaju povezivanje mikrokontrolera (ili mikroprocesora) sa CAN mrežom preko interfejsa. Srećom, za MCP2515 CAN kontroler već postoji podrška u *Linux* operativnom sistemu u vidu drajverskog modula. Da bi se ovaj modul učitao prilikom podizanja sistema na *Raspberry Pi* platformi, potrebno je promijeniti strukturu hardversa sistema u okviru definisanjem odgovarajućih parametara u okviru `/boot/config/txt` fajla. U tom smislu, ovaj fajl treba editovati komandom

```
sudo nano /boot/config.txt
```

i unijeti sljedeće linije

```
dtparam=spi=on
dtoverlay=mcp2515-can0,oscillator=10000000,spimaxfrequency=1000000,interrupt=25
```

ako se radi o verziji *Linux* kernela >= 4.4.x, odnosno

```
dtparam=spi=on
dtoverlay=mcp2515-can0-overlay,oscillator=10000000,spimaxfrequency=1000000,interrupt=25
dtoverlay=spi-bcm2835-overlay
```

za starije verzije *Linux* kernela.

Kada su napravljene opisane izmjene, platformu treba restartovati i provjeriti da li je MCP2515 CAN kontroler uspješno inicijalizovan. U tu svrhu, može se koristiti komanda

```
dmesg | grep can
[    4.112571] mcp251x spi0.0 can0: MCP2515 successfully initialized.
```

pri čemu je u drugoj liniji prikazan tipičan ispis nakon izvršavanja komande (kada je incijalizacija uspješna). Ako se prijavi neka greška, potrebno je provjeriti da li je *CAN SPI click* modul povezan sa SPI interfejsom kako treba.

**Napomena:** Prethodne komande podešavaju komunikaciju sa CAN kontrolerom preko SPI0 kanala platforme, pri čemu se koristi kao SS signal koristi CE0 pin, dok je ulaz za prekid povezan na pin BCM 25 platforme (što je definisano parametrom `interrupt`).

Nakon što je potvrđeno prethodnom provjerom da je CAN kontroler uspješno inicijalizovan, sljedeći korak podrazumijeva aktiviranje CAN interefejsa. Ovo se postiže istim komandama kao kada se radi sa klasičnim mrežnim interfejsima.

```
sudo ip link set can0 up type can bitrate 125000	# enable interface
ip link show dev can0						    	            # print info
sudo ip link set can0 down      					        # disable interface
```

**Napomena:** Važno je definisati tip interfejsa (`type can` segment komande), te podesiti bitsku brzinu CAN mreže (parametar `bitrate`).

Status interfejsa se dodatno može potvrditi komandom

```
dmesg | grep can
[    4.112571] mcp251x spi0.0 can0: MCP2515 successfully initialized.
[  244.793998] IPv6: ADDRCONF(NETDEV_CHANGE): can0: link becomes ready
```

gdje dvije psoljednje linije daju tipičan ispis komadne kada je interfejs uspješno inicijalizovan.

Konačno, CAN mreža se može testirati, tako što se *CAN SPI click* modul (CAN priključak) poveže sa drugim dostupnim modulom sa CAN interfejsom i na *Raspberry Pi* platformi pokrenu komande:

```
cansend can0 127#DEADBEEF
candump can0
```

**Napomena:** Preporučuje se da se student upozna sa opcijama koje nudi komanda `cansend`. Alternativno, može se koristiti i komanda `cangen` iz prethodne vježbe. Posebno treba napomenuti da se komanda `candump` pokreće iz drugog terminala.

Da bi se kompletirala vježba, student treba da provjeri koje poruke dobija od drugog uređaja (ispitivanjem sadržaja u okviru prozora sa `candump` alatkom), odnosno da utvrdi koliko okvira se periodično dobija sa interfejsa, koji su CAN identifikatori ovih okvira i šta je sadržaj (podaci) ovih okvira. O kakvom se tekstu radi ako se dobijeni bajtovi podataka interpretiraju kao ASCII podaci. Takođe, posmatranjem okvira na osiloskopu utvrditi da li i gdje se nalaze umetnuti biti.

Izmijeniti primjer komande `cansend` tako da se omogući slanje dva okvira: prvi sa identifikatorom 0x127 i poljem podataka koje sadrži tekst `hello` i drugi sa identifikatorom 0x128 i poljem podataka koje sadrži tekst `world`.
