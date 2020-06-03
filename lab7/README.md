# Laboratorijska vježba 7: CAN protokol #

## Ciljevi i ishodi ##
Cilj vježbe je da se student upozna sa osnovnim *can-utils* alatima koji su dio *Linux-CAN / SocketCAN* projekta. Takođe, student bi trebalo da se upozna i sa *BSD Socket* aplikacionim interfejsom.

Po završetku vježbe, student će biti sposoban da:

- poveže dva čvora u jednu CAN mrežu,
- korišćenjem dostupnih alata da pošalje, te primi i interpretira CAN okvire
- razvije jednostavnu aplikaciju koja omogućava prijem i slanje CAN okvira korišćenjem standardnog *BSD Socket* aplikacionog interfejsa

## Priprema za vježbu ##
Očekuje se da je student upoznat (kroz prezentacije na predavanjima i konsultovanje dostupne literature) sa osnovnim pravilima komunikacije CAN protokola.

Prije početka vježbe, student treba da ažurira stanje lokalnog repozitorijuma izvršavanjem `git pull` komande u okviru `~/ikm-labs/` direktorijuma. Ako repozitorijum nije ranije preuzet, potrebno ga je klonirati u lokalnom `home` direktorijumu korišćenjem naredbe `git clone https://github.com/knezicm/ikm-labs`. Nakon što je repozitorijum ažuriran/kloniran, potrebno je kopirati folder `lab7` sa cijelim  njegovim sadržajem u `home` direktorijum trenutnog korisnika.

## Preuzimanje i instalacija *can-utils* softverskog paketa (radi se na razvojnoj platformi) ##

Prvi korak je kloniranje izvornog koda projekta sa repozitorijuma. U tu svrhu, koristimo sljedeću komandu:

```
git clone --depth=1 https://github.com/linux-can/can-utils.git
```

Prethodnu komandu treba izvršiti u okviru radnog direktorijuma laboratorijske vježbe (`lab7`).

Sljedeći korak je kroskompajliranje biblioteke. Kroskompajliranje ćemo obaviti na sličan način kao što smo to radili u slučaju *libmodbus* biblioteke, jer se i u ovom projektu koriste alati za automatizovano kompajliranje projekata. Prvo je potrebno napraviti folder (npr. folder `usr` u radnom direktorijumu laboratorijske vježbe) u kojem će se nalaziti prekompajliranja (binarna) verzija biblioteke sa kojom će se kasnije dinamički linkovati izvršni fajl aplikacije.

```
mkdir usr
```

Nakon toga, prelazimo u folder u kojem se nalazi repozitorijum *can-utils* projekta i pokrećemo niz komandi za konfiguraciju *build* sistema i kompajliranje projekta.

```
cd can-utils
./autogen.sh
./configure --prefix=/path/to/usr --host=arm-linux-gnueabihf
make
make install
```

Kao rezultat, u okviru `usr` foldera dobijamo binarne fajlove alata koji su sastavni dio *can-utils* projekta.

Kopirati `cansend` i `candump` iz foldera `usr` na ciljnu platformu.

