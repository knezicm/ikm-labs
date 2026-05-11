# Laboratorijska vježba 7: CAN protokol #

## Ciljevi i ishodi ##
Cilj vježbe je da se student upozna sa osnovnim *can-utils* alatima koji su dio *Linux-CAN / SocketCAN* projekta. Takođe, student bi trebalo da se upozna i sa *BSD Sockets* aplikacionim interfejsom.

Po završetku vježbe, student će biti sposoban da:

- poveže dva čvora u jednu CAN mrežu,
- korišćenjem dostupnih alata da pošalje, te primi i interpretira CAN okvire
- razvije jednostavnu aplikaciju koja omogućava prijem i slanje CAN okvira korišćenjem standardnog *BSD Sockets* aplikacionog interfejsa

## Priprema za vježbu ##
Očekuje se da je student upoznat (kroz prezentacije na predavanjima i konsultovanje dostupne literature) sa osnovnim pravilima komunikacije CAN protokola.

Prije početka vježbe, student treba da ažurira stanje lokalnog repozitorijuma izvršavanjem `git pull` komande u okviru `~/ikm-labs/` direktorijuma. Ako repozitorijum nije ranije preuzet, potrebno ga je klonirati u lokalnom `home` direktorijumu korišćenjem naredbe `git clone https://github.com/knezicm/ikm-labs`. Nakon što je repozitorijum ažuriran/kloniran, potrebno je kopirati folder `lab7` sa cijelim  njegovim sadržajem u `home` direktorijum trenutnog korisnika.

## Preuzimanje i instalacija *can-utils* softverskog paketa (radi se na razvojnoj platformi) ##

Prvi korak je kloniranje izvornog koda projekta sa repozitorijuma. U tu svrhu, koristimo sljedeću komandu:

```sh
git clone --depth=1 --branch v2023.03 https://github.com/linux-can/can-utils.git
```

Prethodnu komandu treba izvršiti u okviru radnog direktorijuma laboratorijske vježbe (`lab7`).

> [!NOTE]  
> Datom komandom se klonira specifična grana repozitorijuma označena sa `v2023.03`, što je u našem slučaju bitno jer novije verzije *can-utils* alata koriste *CMake build* sistem za kroskompajliranje projekta.

Sljedeći korak je kroskompajliranje biblioteke. Kroskompajliranje ćemo obaviti na sličan način kao što smo to radili u slučaju *libmodbus* biblioteke, jer se i u ovom projektu koriste alati za automatizovano kompajliranje projekata. Prvo je potrebno napraviti folder (npr. folder `usr` u radnom direktorijumu laboratorijske vježbe) u kojem će se nalaziti prekompajliranja (binarna) verzija biblioteke sa kojom će se kasnije dinamički linkovati izvršni fajl aplikacije.

```sh
mkdir usr
```

Nakon toga, prelazimo u folder u kojem se nalazi repozitorijum *can-utils* projekta i pokrećemo niz komandi za konfiguraciju *build* sistema i kompajliranje projekta.

```sh
cd can-utils
./autogen.sh
./configure --prefix=/path/to/usr --host=arm-linux-gnueabihf
make
make install
```

Kao rezultat, u okviru `usr` foldera dobijamo binarne fajlove alata koji su sastavni dio *can-utils* projekta.

Kopirati `cansend` i `candump` iz foldera `usr` na ciljnu platformu.

