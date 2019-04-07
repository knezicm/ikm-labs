# Laboratorijska vježba 2: Serijski interfejsi bazirani na UART interfejsu #

## Ciljevi i ishodi ##
Osnovni cilj vježbe je da se student upozna sa osnovnim parametrima UART interfejsa i serijskog porta, kao i da nauči da interpretira talasne oblike na osciloskopu sa serijskog UART interfejsa.

Po završetku vježbe, student će biti sposoban da:

- podesi parametre UART interfejsa u okviru Linux konzole,
- šalje podatke na UART interfejs iz Linux konzole,
- prikazuje podatke primljene sa UART interfejsa u Linux konzoli,
- interpretira talasne oblike na osciloskopu dobijene sa UART interfejsa.

## Priprema za vježbu ##
Očekuje se da je student upoznat sa teorijskim osnovama i principom rada UART interefejsa (kroz prezentacije na predavanjima i konsultovanje dostupne literature), koji predstavlja osnovu serijskog porta u računarskim sistemima.

Prije početka vježbe, student treba da ažurira stanje lokalnog repozitorijuma izvršavanjem `git pull` komande u okviru `~/ikm-labs/` direktorijuma. Ako repozitorijum nije ranije preuzet, potrebno ga je klonirati u lokalnom `home` direktorijumu korišćenjem naredbe `git clone https://github.com/knezicm/ikm-labs`. Nakon što je repozitorijum ažuriran/kloniran, potrebno je kopirati folder `labs` sa cijelim  njegovim sadržajem u `home` direktorijum trenutnog korisnika.

## Aktiviranje UART interfejsa na ciljnoj platformi ##
*Raspberry Pi* platforma posjeduje hardverski UART interfejs čiji se prijemni (RX) i predajni (TX) pinovi nalaze na GPIO konektoru na pozicijama BCM14 i BCM15, respektivno, kao što može da se vidi na slici (izvor: [Raspberry Pi Pinout](https://pinout.xyz/)).

![RX i TX pinovi](./imgs/uart-pins.png)

U okviru Linux operativnog sistema, ovom interfejsu se može pristupati preko virtuelnog fajla `/dev/ttyAMA0`. *Raspbian* operativni sistem podrazumijevano rezerviše ovaj hardverski UART modul za sistemsku konzolu preko koje se može pristupati resursima operativnog sistema u cilju debagovanja. Ovo onemogućava korišćenje UART interfejsa za potrebe korisnika. Prema tome, prvo je potrebno provjeriti da li je port već zauzet (odnosno, da li je sistemska konzola omogućena na ovom portu) i, ako jeste, u podešavanjima operativnog sistema onemogućiti je kako bi serijski port bio dostupan.

Status sistemske konzole na hardverskom UART modulu provjeravamo komandom:

```
dmesg | grep tty
```

**Napomena:** Prethodnu komandu je potrebno pokrenuti na ciljnoj platformi (*Raspberry Pi*), a ne na razvojnoj platformi (PC računar sa *Ubuntu* operativnim sistemom na virtuelnoj mašini).

Ova komanda prikazuje sve terminalne uređaje (`tty`) registrovane u operativnom sistemu (uključujući i hardverski UART interfejs), kao i njihovo stanje po pitanju sistemske konzole. Potrebno je potražiti liniju u kojoj piše da je konzola omogućena (*console enabled* na terminalnom uređaju `/dev/ttyAMA0`). Ako ove linije nema, to znači da konzola nije omogućena na ovom interfejsu i nije potrebno modifikovati konfiguraciju. Ako linija postoji, potrebno je onemogućiti konzolu u okviru sistemskih podešavanja kojima se pristupa komandom:

```
sudo raspi-config
```

Komanda pokreće konfiguracioni program sa ponuđenim različitim opcijama. U našem slučaju, potrebno je odabrati opciju *Interfacing Options*, a zatim *P6 Serial*. U okviru ponuđenih pitanja, potrebno je odabrati opciju za onemogućenje (*Disable*) konzole na serijskom portu i omogućenje (*Enable*) serijskog porta. Konačno, podešavanja treba sačuvati i izaći iz konfiguracionog menija (opcija *Finish*). Pri izlasku, operativni sistem će zahtjevati restartovanje, što treba potvrditi. Nakon restartovanja platforme, na prethodno opisani način, može se potvrditi da sistemska konzola više nije omogućena na hardverskom UART interfejsu.

**Napomena:** Ukoliko se kao ciljna platforma koristi *Raspberry Pi 3*, neophodno je onemogućiti *bluetooth* modul koji se podrazumijevano povezuje na `/dev/ttyAMA0` serijski port. U tu svrhu, potrebno je dodati sljedeću liniju na kraj fajla `/boot/config.txt`:

```
dtoverlay=pi3-disable-bt
```

## Podešavanje parametara serijskog porta ##
U Linux operativnom sistemu, trenutna podešavanja serijskog porta u okviru konzole, mogu se prikazati komandom:

```
stty -F /dev/ttyAMA0
```

Opcija `-F` definiše naziv virtuelnog fajla serijskog porta čija podešavanja želimo da prikažemo. Detaljnije informacije o komandi možemo da dobijemo komandom `man stty`.

Tako, na primjer, ako želimo da promijenimo bitsku brzinu u man stranici komande možemo da vidimo da je potrebno koristiti sljedeći format `stty` komande:

```
stty -F /dev/ttyAMA0 N
```
gdje je `N` broj koji definiše bitsku brzinu.

Ostale opcije u okviru komande, uglavnom se odnose na uključenje i isključenje različitih opcija kojima podešavamo parametre terminala, UART interfejsa ili serijskog porta. Opcija se uključuje navođenjem njenog imena, a isključuje navođenjem imena opcija ispred kojeg je potrebno staviti znak -.

Konačno, da bi poslali podatke na serijski port, najjednostavnije je koristiti *Linux* operator za redirekciju (`>`).

```
echo some_string > /dev/ttyAMA0
```
U komandi iznad, string `some_string` koji bi trebao da se prikaže na terminalu `echo` komandom, preusmjerava se na serijski port `/dev/ttyAMA0`. Važno je napomenuti da se string specificira unutar dvostrukih navodnika.

Da bi testirali ispravnost rada serijskog porta, potrebno je da se pošalje string koji sadrži nekoliko bajtova čija je vrijednost 0x55, a zatim ovu komunikaciju "uhvatiti" na osciloskopu (sondu osciloskopa vezati između TX pina i mase). Analizom segmenta komunikacije prikazanog na osciloskopu, potrebno je identifikovati *start*, *stop* i bite podataka, a zatim potvrditi da je poslan odgovarajući podatak sa definisanom bitskom brzinom.

## Zadatak za samostalnu izradu ##
