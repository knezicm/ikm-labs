# Laboratorijska vježba 2: Serijski interfejsi bazirani na UART prenosu #

## Priprema za vježbu ##
Osnovni cilj vježbe je da se student upozna sa serijskim interejsima koji koriste UART uređaj za prenos podataka. U vježbi se na fizičkom sloju koristi transiver MAX485, koji je neophodan za generisanje diferencijalnih signala na zajedničkoj magistrali u skladu sa RS-485 specifikacijom. S tim u vezi, student treba da bude upoznat sa RS-485 standardom, kao i sa specifikacijom MAX485 komponente, posebno u dijelu koji se odnosi na granične naponske nivoe i strujne kapacitete na njenim pinovima.

Prije početka vježbe, student treba da ažurira stanje lokalnog repozitorijuma izvršavanjem `git pull` komande u okviru `~/ikm-labs/` direktorijuma.

**Napomena:** Ukoliko postoje izmjene u lokalnom repozitorijuma koje onemogućavaju povlačenje nove verzije sa udaljenog repozitorijuma, potrebno je resetovati lokalni repozitorijum korišćenjem sljedeće komande:

```
git reset --hard
```

## Zadatak 1: Osnovni rad sa serijskim portom ##
U okviru provig zadatka je potrebno da se student upozna sa osnovama komunikacije putem serijskog porta u okviru *Linux* operativnog sistema. Važno je napomenuti da ćemo u okviru ove vježbe koristiti hardversku UART jedinicu *Raspberry Pi* platforme, čiji su prijemni (RX) i predajni (TX) pinovi dostupni na GPIO konektoru (njihovu poziciju možete pronaći u okviru [Raspberry Pi Pinout](https://pinout.xyz/) servisa).

U okviru operativnog sistema, ovom modulu se može pristupati preko sistemskog fajla `/dev/ttyAMA0`. *Raspbian* operativni sistem podrazumijevano rezerviše ovaj hardverski UART modul za sistemsku konzolu preko koje se može pristupati resursima operativnog sistema u cilju debagovanja sistema. Ovo onemogućava korišćenje serijskog porta za potrebe korisnika. Prema tome, prvo je potrebno provjeriti da li je port već zauzet (odnosno, da li je sistemska konzola omogućena na ovom portu) i, ako jeste, u podešavanjima operativnog sistema onemogućiti je kako bi serijski port bio dostupan.

Status sistemske konzole na hardverskom UART modulu provjeravamo komandom:

```
dmesg | grep tty
```

**Napomena:** Prethodnu komandu je potrebno pokrenuti na ciljnoj platformi (*Raspberry Pi*).

Ova komandnda prikazuje sve terminalne uređaje (uključujući i hardverski UART modul) registrovane u operativnom sistemu, kao i njihovo stanje po pitanju sistemske konzole. Potrebno je potražiti liniju u kojoj piše da je konzola omogućena (*console enabled* na terminalnom uređaju `/dev/ttyAMA0`). Ako ove linije nema, to znači da konzola nije omogućena na ovom modulu i nije potrebno modifikovati konfiguraciju. Ako linija postoji, potrebno je onemogućiti konzolu u okviru sistemskih podešavanja kojima se pristupa komandom:

```
sudo raspi-config
```

Ova komanda prikazuje korisnički interfejs sa ponuđenim različitim opcijama. U ovom slučaju, potrebno je odabrati opciju *Interfacing Options*, a zatim *P6 Serial*. U okviru ponuđenog pitanja, potrebno je odabrati opciju za onemogućenje (*Disable*) konzole na serijskom portu. Konačno, podešavanja treba sačuvati i izaći iz konfiguracionog menija (opcija *Finish*). Pri izlasku, operativni sistem će zahtjevati restartovanje, što treba potvrditi. Nakon restartovanja platforme, na prethodno opisani način, može se potvrditi da sistemska konzola više nije omogućena na hardverskom UART modulu.