## Omogućenje i konfigurisanje CAN drajvera (radi se na ciljnoj platformi) ##
*Raspberry Pi* platforma nema ugrađen CAN kontroler, pa je neophodno obezbijediti vanjski CAN kontroler koji se sa ovom platformom povezuje preko SPI interfejsa. U našem slučaju u tu svrhu koristimo MCP2515 CAN kontroler, a na raspolaganju su dvije opcije: 
- IKM HAT ploča specijalno projektovana za izvođenje nastave iz ovog predmeta i
- RS485 CAN HAT ploča kompanije Waveshare (više informacija o ovoj ploči možete pronaći na [wiki stranici](https://www.waveshare.com/wiki/RS485_CAN_HAT) ploče)

Ove ploče smo već ranije pominjali i koristili prilikom rada sa RS-485 interfejsom u prethodnim vježbama, tako da se ranije obezbjeđene električne šeme mogu iskoristiti i za ovu vježbu. Obje ploče koriste komponentu SN65HVD230 kao CAN transiver za transliranje signala prijemne (RX) i predajne (TX) linije CAN kontrolera na signale CAN magistrale (CAN_H i CAN_L). Pomenute ploče se razlikuju u izboru SPI interfejsa i njegovog selekcionog signala, što će biti bitno prilikom konfiguracije *Raspberry Pi* platforme koja je opisana u nastavku teksta vježbe.

Za MCP2515 CAN kontroler već postoji podrška u *Linux* operativnom sistemu u vidu drajverskog modula. Da bi se ovaj modul učitao prilikom podizanja sistema na *Raspberry Pi* platformi, potrebno je promijeniti strukturu hardvera sistema definisanjem odgovarajućih parametara u okviru `/boot/config.txt` fajla. U tom smislu, ovaj fajl treba editovati komandom

```sh
sudo nano /boot/config.txt
```

i, ukoliko koristite IKM HAT ploču, unijeti sljedeće linije

```
dtparam=spi=on
dtoverlay=spi1-3cs
dtoverlay=mcp2515-can2,oscillator=10000000,spimaxfrequency=1000000,interrupt=25
```

ako se radi o verziji *Linux* kernela >= 4.4.x, odnosno

```
dtparam=spi=on
dtoverlay=spi1-3cs
dtoverlay=mcp2515-can2-overlay,oscillator=10000000,spimaxfrequency=1000000,interrupt=25
dtoverlay=spi-bcm2835-overlay
```

za starije verzije *Linux* kernela.

> [!TIP]
> Informaciju o trenutnoj verziji *Linux* kernela možete dobiti komandom `uname -r`.

> [!IMPORTANT]  
> Da bi drajver mogao ispravno da se inicijalizuje, obavezno morate da kopirate fajl `mcp2515-can2.dtbo`, koji se nalazi u folderu ove vježbe, u folder `/boot/overlays/` na *Raspberry Pi* platformi.

Ukoliko koristite RS485 CAN HAT ploču, onda prethodno opisana konfiguracija treba da bude prilagođena na sljedeći način:

```
dtparam=spi=on
dtoverlay=mcp2515-can0,oscillator=12000000,interrupt=25,spimaxfrequency=2000000
```

> [!NOTE]  
> Provjerite frekvenciju na kristalu kvarca RS485 CAN HAT ploču pošto postoje dvije varijante izrade. Ukoliko umjesto 12.000 piše 8.000, onda je potrebno prilagoditi konfiguraciju na sljedeći način:
> ```
> dtparam=spi=on
> dtoverlay=mcp2515-can0,oscillator=8000000,interrupt=25,spimaxfrequency=1000000
> ```

Kada su napravljene opisane izmjene, platformu treba restartovati komandom `sudo reboot`, a nakon što je platforma ponovo pokrenuta, provjeriti da li je MCP2515 CAN kontroler uspješno inicijalizovan. U tu svrhu, može se koristiti komanda

```sh
dmesg | grep can
[    4.112571] mcp251x spi0.0 can0: MCP2515 successfully initialized.
```

pri čemu je u drugoj liniji prikazan tipičan ispis nakon izvršavanja komande (kada je incijalizacija uspješna). Ako se prijavi neka greška, potrebno je provjeriti da li je *CAN SPI click* modul, odnosno namjenska dodatna pločica, ispravno povezan sa SPI interfejsom *Raspberry Pi* platforme.

> [!NOTE]
> Kao što možemo da vidimo iz prethodnih konfiguracija, IKM HAT ploča koristi SPI2 interfejs i CE2 selekcioni signal za povezivanje sa CAN kontrolerom. S druge strane, RS485 CAN HAT ploča koristi SPI0 interfejs i CE0 selekcioni signal za povezivanje sa CAN kontrolerom. Obje ploče za signal prekida koriste pin BCM25 *Raspberry Pi* platforme.

Nakon što je prethodnom provjerom potvrđeno da je CAN kontroler uspješno inicijalizovan, sljedeći korak podrazumijeva aktiviranje CAN interefejsa. Ovo se postiže istim komandama kao kada se radi sa klasičnim mrežnim interfejsima.

```sh
sudo ip link set can0 up type can bitrate 125000    # enable interface
ip link show dev can0                               # print info
sudo ip link set can0 down                          # disable interface
```

> [!IMPORTANT]
> Važno je definisati tip interfejsa (`type can` segment komande), te podesiti bitsku brzinu CAN mreže (parametar `bitrate`).

Status interfejsa se dodatno može potvrditi komandom

```sh
dmesg | grep can
[    4.112571] mcp251x spi0.0 can0: MCP2515 successfully initialized.
[  244.793998] IPv6: ADDRCONF(NETDEV_CHANGE): can0: link becomes ready
```

gdje dvije posljednje linije daju tipičan ispis komadne kada je interfejs uspješno inicijalizovan.

Konačno, CAN mreža se može testirati, tako što se CAN_H i CAN_L linije *CAN SPI click* modula, odnosno dodatne pločice, povežu sa CAN_H i CAN_L linijama drugog dostupnog modula i na *Raspberry Pi* platformi pokrenu komande:

```sh
cansend can0 127#DEADBEEF
candump can0 (pokreće se u drugom terminalu)
```

Komanda `cansend` očekuje najmanje tri parametra. Prvi parametar označava CAN mrežni interfejs sa kojim komuniciramo. U našem slučaju, to je `can0`. Drugi parametar (127) predstavlja identifikator CAN okvira u kojem šaljemo podatke. Ovaj parametar mora da sadrži tri heksadecimalne cifre ako se radi o 11-bitnom identifikatoru, odnosno 8 heksadecimalnih cifara ako se radi o 29-bitnom identifikatoru (manje vrijednosti se dopunjavaju vodećim nulama). Treći parametar, koji je od drugog odvojen znakom `#`, predstavlja podatke u heksadecimalnoj notaciji (maksimalno 8 bajtova). Konačno, ako želite da pošaljete *Remote Request* okvir, poslije `#`, a prije podataka, potrebno je staviti `R`, npr. `cansend can0 127#R`.

> [!NOTE]
> Preporučuje se da se student upozna sa opcijama koje nudi komanda `cansend`. Alternativno, može se koristiti i komanda `cangen` iz prethodne vježbe. Posebno treba napomenuti da se komanda `candump` pokreće iz drugog terminala.

## *BSD Sockets* aplikacioni interfejs ##

Aplikacioni interfejs [*BSD Sockets*](https://en.wikipedia.org/wiki/Berkeley_sockets) je standardizovan interfejs za pristup mrežnim uređajima s ciljem prenosa podataka. Konceptualno je zamišljen kao apstrakcija mrežnog interfejsa, odnosno skupa protokola koji se na tom interfejsu koriste, a reprezentuje se tzv. *socket* objektom na dva krajnja čvora koji treba da komuniciraju. Da bi se omogućila komunikacija čvorova, njihove aplikacije treba da se "priključe" na *socket*, nakon čega se podaci mogu razmjenjivati standardnim sistemskim pozivima. Osim komunikacije između umreženih uređaja, *BSD Sockets* interfejs se koristi i kao efikasan mehanizam za komunikaciju između različitih procesa unutar istog računara.

Za potrebe CAN infrastrukture, realizovan je poseban *SocketCAN* aplikacioni interfejs, koji je kompatibilan sa *BSD Sockets* interfejsom. Iako je ovaj interfejs kompatibilan sa konceptom *BSD Sockets* interfejsa, zbog specifičnosti CAN adresiranja, preporučuje se da student konsultuje zvaničnu [*SocketCAN* dokumentaciju](https://www.kernel.org/doc/Documentation/networking/can.txt).

Prvi korak pri radu sa *BSD Sockets* aplikacionim interfejsom je kreiranje samog *socket* objekta:

```c
s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
```

Nakon toga, neophodno je da se *socket* poveže sa mrežnim interfejsom (u našem slučaju to je CAN mrežni interfejs), kako bi znao kome se podaci šalju.

```c
bind(s, (struct sockaddr *)&addr, sizeof(addr));
```

Kod povezivanja, jasno je da je neophodno na odgovarajući način popuniti polja `struct sockaddr` strukture podataka da bi povezivanje bilo uspješno. Ova struktura ima sljedeći izgled:

```c
struct sockaddr_can {
    sa_family_t can_family;
    int         can_ifindex;
    union {
        /* transport protocol class address info (e.g. ISOTP) */
        struct { canid_t rx_id, tx_id; } tp;

        /* reserved for future CAN protocols address information */
} can_addr;
};
```

Za osnovu manipulaciju CAN okvirima, dovoljno je podesiti polja `can_family` i `can_ifindex`. Prvo polje se tipično podešava da bude `AF_CAN`, dok je za drugo neophodno pribaviti indeks na osnovu naziva mrežnog interfejsa. Ovo se može obaviti `ioctl()` sistemskim pozivom na sljedeći način:

```c
int s;
struct sockaddr_can addr;
struct ifreq ifr;
...
strcpy(ifr.ifr_name, "can0" );
ioctl(s, SIOCGIFINDEX, &ifr);

addr.can_family = AF_CAN;
addr.can_ifindex = ifr.ifr_ifindex;
...
```

Konačno, korišćenjem standardnih sistemskih poziva `write()` i `read()`, možemo da šaljemo i primamo podatke preko ovog mrežnog interfejsa. Prije samog slanja, potrebno je da se popune polja strukture podataka `struct can_frame` koja sadrže informacije o strukturi CAN okvira koji šaljemo. Sama struktura ima sljedeći izgled:

```c
struct can_frame {
    canid_t can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    __u8    can_dlc; /* frame payload length in byte (0 .. 8) */
    __u8    __pad;   /* padding */
    __u8    __res0;  /* reserved / padding */
    __u8    __res1;  /* reserved / padding */
    __u8    data[8] __attribute__((aligned(8)));
};
```

Kao što može da se vidi, relevantna polja su `can_id`, koje određuje identifikator CAN okvira, `can_dlc`, koje definiše dužinu poruke izraženo brojem bajtova, i niz bajtova `data` u kojem se nalaze sami podaci. Primjer slanja niza bajtova 0xdeadbeef CAN okvirom čiji je identifikator 0x127 dat je ispod:

```c
struct can_frame frame;
...
frame.can_id = 0x127;
frame.can_dlc = 4;
frame.data[0] = 0xde;
frame.data[1] = 0xad;
frame.data[2] = 0xbe;
frame.data[3] = 0xef;

write(s, &frame, sizeof(frame));
```

Na ekvivalentan način, možemo da primimo podatke:

```c
struct can_frame frame;
int nbytes;
...
nbytes = read(s, &frame, sizeof(frame));

if (nbytes < 0)
{
    fprintf(stderr, "can raw socket read failed\n");
    return 1;
}

/* additional check */
if (nbytes < sizeof(frame)
{
    fprintf(stderr, "read: incomplete CAN frame\n");
    return 1;
}
```

Važno je napomenuti da se u tipičnom scenariju (npr. TCP/IP protokol), prije samog slanja podataka, *socket* treba uspostaviti konekciju sa udaljenim čvorom korišćenjem odgovarajuće *handshake* procedure. Međutim, u slučaju *SocketCAN* interfejsa, ovaj korak nije neophodan.

Po završetku rada sa *SocketCAN* interfejsom, potrebno je pozivom funkcije `close()` osloboditi resurse i zatvoriti prethodno kreirani *socket*.

```c
close(s);
```

*SocketCAN* aplikacioni interfejs nudi dodatne opcije specifične za CAN protokol, koje se mogu podešavati ili preko `ioctl()` sistemskog poziva ili promjenom opcija za *socket* objekat (funkcija `setsockopt()`). Jedna od najznačajnijih opcija u tom smislu je svakako podešavanje filtra okvira na osnovu CAN identifikatora. Parametri filtra se definišu u okviru specifično definisane strukture podataka:

```c
struct can_filter {
    canid_t can_id;
    canid_t can_mask;
};
```

u kojoj polje `can_id` definiše bite identifikatora koji želimo da prihvatimo, dok `can_mask` određuje koji biti u identifikatoru su relevantni, tj. koji biti se ispituju i porede sa vrijednostima datim u polju `can_id`. Sumirano, provjera će rezultovati prihvatanjem okvira ako je zadovoljeno:

```
<received_can_id> & can_mask == can_id & can_mask
```

Jedan primjer podešavanja filtra poruka je dat ispod:

```c
struct can_filter rfilter[2];
...
rfilter[0].can_id   = 0x123;
rfilter[0].can_mask = CAN_SFF_MASK;
rfilter[1].can_id   = 0x200;
rfilter[1].can_mask = 0x700;

setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
```

U primjeru iznad, omogućen je prijem CAN okvira čiji je identifikator jednak 0x123 (filter `rfilter[0]`) i onih okvira kod kojih su najviša tri bita identifikatora postavljena na 0x2 (filter `rfilter[1]`).

Ako želimo da onemogućimo prijem CAN okvira na nekom mrežnom interfejsu, koristićemo sljedeću varijantu filtra:

```c
setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
```

Ova opscija se tipično koristi kod čvorova koji samo šalju CAN okvire, da bi se dodatno optimizovalo iskorišćenje procesora.

Za detaljnije opcije vezane za filtriranje CAN poruka, studenti se upućuju na zvaničnu *SocketCAN* dokumentaciju.

## Zadaci za samostalnu izradu ##
> [!IMPORTANT]
> Izmjene koje je potrebno napraviti u datom izvornom kodu, označene su sa `TODO` u okviru linija sa komentarima.

### Zadatak 1: Eksperimentisanje sa *can-utils* alatima ###
U zadatku je potrebno uraditi sljedeće:

1. Izmijeniti primjer komande `cansend` tako da se omogući slanje dva okvira: prvi sa identifikatorom 0x34 i poljem podataka koje sadrži tekst `hello` i drugi sa identifikatorom 0x123456 i poljem podataka koje sadrži tekst `world`.

2. Uhvatiti sekvencu okvira na osciloskopu i identifikovati pojedina polja definisana CAN protokolom. Posebnu pažnju obratiti na utisnute bite.

3. Pomoću `cansend` alata poslati *Remote Request* okvir kojim se zahtijeva slanje CAN okvira čiji je identifikator 0x34, a zatim uhvatiti ovaj okvir na osciloskopu. U čemu je razlika u odnosu na okvir sa istim identifikatorom iz prethodnih tačaka?

### Zadatak 2: Rad sa *SocketCAN* interfejsom ###
U zadatku je potrebno uraditi sljedeće:

1. Dopuniti izvorni kod `can-sender.c`, koji se nalazi u radnom direktorijumu vježbe, tako da se omogući slanje CAN okvira iz prethodnog zadatka. Ako je potrebno, konsultovati izvorni kod u fajlu `cansend.c`, koji je sastavni dio *Linux-CAN / SocketCAN* projekta.

2. Dopuniti izvorni kod `can-receiver.c`, koji se nalazi u radnom direktorijumu vježbe, tako da se omogući prijem i prikazivanje sadržaja CAN okvira na način koji je sličan `candump` alatu. Takođe, implementirati funkciju filtriranja CAN identifikatora, tako da prijemni uređaj obrađuje samo CAN okvire koje šalje predajnik koji je realizovan u prethodnoj tački. Demonstrirati ispravnost funkcionalnosti filtriranja pomoću `cansend` alata, na način da se pošalje CAN okvir koji ne pripada selektovanim identifikatorima, što treba da rezultuje time da čvor neće reagovati na te okvire. Ako je potrebno, konsultovati implementaciju prijemnika koja se nalazi u `candump.c` fajlu, koji je sastavni dio *Linux-CAN / SocketCAN* projekta.

**Napomena:** Izvorni kod `candump` alata je prilično kompleksan, jer pokriva više različitih scenarija upotrebe ovog alata. Da bi se pojednostavile stvari, preporučuje sa da ovaj kod bude samo vodilja u smislu šta je sve neophodno obezbijediti u strukturi ovakvog programa. Takođe, umjesto funkcije `recvmsg()`, za realizaciju funkcionalnosti datog programa, jednostavnije je koristiti funkciju `read()`, koja predstavlja ekvivalent funkciji `write()` iz fajla `cansend.c`, a koja se koristi pri slanju CAN okvira.
