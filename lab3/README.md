# Laboratorijska vježba 2: Protokoli fizičkog sloja UART interfejsa #

## Ciljevi i ishodi ##
Osnovni cilj vježbe je da student savlada programski interfejs za serijski port u *Linux* operativnom sistemu, kao i da se upozna sa praktičnim aspektima protokola na fizičkom sloju UART interfejsa.

Po završetku vježbe, student će biti sposoban da:

- parametrizuje i razmjenjuje podatke preko UART interfejsa korišćenjem *Linux* programskog interfejsa,
- poveže komponente fizičkog sloja tako da se formira jednostavna RS-485 mreža,
- obezbijedi programsku podršku koja omogućuje razmjenu podataka između dva čvora u RS-485 mreži.

## Priprema za vježbu ##
Očekuje se da je student upoznat (kroz prezentacije na predavanjima i konsultovanje dostupne literature) sa funkcijama programskog interfejsa serijskog porta u *Linux* operativnom sistemu, kao i sa osnovnim karakteristikama komponenata fizičkog sloja UART interfejsa. Osim toga, student bi trebalo da je upoznat sa korišćenjem i opcijama komande `stty` za podešavanje parametara serijskog porta u *Linux* konzoli.

Prije početka vježbe, student treba da ažurira stanje lokalnog repozitorijuma izvršavanjem `git pull` komande u okviru `~/ikm-labs/` direktorijuma. Ako repozitorijum nije ranije preuzet, potrebno ga je klonirati u lokalnom `home` direktorijumu korišćenjem naredbe `git clone https://github.com/knezicm/ikm-labs`. Nakon što je repozitorijum ažuriran/kloniran, potrebno je kopirati folder `lab3` sa cijelim  njegovim sadržajem u `home` direktorijum trenutnog korisnika.

## Programski interefejs serijskog porta (termios API) ##
U *Linux* operativnom sistemu, za kontrolu rada serijskog porta koristi se *termios API* programski interfejs. U osnovi, to je programski interfejs kojim se u UNIX baziranom operativnom sistemu omogućava pristup terminalnom uređaju. Osnovni koncept se ogleda u tome da se svi uređaji vide kao virtuelni fajlovi, u konkretnom slučaju `/dev/ttyAMA0`, kojima se pristupa standardnim funkcijama i sistemskim pozivima za manipulaciju fajlovima.

Da bi mogli da pristupimo nekom fajlu, potrebno je prvo da ga otvorimo. U tu svrhu koristimo sistemski poziv `open()`. U primjeru serijskog porta, fajl se otvara na sljedeći način:

```
int fd;
...
fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
if (fd < 0)
	fprintf(stderr, "Failed to open serial port. Check if it is used by another device.\n");
```

