# Laboratorijska vježba 5: Modbus serijski protokol #

## Ciljevi i ishodi ##
Cilj vježbe je da se student upozna sa osnovama Modbus serijskog protokola i da savlada API *libmodbus* biblioteke, koji omogućava realizaciju aplikacija čija je uloga da omogući transfer poruka u Modbus mreži.

Po završetku vježbe, student će biti sposoban da:

- realizuje jednostavnu Modbus mrežu,
- razumije osnove pravila komunikacije u Modbus serijskom protokolu,
- koristi osnovne API funkcije *libmodbus* biblioteke,
- razvije aplikaciju koja omogućava osnovnu komunikaciju sa Modbus *slave* uređajem.

## Priprema za vježbu ##
Očekuje se da je student upoznat (kroz prezentacije na predavanjima i konsultovanje dostupne literature) sa osnovnim pravilima komunikacije Modbus protokola.

Prije početka vježbe, student treba da ažurira stanje lokalnog repozitorijuma izvršavanjem `git pull` komande u okviru `~/ikm-labs/` direktorijuma. Ako repozitorijum nije ranije preuzet, potrebno ga je klonirati u lokalnom `home` direktorijumu korišćenjem naredbe `git clone https://github.com/knezicm/ikm-labs`. Nakon što je repozitorijum ažuriran/kloniran, potrebno je kopirati folder `lab5` sa cijelim  njegovim sadržajem u `home` direktorijum trenutnog korisnika.

## Programski interfejs *libmodbus* biblioteke ##
Instaliranje dodatnih modula

```
sudo apt-get install -y autoconf libtool
```

Kloniranje repozitorijuma

```
git clone https://github.com/dhruvvyas90/libmodbus
```

Kompajliranje i instalacija biblioteke

```
mkdir usr
cd libmodbus
./autogen.sh
./configure ac_cv_func_malloc_0_nonnull=yes --prefix=/path/to/usr --host=arm-linux-gnueabihf
make
make install
```

Da bi koristili prethodno opisane funkcije, potrebno je uključiti odgovarajuće sistemske *header* fajlove u kojima su definisani njihovi prototipi. S tim u vezi, u zaglavlju programa, treba da se nalaze sljedeće direktive:

```
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
```

## Modbus *slave* sa 4-kanalnim relejnim izlazom ##
Modul sa senzorom za mjerenje ubrzanja (akelerometar) ADXL345 posjeduje dva interfejsa za komunikaciju: SPI i I2C. U oba slučaja, ADXL345 ima ulogu *slave* uređaja. I2C interfejs se omogućava ako je CS pin na visokom logičkom nivou. Za SPI, moguće je koristiti trožični i četvorožični mod. Konkretan modul koji se koristi u ovoj vježbi, koristi standardni četvorožični SPI mod.

Modul dostupan za realizaciju laboratorijske vježbe prevezan je tako da se omogući lako povezivanje sa *Raspberry Pi* platformom. Postoje dvije varijante: (1) sa eksternim *pull-up* otpornicima i (2) bez eksternih *pull-up* otpornika (slika ispod).

![ADXL345 moduli](./imgs/accell-boards.png)

Iako druga varijanta modula nema eksterne *pull-up* otpornike, koji su obavezni u slučaju komunikacije preko I2C interfejsa, ovo ne predstavlja ograničenje, jer *Raspberry Pi* na I2C linijama ima *pull-up* otpornike.

Pinovi za povezivanje ADXL345 modula preko prethodno pomenutih serijskih interfejsa, prikazani su na slici ispod.

![ADXL345 pinovi](./imgs/accell-pins.jpg)

Prvo treba obratiti pažnju na pinove za napajanje modula. To su pinovi označeni sa 3.3V i GND (masa), i oni se povezuju posljednji (prvo masa pa onda napajanje).

### Modbus parametri ###
Maksimalna frekvencija serijskog takta za SPI interfejs ADXL345 senzora iznosi 5MHz. Format SPI transakcije sadrži dva ili više bajtova. Prvi bajt je uvijek komanda koja definiše adresu registra kojem se pristupa (najnižih šest bita), sedmi bit (MB) određuje da li se obavlja jednobajtni (MB=0) ili višebajtni (MB=1) prenos podataka. Konačno, najviši R/W bit definiše da li se radi o pristupu sa upisom u registar (R/W=0) ili sa čitanjem registra (R/W=1). Prenos bita treba da se obavlja prema *big-endian* konvenciji (od najvišeg prema najnižem bitu).

Nakon bajta komande, slijedi jedan ili više bajtova (u zavisnosti od toga da li je postavljen MB bit) podataka prema periferiji (ako je pristup sa upisom), odnosno prema mikrokontroleru (ako je pristup sa čitanjem).

Iz talasnih oblika specifikacije proizvođača komponente ADXL345, može se vidjeti da je za četvorožični mod potrebno koristiti sljedeće parametre SPI komunikacije: CPOL=1 i CPHA=1 (tzv. mod 3 kod *spidev API* programskog interfejsa).