## Omogućenje i konfigurisanje CAN drajvera (radi se na ciljnoj platformi) ##
Da bi mogao da se koristi CAN interefejs na *Raspberry Pi* platformi, neophodno je obezbijediti odgovarajući hardverski modul koji je povezan na jedan od interfejsa koje ova platforma nudi. U našem slučaju, koristićemo modul [*CAN SPI click*](https://www.mikroe.com/can-spi-5v-click) proizvođača Mikroelektronika. Ovaj modul sadrži CAN kontroler MCP2515 i CAN transiver MCP2551, koji omogućavaju povezivanje mikrokontrolera (ili mikroprocesora) sa CAN mrežom preko interfejsa. Alternativno, za rad sa CAN mrežom, koristićemo i dodatnu pločicu za *Raspberry Pi* namjenski razvijenu za potrebe ovog kursa.

Za MCP2515 CAN kontroler već postoji podrška u *Linux* operativnom sistemu u vidu drajverskog modula. Da bi se ovaj modul učitao prilikom podizanja sistema na *Raspberry Pi* platformi, potrebno je promijeniti strukturu hardvera sistema definisanjem odgovarajućih parametara u okviru `/boot/config.txt` fajla. U tom smislu, ovaj fajl treba editovati komandom

```
sudo nano /boot/config.txt
```

i unijeti sljedeće linije

```
dtparam=spi=on
dtoverlay=mcp2515-can2,oscillator=10000000,spimaxfrequency=1000000,interrupt=25
```

ako se radi o verziji *Linux* kernela >= 4.4.x, odnosno

```
dtparam=spi=on
dtoverlay=mcp2515-can2-overlay,oscillator=10000000,spimaxfrequency=1000000,interrupt=25
dtoverlay=spi-bcm2835-overlay
```

za starije verzije *Linux* kernela.

**Napomena:** Informaciju o trenutnoj verziji *Linux* kernela možete dobiti komandom `uname -r`.

Kada su napravljene opisane izmjene, platformu treba restartovati komandom `sudo reboot`, a nakon što je platforma ponovo pokrenuta, provjeriti da li je MCP2515 CAN kontroler uspješno inicijalizovan. U tu svrhu, može se koristiti komanda

```
dmesg | grep can
[    4.112571] mcp251x spi0.0 can0: MCP2515 successfully initialized.
```

pri čemu je u drugoj liniji prikazan tipičan ispis nakon izvršavanja komande (kada je incijalizacija uspješna). Ako se prijavi neka greška, potrebno je provjeriti da li je *CAN SPI click* modul, odnosno namjenska dodatna pločica, ispravno povezan sa SPI interfejsom *Raspberry Pi* platforme.

**Napomena:** Prethodne komande podešavaju komunikaciju sa CAN kontrolerom preko SPI1 kanala platforme, pri čemu se kao SS signal koristi CE2 pin, dok je ulaz za prekid povezan na pin BCM 25 platforme (definisano parametrom `interrupt`).

Nakon što je prethodnom provjerom potvrđeno da je CAN kontroler uspješno inicijalizovan, sljedeći korak podrazumijeva aktiviranje CAN interefejsa. Ovo se postiže istim komandama kao kada se radi sa klasičnim mrežnim interfejsima.

```
sudo ip link set can0 up type can bitrate 125000	# enable interface
ip link show dev can0						    	# print info
sudo ip link set can0 down      					# disable interface
```

**Napomena:** Važno je definisati tip interfejsa (`type can` segment komande), te podesiti bitsku brzinu CAN mreže (parametar `bitrate`).

Status interfejsa se dodatno može potvrditi komandom

```
dmesg | grep can
[    4.112571] mcp251x spi0.0 can0: MCP2515 successfully initialized.
[  244.793998] IPv6: ADDRCONF(NETDEV_CHANGE): can0: link becomes ready
```

gdje dvije psoljednje linije daju tipičan ispis komadne kada je interfejs uspješno inicijalizovan.

Konačno, CAN mreža se može testirati, tako što se CAN_H i CAN_L linije *CAN SPI click* modula, odnosno dodatne pločice, povežu sa CAN_H i CAN_L linijama drugog dostupnog modula i na *Raspberry Pi* platformi pokrenu komande:

```
cansend can0 127#DEADBEEF
candump can0 (pokreće se u drugom terminalu)
```

Komanda `cansend` očekuje najmanje tri parametra. Prvi parametar označava CAN mrežni interfejs sa kojim komuniciramo. U našem slučaju, to je `can0`. Drugi parametar (127) predstavlja identifikator CAN okvira u kojem šaljemo podatke. Ovaj parametar mora da sadrži tri heksadecimalne cifre ako se radi o 11-bitnom identifikatoru, odnosno 8 heksadecimalnih cifara ako se radi o 29-bitnom identifikatoru (manje vrijednosti se dopunjavaju vodećim nulama). Treći parametar, koji je od drugog odvojen znakom `#`, predstavlja podatke u heksadecimalnoj notaciji (maksimalno 8 bajtova). Konačno, ako želite da pošaljete *Remote Request* okvir, poslije `#`, a prije podataka, potrebno je staviti `R`, npr. `cansend can0 127#R`.

**Napomena:** Preporučuje se da se student upozna sa opcijama koje nudi komanda `cansend`. Alternativno, može se koristiti i komanda `cangen` iz prethodne vježbe. Posebno treba napomenuti da se komanda `candump` pokreće iz drugog terminala.

## *BSD Socket* aplikacioni interfejs ##

[*BSD Socket*](https://en.wikipedia.org/wiki/Berkeley_sockets)

U drugom zadatku, student treba da se upozna sa interfejsom koji obezbjeđuje *SocketCAN*. Iako je ovaj interfejs kompatibilan sa konceptom *Berkley Socket* interfejsa, zbog specifičnosti CAN adresiranja, preporučuje se da student pri izradi zadatka redovno konsultuje zvaničnu [*SocketCAN* dokumentaciju](https://www.kernel.org/doc/Documentation/networking/can.txt). Alternativno, student može da se upozna sa osnovnom procedurom za prijem i slanje CAN okvira preko *SocketCAN* interfejsa konsultovanjem dodatne literature koja je data u sklopu nastavnih materijala.

Cilj zadatka je da se implementiraju programi *CANsender* i *CANreceiver*. Program *CANsender* treba da implementira funkcionalnost predajnog CAN uređaja iz prethodnog zadatka. Funkcionalnost se može identifikovati na osnovu analize saobraćaja dobijenog u okviru `candump` alata. Kao polazna tačka za implementaciju programa, može se iskoristiti izvorni kod iz fajla `cansend.c`, koji je sastavni dio *Linux-CAN / SocketCAN* projekta.

Program *CANreceiver* treba da realizuje funkcionalnost prijemnog uređaja (kao ekvivalent `candump` alatu). U tom smislu, potrebno je implementirati prijemnik koji sadrži CAN filter za identifikatore okvira koje šalje predajni uređaj i da na terminalu prikazuje saobraćaj ograničen na *CANsender* uređaj. Kao polazna tačka za implementaciju programa, može se iskoristiti izvorni kod iz fajla `candump.c`, koji je sastavni dio *Linux-CAN / SocketCAN* projekta.

**Napomena:** Izvorni kod `candump` alata, je prilično kompleksan, jer pokriva više različitih scenarija upotrebe ovog alata. Da bi se pojednostavile stvari, preporučuje sa da ovaj kod bude samo vodilja u smislu šta je sve neophodno obezbijediti u strukturi ovakvog programa. Takođe, umjesto funkcije `sendmsg()`, za realizaciju funkcionalnosti datog programa, jednostavnije je koristiti funkciju `read()` ili `recv()/recvfrom()`, koja predstavlja ekvivalent funkciji `write()` iz fajla `cansend.c`, koja se koristi pri realizaciji *CANsender* programa.

## Zadaci za samostalnu izradu ##
**Važne napomene:** Izmjene koje je potrebno napraviti u datom izvornom kodu, označene su sa `TODO` u okviru linija sa komentarima.

### Zadatak 1: Eksperimentisanje sa *can-utils* alatima ###

1. Izmijeniti primjer komande `cansend` tako da se omogući slanje dva okvira: prvi sa identifikatorom 0x34 i poljem podataka koje sadrži tekst `hello` i drugi sa identifikatorom 0x123456 i poljem podataka koje sadrži tekst `world`.

2. Uhvatiti sekvencu okvira na osciloskopu i identifikovati pojedina polja definisana CAN protokolom. Posebnu pažnju obratiti na utisnute bite.

3. Pomoću `cansend` alata poslati *Remote Request* okvir kojim se zahtijeva slanje CAN okvira čiji je identifikator 0x34, a zatim uhvatiti ovaj okvir na osciloskopu. U čemu je razlika u odnosu na okvir sa istim identifikatorom iz prethodnih tačaka?

### Zadatak 2: Rad sa *SocketCAN* interfejsom ###

1. 
