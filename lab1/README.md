# Laboratorijska vježba 1: Kroskompajliranje i prenos na ciljnu platformu
## Priprema za vježbu
Za realizaciju vježbe, neophodno je pokrenuti razvojnu platformu zasnovanu na *Ubuntu 16.04 LTS* operativnom sistemu, koja je preinstalirana i izvršava se kao virtuelna mašina pod nazivom *rpi-linux-dev* u okviru *VMWare Workstation Player* softvera za virtuelizaciju.

Nakon prijavljivanja na sistem, prvi korak je kloniranje udaljenog repozitorijuma u kojem se nalaze svi fajlovi neophodni za izradu laboratorijske vježbe (u daljem tekstu se pretpostavlja da se korisnik nalazi u `/home/student`). U tu svrhu, u terminalu je neophodno pokrenuti sljedeću komandu:

```
git clone https://github.com/knezicm/ikm-labs/
```

Kao efekat pomenute komande, u okviru trenutnog direktorijuma (`/home/student`) će se formirati direktorijum `ikm-labs` i u njemu će se kreirati struktura udaljenog repozitorijuma. Svi fajlovi neophodni za realizaciju vježbe se nalaze u okviru foldera čija je putanja `~/ikm-labs/lab1`. Ovdje je važno napomenuti da je kloniranje udaljenog repozitorijuma na navedeni način potrebno uraditi samo pri realizaciji prve vježbe. U narednim vježbama, repozitorijum je potrebno samo ažurirati korišćenjem komande `git pull`.

## Zadatak 1: Kroskompajliranje
U ovom zadatku student treba da se upozna sa procedurama koje su neophodne za podešavanje razvojne platforme (*Ubuntu 16.04 LTS* virtuelna mašina na PC računaru), kako bi se omogućilo kroskompajliranje za ciljnu platformu (u našem slučaju, to je *Raspberry Pi*).

Prvi korak podrazumijeva prelazak u direktorijum `lab1`. Nakon toga, student treba da se upozna sa strukturom ovog direktorijuma, pronađe izvorni fajl koji je potrebno kroskompajlirati i identifikovati dodatne fajlove koji definišu proces kroskompajliranja na razvojnoj platformi.

Nakon što se student upozna sa strukturom foldera i identifikuje segmente koji su ključni za proces kroskompajliranja, može se pristupiti prevođenju izvornog fajla u izvršni oblik koji je pogodan za izvršavanje na ciljnoj platformi. U tu svrhu, u trenutnom folder (`~/ikm-labs/lab1`), potrebno je pokenuti sljedeću sekvencu komandi:

```
cmake . -DCMAKE-TOOLCHAIN-FILE=Toolchain-rpi.cmake
make
```

Ukoliko se prilikom procesa prevođenja ne pojave nikakve greške i upozorenja, kao rezultat bi trebalo da se trenutnom folderu generiše izvršni fajl pod nazivom `hello-world`. Ukoliko pokušate da pokrenete ovaj fajl na razvojnoj platformi, trebalo bi da dobijete informaciju o greški, jer fajl nije kompatibilan za izvršavanje na trenutnoj platformi.

Da biste mogli da izvršite fajl, potrebno je prvo da ga prenesete na ciljnu platformu. To se može postići korišćenjem `scp` programa. Ovaj program omogućava siguran prenos fajlova sa lokalnog na udaljeni računar, na sličan način kao što se koristi `cp` program.