Za povezivanje datog modula koriste se sljedeći pinovi:

- SCK/SCL: signal serijskog takta,
- MOSI/SDA: signal MOSI za podatke (od *master* ka *slave* uređaju),
- MISO/ALT: signal MISO za podatke (od *slave* ka *master* uređaju) i
- CS: *slave select* signal za omogućenje SPI prenosa.

Ovi pinovi se povezuju sa odgovarajućim pinovima na konektoru *Raspberry Pi* platforme sa sljedeće slike:

![SPI pinovi](./imgs/rpi-spi-pins.png)

pri čemu u našem slučaju koristimo SPI kontroler SPI0, a periferiju povezujemo na njegov kanal 0 (SPI transakcija se aktivira signalom CE0).

Konačno, potrebno je omogućiti SPI interfejs korišćenjem alatke `raspi-config` ili editovanjem fajla `/boot/config.txt` (tako da postoji linija `dtparam=spi=on`). Ovo je neophodno samo ako SPI interfejs već nije omogućen, što je provjeriti komandom `dmesg | grep spi`.

### I2C interfejs ###
Maksimalna frekvencija serijskog takta za I2C interfejs ADXL345 senzora iznosi 400kHz. Pinovi za povezivanje modula preko I2C interfejsa su sljedeći:

- SCK/SCL: signal serijskog takta,
- MOSI/SDA: signal serijskog podatka i
- MISO/ALT: signal za selekciju adrese *slave* uređaja.

Prema specifikaciji proizvođača, povezivanjem pina za selekciju adrese uređaja (ALT) na napajanje (3.3V) postavljamo 7 -bitnu adresu *slave* uređaja na 0x1D, dok se povezivanjem ovog pina na masu adresa postavlja na 0x53.

Pinovi I2C interfejsa (SCL i SDA) treba da budu povezani sa odgovarajućim pinovima na konektoru *Raspberry Pi* platforme sa sljedeće slike:

![I2C pinovi](./imgs/rpi-i2c-pins.png)

pri čemu u našem slučaju koristimo I2C kontroler I2C1 na pinovima BCM2 i BCM3.

Konačno, potrebno je omogućiti I2C interfejs korišćenjem alatke `raspi-config` ili editovanjem fajla `/boot/config.txt` (tako da postoji linija `dtparam=i2c_arm=on`). Ovo je neophodno samo ako I2C interfejs već nije omogućen, što je provjeriti komandom `dmesg | grep i2c`.

## Zadaci za samostalnu izradu ##
**Važne napomene:** Izmjene koje je potrebno napraviti u datom izvornom kodu, označene su sa `TODO` u okviru linija sa komentarima.

### Zadatak 1: Povezivanje i pristup registrima ADXL345 senzora preko SPI interfejsa ###

U zadatku je potrebno uraditi sljedeće:

1. Dopuniti polazni izvorni kod `spi.c`, koji se nalazi u folderu `lab4-1`, tako da omogući čitanje identifikacionog broja ADXL345 senzora prilikom inicijalizacije, kao i kontinualno čitanje ubrzanja po sve tri ose. Sve pročitane informacije trebaju da se prikazuju u konzoli.

2. Povezati dati ADXL345 modul sa *Raspberry Pi* platformom.

3. Kroskompajlirati dati program i prenijeti dobijeni izvršni fajl na ciljnu platformu, a zatim pokrenuti njegovo izvršavanje.

4. Verifikovati ispravnost rada analizom podataka koji se prikazuju na konzoli. Dodatno, verifikovati ispravnost parametara SPI komunikacije posmatranjem talasnih oblika SCK i MOSI signala na osciloskopu.

### Zadatak 2: Povezivanje i pristup registrima ADXL345 senzora preko I2C interfejsa ###

U zadatku je potrebno uraditi sljedeće:

1. Dopuniti polazni izvorni kod `i2c.c`, koji se nalazi u folderu `lab4-2`, tako da omogući čitanje identifikacionog broja ADXL345 senzora prilikom inicijalizacije, kao i kontinualno čitanje ubrzanja po sve tri ose. Sve pročitane informacije trebaju da se prikazuju u konzoli.

2. Povezati dati ADXL345 modul sa *Raspberry Pi* platformom.

3. Kroskompajlirati dati program i prenijeti dobijeni izvršni fajl na ciljnu platformu, a zatim pokrenuti njegovo izvršavanje.

4. Verifikovati ispravnost rada analizom podataka koji se prikazuju na konzoli. Posmatranjem talasnih oblika I2C transakcije (signali SCL i SDA), utvrditi brzinu komunikacije i format I2C transakcije. U okviru I2C transakcije, identifikujte sve relevantne elemente: start i stop sekvence, adresu uređaja i bit koji definiše način pristupa, ACK/NACK sekvence, te podatke koji se prenose.

5. Modifikovati prethodni program tako da se pritup registrima odvija u dvije umjesto jedne kombinovane I2C transakcije. Verifikovati dobijeni rezultat pomoću osciloskopa.


