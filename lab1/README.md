# Laboratorijska vježba 1: Kroskompajliranje i prenos na ciljnu platformu
## Priprema za vježbu
Za realizaciju vježbe, neophodno je pokrenuti razvojnu platformu zasnovanu na *Ubuntu 16.04 LTS* operativnom sistemu, koja je preinstalirana i izvršava se kao virtuelna mašina pod nazivom *rpi-linux-dev* u okviru *VMWare Workstation Player* softvera za virtuelizaciju.

Nakon prijavljivanja na sistem, prvi korak je kloniranje udaljenog repozitorijuma u kojem se nalaze svi fajlovi neophodni za izradu laboratorijske vježbe (u daljem tekstu se pretpostavlja da se korisnik nalazi u `/home/student`). U tu svrhu, u terminalu je neophodno pokrenuti sljedeću komandu:

```
git clone https://github.com/knezicm/ikm-labs/
```

Kao efekat pomenute komande, u okviru trenutnog direktorijuma (`/home/student`) će se formirati direktorijum `ikm-labs` i u njemu će se kreirati struktura udaljenog repozitorijuma. Svi fajlovi neophodni za realizaciju vježbe se nalaze u okviru foldera čija je putanja `~/ikm-labs/lab1`.

**Napomena:** Kloniranje udaljenog repozitorijuma na navedeni način potrebno je uraditi samo pri realizaciji prve vježbe. U narednim vježbama, repozitorijum je potrebno samo ažurirati korišćenjem komande `git pull` (uz eventualno odbacivanje prethodno napravljenih lokalnih izmjena, što će biti pojašnjeno kasnije).

## Zadatak 1: Kroskompajliranje
U ovom zadatku student treba da se upozna sa procedurama koje su neophodne za podešavanje razvojne platforme (*Ubuntu 16.04 LTS* virtuelna mašina na PC računaru), kako bi se omogućilo kroskompajliranje za ciljnu platformu (u našem slučaju, to je *Raspberry Pi*).

Prvi korak podrazumijeva prelazak u direktorijum `lab1`. Nakon toga, student treba da se upozna sa strukturom ovog direktorijuma, pronađe izvorni fajl koji je potrebno kroskompajlirati i identifikovati dodatne fajlove koji definišu proces kroskompajliranja na razvojnoj platformi.

Nakon što se student upozna sa strukturom direktorijuma i identifikuje segmente koji su ključni za proces kroskompajliranja, može se pristupiti prevođenju izvornog fajla u izvršni oblik koji je pogodan za izvršavanje na ciljnoj platformi. U tu svrhu, u trenutnom direktorijumu (`~/ikm-labs/lab1`), potrebno je pokenuti sljedeću sekvencu komandi:

```
cd lab1-1
cmake . -DCMAKE_TOOLCHAIN_FILE=Toolchain-rpi.cmake
make
```
Ukoliko cmake komanda nije pronađena, potrebno je prethodno instalirati komandom:
```
sudo apt install cmake
```

Ukoliko se prilikom procesa prevođenja ne pojave nikakve greške i upozorenja, kao rezultat bi trebalo da se trenutnom direktorijumu pojavi izvršni fajl pod nazivom `hello-world`. Ako pokušate da pokrenete izvršavanje ovog fajla na razvojnoj platformi, trebalo bi da dobijete informaciju o greški, jer fajl nije kompatibilan za izvršavanje na trenutnoj platformi.

Da biste mogli da izvršite fajl, potrebno je prvo da ga prenesete na ciljnu platformu. To se može postići korišćenjem `scp` programa. Ovaj program omogućava siguran prenos fajlova sa lokalnog na udaljeni računar, na sličan način kao što se koristi `cp` program. Opšta sintaksa za korišćenje komande ima sljedeći oblik:

```
scp /path/to/local/source/file username@ip_address:/path/to/remote/destination/file
```

U konkretnom slučaju, s obzirom da je podrazumijevano korisničko ime *pi* i da je IP adresa *Raspberry Pi* platforme podešena na `192.168.23.205`, potrebno je koristiti sljedeću komandu:

```
scp /home/student/ikm-labs/lab1/lab1-1/hello-world pi@192.168.23.205:/home/pi/student-name/lab1
```

