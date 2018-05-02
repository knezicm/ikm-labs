# Laboratorijska vježba 5: LIN protokol #

## Priprema za vježbu ##
Laboratorijska vježba se bazira na realizaciji LIN *kernel-space* modula za *Linux* operativni sistem. Detaljnije informacije, svi neophodni resursi, kao i dokumentacija, može se pronaći na web stranici [slLIN - LIN Driver for Standard UART/TTY Interfaces](https://rtime.felk.cvut.cz/can/lin-bus/).

Da bi se izbjegle nekompatibilnosti između korišćenih softverskih paketa, preporučuje se da se uradi ažuriranje lokalne baze softverskih paketa i njihova reinstalacija pomoću komandi:

```
sudo apt-get update
sudo apt-get upgrade
```

**Napomena:** Komanda `sudo apt-get upgrade` zahtjeva značajno vrijeme da se kompletira ažuriranje sistema. Stoga je ne trebate pokretati ako nije neophodno.

## Zadatak 1: Instalacija slLIN modula ##

Prvo je potrebno preuzeti izvorni kod korišćenjem komande:

```
git clone git://rtime.felk.cvut.cz/linux-lin.git
```

**Napomena:** Alternativno, repozitorijum ovog projekta se može preuzeti i sa https://github.com/trainman419/linux-lin.

S obzirom da se slLIN kompajlira i učitava u sistem kao *kernel-space* modul, da bi se moglo obaviti prevođenje, neophodno je da se na *Raspberry Pi* platformi nalazi izvorni kod *Linux* operativnog sistema. Za ovu platformu, to se može postići komandom

```
sudo apt-get install raspberrypi-kernel-headers
```

**Napomena:** Često nakon ažuriranja lokalne baze berzija softverskih paketa komandom `sudo apt-get update`, dolazi do neusklađenosti trenutne verzije izvornog koda kernela i binarne verzije kernela koja se učitava prilikom pokretanja sistema. Da bi provjerili koja je trenutna verzija učitanog kernela, možete koristiti komandu `uname -r`. Poređenjem dobijenog broja verzije sa verzijom koja se nalazi u nazivu direktorijuma izvornog koda (koji se nalazi u `/usr/src/` direktorijumu), možemo da utvrdimo da li su verzije usklađene. Ako to nije slučaj, moramo da pokrenemo komandu `sudo apt-get upgrade` da bi preuzeli posljednju verziju kernela.

Nakon što smo preuzeli fajlove izvornog koda kernela, možemo pristupiti kompajliranju slLIN modula. Prvo je potrebno preći u direktorijum `/linux-lin/sllin/`, a zatim pokrenuti proces kompajliranja komandom:

```
make
```

**Napomena:** Prilikom prevođenja, moguće je da se pojavi greška zbog neusklađenosti verzije učitanog i verzije fajlova izvornog koda kernela, što se može prevazići pokretanjem komande `sudo apt-get upgrade`, kako je prethodno opisano. Međutim, ako je instalirana novija verzija kernela (>= 4.11.9), kompajler će prijaviti greške zbog toga što su u novijim verzijama kernela, izmijenjene određene strukture podataka. U tom slučaju, nephodno je da se instalira zakrpa `recent-kernel-workaround.patch` koja može da se pronađe u direktorijumu `./patches/`. U tu svrhu, koristi se *Linux* komanda `patch`, na način kako je opisano u članku [7 Patch Command Examples to Apply Diff Patch Files in Linux](https://www.thegeekstuff.com/2014/12/patch-command-examples/).

Nakon uspješnog prevođenja slLIN modula, u trenutnom direktorijumu bi trebalo da se nalazi fajl `sllin.ko`, koji se može učitati u sistem kao *kernel-space* modul. U tu svrhu, koristimo komandu 

```
sudo insmod ./sllin.ko baudrate=9600
```

gdje se parametrom `baudrate` definiše bitska brzina LIN komunikacije. Ovom modulu se mogu proslijediti i neki drugi parametri. Detaljnije informacije, student može pronaći u dokumentaciji za slLIN implementacije.

Kada je slLIN modul učitan, potrebno je da se poveže sa fizičkim UART interfejsom, što se (u slučaju *RAspberry Pi* platforme) postiže komandom:

```
sudo ldattach 25 /dev/ttyAMA0
```

Nakon toga, slLIN modul se od strane *Linux* operativnog sistema vidi kao standardni mrežni interfejs, što znači da se može kontrolisati i podešavati standardnim komandama. Tako, na primjer, ako želimo da omogućimo, pregledamo statusne i druge informacije ili onemogućimo interefejs, koristimo sljedeće komande:

```
sudo ip link set sllin0 up      # enable interface
ip link show dev sllin0         # print info
sudo ip link set sllin0 down    # disable interface
```

Da bi dalje mogli da koristimo ovaj interfejs, potrebno je da se on omogući korišćenjem komande `sudo ip link set sllin0 up`.

## Zadatak 2: Instalacija *SocketCAN* aplikacija ##
U osnovi, slLIN modul predstavlja *Linux* drajver koji omogućava korisničkim aplikacijama u okviru [*Linux-CAN / SocketCAN*](https://github.com/linux-can/can-utils) projekta da šalju i primaju LIN okvire preko UART interfejsa. Suštinski gledano, ovaj modul se ponaša kao CAN-LIN *gateway*, pri čemu se CAN okviri koje generišu *SocketCAN* aplikacije transliraju prema predefinisanim pravilima u LIN okvire i obrnuto, LIN okviri primljeni preko UART interfejsa, prevode se u CAN okvire koji se dalje mogu logovati i interpretirati odgovarajućim *SocketCAN* aplikacijama. Iz navedenog razloga, da bi se omogućila LIN komunikacija, neophodno je da se instaliraju *SocketCAN* aplikacije.

Postoje dva pristupa pri instalaciji. Prvi način podrazumijeva jednostavnu instalaciju binarnog paketa pomoću komande

```
sudo apt-get install can-utils
```

jer je ovaj projekat dio glavne linije *Linux* kernela.

Međutim, ovaj način ne nudi fleksibilnost u smislu eventualnih modifikacija samog *SocketCAN* softvera. Da bi to bilo moguće, instalaciju treba uraditi ručno, tj. preuzimanjem i kompajliranjem izvornog koda. Ovdje ćemo koristiti drugi način instalacije, jer će biti neophodno napraviti određene modifikacija izvornog koda kako bi se on prilagodio slLIN implementaciji.

Prvo je potrebno preuzeti kompletan projekat sa repozitorijuma korišćenjem komande:

```
git clone https://github.com/linux-can/can-utils.git
```

Nakon toga, potrebno je instalirati zakrpu `slcan_attach-sllin-workaround.patch` (iz direktorijuma `./patches`) koja omogućava integraciju *SocketCAN* i slLIN implementacije.

Konačno, projekat se može kompajlirati sljedećom sekvencom komandi:

```
cd can-utils
./autogen.sh
./configure
make
sudo make install
```

Kada je *SocketCAN* projekat instaliran, potrebno je testirati funkcionalnost čitavog sistema. Za testiranje, mogu se koristiti alatke `cangen` i `candump`. Više o opcijama koje ove alatke nude, student može da pročita u okviru dokumentacije koja se može naći na web stranici [*Linux-CAN / SocketCAN*](https://github.com/linux-can/can-utils). Pravila po kojima se CAN okviri transliraju u LIN okvire, student može da pronađe u dokumentu *Report describing proposal of LIN to SocketCAN integration* koji se može preuzeti sa web stranice [slLIN - LIN Driver for Standard UART/TTY Interfaces](https://rtime.felk.cvut.cz/can/lin-bus/). Da bi kompletirali zadatak, potrebno je da testirate scenarije opisane i demonstrirane u ovom dokumentu. Rad sistema obavezno verifikovati posmatranjem talasnih oblika na osciloskopu koji je povezan sa UART interfejsom i njihovim poređenjem sa vrijednostima dobijenim u okviru `candump` alata.

## Zadatak 3: Konfiguracija LIN mreže ##

# instalacija libxml2-dev paketa
sudo apt-get -y install libxml2-dev

# instalacija libnl-route-3-dev paketa
sudo apt-get -y install libnl-route-3-dev

# PCAN-LIN konfiguracioni softver
https://www.peak-system.com/fileadmin/media/files/pcanlin.zip

# ELMOS LIN Frames

# Start Measurement frame
[0x00] 0x20 0x00 0xc8

# Read Measured Data frame
[0x03]