gdje `fd` predstavlja promjenljivu tipa `int` u kojoj se nalazi *file descriptor* fajla koji otvaramo. Ukoliko je ova vrijednost negativna, to znači da je došlo do neke greške pri otvaranju fajla. Prilikom poziva `open()` funkcije, mogu se proslijediti različite opcije kojima se bliže definiše način pristupa fajlu. Tako, na primjer, opcijom `O_RDWR` omogućavamo pristup fajlu sa čitanjem/upisom. Opcijom `O_NOCTTY` se onemogućava da serijski port postane kontrolni terminal procesa koji ga je otvorio, dok opcijom `O_NDELAY` omogućavamo neblokirajući režim rada operacija koje se koriste pri radu sa serijskim portom. Više informacija o ostalim dostupnim opcijama se može pronaći na man stranici [`open()`](http://man7.org/linux/man-pages/man2/open.2.html) sistemskog poziva. Funkcija `frpintf()` je ista kao standardna `printf()` funkcija s tom razlikom da joj se, kao prvi argument, prosljeđuje *file descriptor* fajla u koji se upisuje formatirani string (u konkretnom slučaju, to je `stderr`, tj. *standard error*).

Podaci se šalju i primaju preko serijskog porta korišćenjem `write()` i `read()` sistemskih poziva. Obje funkcije zahtijevaju da im se proslijedi *file descriptor* otvorenog vrituelnog fajla serijskog porta, pokazivač na bafer u kojem se nalaze podaci koje treba poslati, odnosno u koji će se upisati primljeni podaci i broj bajtova. Funkcija vraća broj uspješno poslatih/primljenih bajtova, odnosno negativnu vrijednost ako se desi greška. Dio koda kojim se razmjenjuju podaci preko serijskog porta (softverska *loopback* veza) ima sljedeći izgled:

```
unsigned char  buffer = 's';
...
while (1)
{
	// software loopback
	if (read(fd, &buffer, 1) > 0)
		write(fd, &buffer, 1);
}
```

Po završetku rada sa serijskim portom, fajl je potrebno zatvoriti sistemskim pozivom `close()` kojoj se prosljeđuje *file descriptor* datog fajla:

```
close(fd)
```

Važno je napomenuti da korišćenje prethodnih funkcija zahtjeva uključivanje određenih sistemskih *header* fajlova u kojima su definisani prototipi datih funkcija. S tim u vezi, u zaglavlju programa, treba da se nalaze sljedeće direktive:

```
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
```

Sve prethodno bazira se na standardnim funkcijama i sistemskim pozivima *Linux* operativnog sistema i ne zahtjeva poseban programski interfejs serijskog porta. Međutim, ako želimo da konfigurišemo različite opcije i parametere serijskog porta, odnosno UART interfejsa (npr. bitsku brzinu), potrebno je da koristimo poseban set funkcija i specifične strukture podataka. Programski interfejs *termios API* nudi skup funkcija za podešavanje parametara serijskog porta. Neke od tih funkcija su sljedeće:

|Funkcija|Opis|
|--------|----|
|`tcgetattr()`|Pribavlja trenutne parametre uređaja|
|`tcsetattr()`|Postavlja definisane parametre uređaja|
|`cfgetispeed()`|Pribavlja trenutno podešenu bitsku brzinu prijemne linije|
|`cfgetospeed()`|Pribavlja trenutno podešenu bitsku brzinu predajne linije|
|`cfsetispeed()`|Postavlja bitsku brzinu prijemne linije|
|`cfsetospeed()`|Postavlja bitsku brzinu predajne linije|
|`cfmakeraw()`|Podešava terminal tako da radi u tzv. "sirovom" modu|
|`tcdrain()`|Čeka dok se ne isprazni predajni bafer|
|`tcflow()`|Suspenduje slanje i prijem podataka dok se ne ispuni definisani uslov|
|`tcflush()`|Odbacuje sve neobrađene podatke u baferima|
|`tcsendbrak()`|Generiše *break* signal na predajnoj liniji|

Većina prethodno navedenih funkcija kao argument očekuje pokazivač na `termios` strukturu podataka, koja sadrži sljedeća polja:

```
tcflag_t c_iflag;   /* input modes */
tcflag_t c_oflag;   /* output modes */
tcflag_t c_cflag;   /* control modes */
tcflag_t c_lflags;  /* local modes */
cc_t c_cc[NCCS];    /* special characters */
```

Flegovi u okviru pojedinih polja iz `termios` strukture odgovaraju opcijama koje prihvata `stty` komanda sa kojom su se studenti upoznali u prethodnoj vježbi.

Za više informacija o dostupnim funkcijama i opcijama, preporučuje se da studenti pažljivo prouče [`termios`](http://man7.org/linux/man-pages/man3/termios.3.html) man stranicu.

Primjer podešavanja serijskog porta ima sljedeći izgled:

```
struct termios config;
...
tcgetattr(fd, &config); // get current config
cfmakeraw(&config); // set raw serial communication
config.c_cc[VMIN] = 1; // at least 1 char threshold
config.c_cc[VTIME] = 0; // no timeout
cfsetispeed(&config, B9600); // set receiver baudrate to 9600
cfsetospeed(&config, B9600); // set transmitter baudrate to 9600
tcsetattr(fd, TCSANOW, &config); // update the config
...
```

Parametar `VMIN` označava minimalan broj potrebnih karaktera u baferu da `read()` sistemski poziv vrati vrijednost veću od nula. Parametar `VTIME` definiše *timeout* interval u decisekundama koji će `read()` sistemski poziv čekati da se ispuni uslov o broju potrebnih karaktera u baferu. Opcija `TCSANOW` obezbjeđuje da se željena podešavanja aktuelizuju bez odgode.

Da bi mogli koristiti *termios API*, u zaglavlju fajla je neophodno dodati sljedeću direktivu:

```
#include <termios.h>
```

## Povezivanje RS-485 mreže ##
Treći zadatak se odnosi na temu vezanu za formiranje i testiranje RS-485 mreže. U datom slučaju, radi se o jednostavnoj mreži koja se sastoji samo iz dva čvora poevzana na zajedničku magistralu diferencijalnim linijama A-B. Student najprije treba da se upozna sa električnom šemom i načinom povezivanja [RS-485 modula](https://arduino-info.wikispaces.com/RS485-Modules), a zatim da poveže dva *Raspberry Pi* čvora u RS-485 mrežu. Važno je napomenuti da čvorovi u RS-485 mreži komuniciraju poludupleksnom (engl. *half duplex*) vezom, odnosno da čvorovi imaju dvosmjerni prenos podataka koji ne može da se ostvaruje istovremeno, neophodno je obezbijediti da softverski procesi, koji se izvršavaju na čvorovima, imaju neki vid kontrole komunikacije kako bi se izbjegle kolizije na magistrali. U tom smislu, najčešće se koristi tzv. *master/slave* komunikaciona paradigma, kod koje jedan čvor ima ulogu nadređenog uređaja (engl. *master*) i kontroliše aktivaciju ostalih, podređenih (engl. *slave*), čvorova.

**Napomena:** Pinovi *Raspberry Pi* su predviđeni za rad sa nivoima signala od 3.3V i ne tolerišu priključivanje većih napona (npr. 5V). S druge strane, dati RS-485 modul zahtjeva napajanje od 5V (koje obezbjeđuje *Raspberry Pi* platforma). Da bi povezali RS-485 sa *Raspberry Pi* platformom, potrebno je da se konsultuje MAX485 specifikacija u smislu očekivanih naponskih nivoa. Takođe, potrebno je obezbijediti zaštitu ulaznih pinova *Raspberry Pi* platforme od napona većih od 3.3V koje generiše RS-485 modul. Više informacija o povezivanju GPIO pinova *Raspberry Pi* platforme sa uređajima koji imaju nekompatibilne naponske nivoe, može se pronaći u članku [RPi GPIO Interface Circuits](https://elinux.org/RPi_GPIO_Interface_Circuits#Level_Shifters).

Nakon povezivanja dva RS-485 čvora, potrebno je kroskompajlirati programe u okviru `~/ikm-labs/lab2/lab2-3/` direktorijuma. Kao rezultat kroskompajliranja, dobijaju se dva izvršna fajla: `master` i `slave`. Takođe, preporučuje se da se student upozna sa izvornim kodom ova dva softverska modula (`master.c` i `slave.c`) koji se nalaze u `src` direktorijumu.

Izvršni fajlovi treba da se prenesu na ciljne platforme, i to `master` na jednu (192.168.23.205), a `slave` na drugu (192.168.23.206). Nakon logovanja na obje platforme (u okviru dva zasebna terminala), potrebno je pokrenuti izvršavanje programa na obje platforme. Na ciljnoj platformi na kojoj se pokreće `master`, student treba u terminalu da unosi podatke koji se šalju nakon pritiskanja tastera `ENTER`. Sondu osciloskopa priključiti između diferencijalnih linija A i B, a zatim posmatrati aktivnost na magistrali.

## Zadaci za samostalnu izradu ##

### Zadatak 1: Osnovno podešavanje parametara, slanje bajta i interpretacija poslanog podatka na osciloskopu ###

U zadatku je potrebno uraditi sljedeće:

1. Podesiti parametre UART interfejsa na ciljnoj platformi tako da se šalju 8-bitni podaci brzinom 19200 bps, bez parnosti i sa jedni start i jednim stop bitom.

2. Priključiti sondu osciloskpa tako da se posmatra signal na predajnoj (TX) liniji UART interefejsa na ciljnoj platformi.

3. Poslati podatak 0x55 i uhvatiti njegov izgled na osciloskopu. Analizirati dobijeni talasni oblik i potvrditi da se šalje definisani podatak sa podešenom bitskom brzinom. Identifikovati start i stop bite.

4. Promijeniti bitsku brzinu na 4800 i poslati podatak 0x12. Ponoviti sve iz prethodne tačke.

### Zadatak 2: Napredno podešavanje UART interfejsa sa slanjem i prijemom podataka ###

U zadatku je potrebno uraditi sljedeće:

1. Podesiti parametre UART interfejsa na ciljnoj platformi tako da se šalju 8-bitni podaci brzinom 9600 bps, bez parnosti i sa jedni start i jednim stop bitom.

2. Napraviti hardversku *loopback* konekciju na ciljnoj platformi (povezati RX i TX pin).

3. U jednom terminalu slati podatke pomoću `echo` komande.

4. U drugom terminalu verifikovati primljene podatke pomoću `cat` komande.

5. Otkloniti problem beskonačne petlje na prijemnoj strani odgovarajućim podešavanjem parametara serijskog porta.