**Napomena:** Prije pokretanja prethodne komande, student treba da napravi direktorijum (korišćenjem `mkdir` komande) sa svojim imenom (npr. `milan-milanovic`) i prezimenom na ciljnoj platformi u okviru `/home/pi` direktorijuma. Takođe, u okviru ovog direktorijuma, treba da se nalazi direktorijum `lab1`. Da bi ovo mogli uraditi na ciljnoj platformi, potrebno je da se daljinski povežemo sa ciljnom platformom. Ovo se može postići sa razvojne platforme pomoću `ssh` programa. Sintaksa ove komande je slična sintaksi `scp` programa, tj. potrebno je definisati korisničko ime i IP adresu ciljne platforme. Razlika je u tome što se ne specificiraju nazivi fajlova, jer se ne obavlja nikakvo kopiranje. Potrebno je unijeti sljedeću sekvencu komandi:

```
ssh pi@192.168.23.205
/// unijeti korisničku šifru ///
mkdir student-name
cd student-name
mkdir lab1
```
hello-world
Da bi prekinuli vezu sa ciljnom platformom i pokrenuli prethodno navedenu komandu `scp`, dovoljno je unijeti komandu:

```
exit
```

Kada je izvršni fajl prenesen na ciljnu platformu, može se pokrenuti njegovo izvršavanje. Potrebno je ponovo se poveyati na ciljnu platformu i unijeti sljedeću sekvencu komandi

```
ssh pi@192.168.23.205
/// unijeti korisničku šifru /////
cd ./student-name/lab1
./hello-world
```

gdje `student-name` označava folder sa imenom studenta na ciljnoj platformi.

Ako je sve urađeno kako treba, kao rezultat izvršavanja programa, u terminalu bi trebalo da se pojavi poruka "Hello, World!".

## Zadatak 2: Kroskompajliranje sa uključenom bibliotekom
U prethodnom zadatku, pokazali smo kako se može kroskompajlirati izvorni kod na razvojnoj PC platformi, tako da generisani izvršni fajl može da se izvrši na ciljnoj *Raspberry Pi* platformi. Korišćen je jednostavan primjer bez spoljnih biblioteka koje je potrebno dinamički povezati sa izvršnim fajlom.

Kada se koristi neka spoljnja biblioteka i kada se koristi dinamičko povezivanje, potrebno je napraviti dodatne modifikacije u konfiguraciji *CMake* alata za automatizovano prevođenje izvornog koda. U suštini, s korisničke strane, postupak kroskompajiliranja je u potpunosti transparentan i potpuno identičan kao u prethodnom zadatku. Razlika je u konfiguracionom fajlu (`CMakeLists.txt`) i u tome što spoljnja biblioteka mora da bude prekompajlirana i na razvojnoj (korišćenjem postupka kroskompajliranja) i na ciljnoj (korišćenjem standardnog kompajliranja) platformi. U cilju pojednostavljenja izrade vježbe, ove aktivnosti su već urađene na obje platforme, tako da student samo treba da kroskompajlira glavni izvorni kod, kao što je opisano u prethodnom zadatku.

U konkretnom primjeru, korišćena je [*wiringPi*](http://wiringpi.com/) kao spoljnja, dinamički povezana biblioteka. Ova biblioteka omogućava pristup hardverskim resursima na *Raspberry Pi* platformi i biće intenzivno korišćena u narednim laboratorijskim vježbama. U tom smislu, preporuka je da se student upozna sa funkcijama za rad sa GPIO (engl. *General Purpose Input Output*) sistemom, kao i periferijskim interfejsima (UART, SPI, I2C).

Prvo je potrebno obezbijediti da se student nalazi u direktorijumu `lab1-2`, u kojem se nalaze svi fajlovi neophodni za realizaciju zadatka. Kao i u prethodnom zadatku, potrebno je da se student upozna sa strukturom direktorijuma, izvornim kodom (fajl `blinking.c` u `./src` direktorijumu), kao i sa konfiguracionim fajlom *CMake* alata.

U ovom zadatku je dat primjer manipulacije jednim GPIO pinom koji je konfigurisan kao izlaz. Program periodično, nakon isteka intervala od pola sekunde, invertuje stanje izlaznog pina GPIO0. Ako na ovaj pin povežemo LED diodu, aktivnost se manifestuje "žmiganjem" (engl. *blinking*). Fizička lokacija pina na *Raspberry Pi* platformi se može pronaći pomoću [*Pinout*](https://pinout.xyz/) alata. U konkretnom slučaju, GPIO0 pin se nalazi fizičkom pinu 11 (BCM 17).

Nakon kroskompajliranja i prenosa dobijenog izvršnog fajla (pod nazivom `blinking`) na ciljnu platformu u folder `/home/pi/student-name/lab1`, kao što je opisano u prethodnom zadatku, trebalo bi da se dobije efekat "žmiganja" na povezanoj LED diodi.
