# Industrijske komunikacione mreže (2322)
U okviru ovog repozitorijuma, studenti mogu da pronađu sve materijale nephodne za realizaciju laboratorijskih vježbi iz predmeta Industrijske komunikacione mreže (2322). Laboratorijske vježbe su bazirane na *Raspberry Pi* platformi i *Linux* operativnom sistemu. Za razvoj i prevođenje programa koji se izvršavaju na *Raspberry Pi* platformi, studentima je na raspolaganju specifično kreirana virtuelna mašina sa *Ubuntu 16.04 LTS* operativnim sistemom (*rpi-linux-dev*), koja se pokreće u okviru *VMWare Workstation Player* softvera za virtuelizaciju.

Ukoliko koristite lični računar i *VMWare Workstation Player* softvera za virtuelizaciju, preporuka je da koristite preinstaliran *Ubuntu 16.04 LTS* operativni sistem, koji možete da preuzmete sa sljedećeg linka:

https://www.osboxes.org/ubuntu/#ubuntu-16-04-vmware

Za ovu virtuelnu mašinu, podrazumijevani korisnik sa administratorskim privilegijama je `osboxes.org`, pri čemu je podrazumijevana šifra ista kao korisničko ime.

## Završni ispit ##
Završni ispit čini 40% ukupne ocjene i polaže se iz dva dijela:

- [eliminatorna ispitna pitanja](./exam-questions.md) (maksimalno 10 bodova)
- usmeni dio ispita (maksimalno 30 bodova)

Da bi student zadovoljio na završnom ispitu, potrebno je da ostvari najmanje 8 bodova u dijelu koji se odnosi na eliminatorna pitanja i 50% na usmenom dijelu ispita.

## Fond radnih sati
Za realizaciju ovog kursa, predviđen je sedmični fond od 2 časa predavanja i 2 časa vježbi. Vježbe dominantno čini rad u laboratoriji, s tim da određeni broj časova može biti realizovan kroz klasične auditorne, odnosno pokazne laboratorijske vježbe. Važno je napomenuti da kurs nosi 5 ECTS kredita, što efektivno znači da je za njegovu realizaciju predviđeno 100 radnih sati tokom semestra. S obzirom da je za direktnu nastavu predviđeno 60 sati, preostalih 40 sati (otprilike 2.67 sati sedmično) čini samostalan rad studenta.

## Osnovna pravila
Za uspješnu i efikasnu realizaciju laboratorijskih vježbi, preporučuje se da studenti poštuju sljedeća osnovna pravila.

* Ako se student ne suoči sa problemom i ne pokuša da ga samostalno riješi na kreativan način, teško se može očekivati da će postići otklon u smislu unapređenja znanja iz određene oblasti. Stoga, studenti trebaju da očekuju minimalan upliv predmetnog nastavnika i asistenta pri realizaciji laboratorijskih vježbi. Očekuje se da student bude pripremljen za rad u laboratoriji i da pristupi izradi vježbe sa minimalno potrebnim teorijskim znanjem iz predmetne teme.
* Zadatak predviđen laboratorijskom vježbom student treba da uradi samostalno. Konsultacije i razmjena informacija sa drugim studentima je dozvoljena i poželjna ukoliko za cilj ima da se definisani problem sagleda sa više strana i na taj način razvije zdrava diskusija na datu temu u duhu timskog rada pri rješavanju zajedničkog problema.
* Radno mjesto treba da se ostavi u stanju u kojem je zatečeno. To podrazumijeva da sve komponente koje se koriste, trebaju da budu uredno složene i vraćene na mjesto sa kojeg su preuzete. Računare i ostalu mjernu opremu treba propisno isključiti, a mrežne kablove koji su eventualno korišćeni za specifične potrebe, vratiti u stanje prije početka laboratorijske vježbe.
* Iako je za vježbe predviđen rad u okviru dva 45-minutna školska časa, dozvoljeno je da student sam planira svoje radno vrijeme i da pravi više kraćih pauza ukoliko to pozitivno utiče na efikasnost rada.
* Student može, na lični zahtjev i uz odobrenje predmetnog nastavnika ili asistenta, da pozajmi opremu koja se koristi za realizaciju laboratorijskih vježbi, uz uslov da se pozajmica uredno evidentira i da se do početka sljedeće vježbe (ili, po potrebi, ranije) oprema vrati u laboratoriju.

## Dostupna oprema
- PC razvojna platforma sa *Ubuntu 16.04 LTS* virtuelnom mašinom
- [*Raspberry Pi*](https://www.raspberrypi.org/) ciljna platforma sa preinstaliranim *Raspbian* operativnim sistemom
- Adapter za napajanje
- Protobord za realizaciju električnih šema
- Dodatne elektronske komponente i žice za njihovo povezivanje na protobordu
- Osciloskop

## Priprema za realizaciju laboratorijskih vježbi

Ukoliko koristite vlastiti računar, bilo da se radi o virtuelnoj mašini ili o stvarnoj instalaciji *Linux* operativnog sistema, potrebno je da pripremite razvojnu platformu u skladu sa instrukcijama datim ispod. Pretpostavićemo da se razvoj koristi PC računar sa *Ubuntu 16.04 LTS* operativnim sistemom. Novije verzije *Ubuntu* distribucije bi trebalo da rade bez problema. Međutim, napominjemo da su instrukcije date ispod testirane samo na pomenutoj verziji. Druge distribucije će možda zahtijevati dodatna prilagođenja.

*Instalacija neophodnih alata*

Za realizaciju laboratorijskih vježbi, potrebno je da imate instalirane sljedeće alate:

* GCC prevodilac (kompajler)
* Git softver za verzionisanje koda

Distribucije *Ubuntu* operativnog sistema se obično isporučuju sa GCC kompajlerom. Da biste utvrdili da li je ovaj kompajler instaliran na razvojnoj platformi, možete pokrenuti sljedeće komandu u terminalu:

```
gcc --version
```

Ako je kompajler instaliran, trebalo bi da dobijete ispis sličan ovome:

```
gcc (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609
Copyright (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

u suprotnom ćete dobiti informaciju o tome da kompajler nije instaliran

Ako GCC kompajler, iz bilo kojeg razloga, nije uključen u distribuciju koju koristite, možete da ga instalirate sljedećom komandom (važi samo za distribucije bazirane na *Debian* operativnom sistemu, poput *Ubuntu*):

```
sudo apt-get install build-essential
```

Git softver ćemo primarno koristiti za preuzimanje izvornog koda sa repozitorijuma kursa, a povremeno i za predaju zadataka koji se rade u okviru laboratorijskih vježbi. Na sličan način, kao i u slučaju GCC kompajlera, prvo ćemo provjeriti da li je ovaj softver već instaliran na distribuciji koju koristimo:

```
git --version
```

Ukoliko kao izlaz dobijemo

```
The program 'git' is currently not installed. You can install it by typing:
sudo apt install git
```

to znači da Git softver nije instaliran. Možemo ga instalirati komandom:

```
sudo apt-get install git
```

nakon čega, ako pokrenemo `git --version`, dobijamo ispis sličan datom ispod:

```
git version 2.7.4
```

**Napomena:** Prije instalacije bilo kojeg softverskog paketa na *Ubuntu* operativnom sistemu, dobra praksa je da se pokrene ažuriranje lokalne baze podataka u kojoj se čuvaju informacije o najnovijim verzijama softverskih paketa. To možete učiniti komandom `sudo apt-get update`.

Ukoliko prilikom pokretanja instalacije Git softvera dobijete sljedeću poruku

```
E: Could not get lock /var/lib/dpkg/lock-frontend – open (11: Resource temporarily unavailable)
E: Unable to acquire the dpkg frontend lock (/var/lib/dpkg/lock-frontend), is another process using it?
```

onda to znači da operativni sistem instalira posljednja ažuriranja, pa ćete morati sačekati nekoliko minuta prije ponovnog pokretanja instalacije.

Uz prethodno pomenute alate, da bi razvojna platforma bila spremna, potrebno je još obezbijediti i kros-kompajler za ciljnu platformu (*Raspberry Pi* u našem slučaju).

### Priprema kros-kompajlera za Raspberry Pi platformu

Kros-kompajler je kompajler koji kreira izvršni fajl koji će se izvršavati na platformi različitoj od one na kojoj pokrećemo proces prevođenja (npr. kompajler koji se izvršava na x86 arhitekturi generiše binarni fajl koji će se izvršavati na ARM arhitekturi). Direktno prevođenje na ciljnoj platformi često nije pogodno, jer one tipično ne nude komforno radno okruženje kao što je slučaj sa PC računarom, a često su i ograničene sa resursima. Kao posljedicu imamo da, čak i ako postoji mogućnost direktnog prevođenja, to prevođenje značajno duže traje, a imamo i ograničene mogućnosti za debagovanje. U određenim slučajevima, direktno prevođenje je čak i neizvodivo, npr. na mikrokontroleru ugrađenog sistema, ukoliko ti sistemi ne sadrže operativni sistem.

U okviru laboratorijskih vježbi ovog kursa, koristićemo kros-kompajler za prevođenje izvornog koda koji treba da se izvršava na ARM arhitekturi (*RaspberryPi* platforma). S obzirom da, kao razvojnu platformu, koristimo PC računar sa *Ubuntu* operativnim sistemom, prvo je potrebno preuzeti alate neophodne za kros-kompajliranje.

Prije svega, kreiraćemo direktorijum u `home` direktorijumu razvojne mašine, sa nazivom `rpi` u kojem ćemo smještati sve alate i biblioteke neophodne za rad sa *Raspberry Pi* platformom:

```
mkdir rpi
cd rpi
```

Kada se nalazimo u `rpi` direktorijumu, sljedećom komandom možemo preuzeti skup alata za kros-kompajliranje za ciljnu platformu:

```
git clone --depth 1 https://github.com/raspberrypi/tools
```

Da bi kros-kompajler bio globalno vidljiv prilikom svake sesije terminala, potrebno je da eksportujemo putanju do njega u okviru `PATH` sistemske varijable:

```
export PATH=$PATH:/path/to/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin
```

pri čemu je potrebno naglasiti da `/path/to/tools/` trebate zamijeniti apsolutnom putanjom do lokacije na koju ste preuzeli skup alata za kros-kompajliranje (ako se trenutno nalazite na toj lokaciji, absolutnu putanju možete dobiti komandom `pwd`). Tako, na primjer, ako koristite virtuelnu mašinu dostupnu u laboratoriji (i ako ste doslovno pratili instrukcije date iznad), onda će komanda izgledati kao što je prikazano ispod.

```
export PATH=$PATH:/home/student/rpi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin
```

U slučaju virtuelne mašine koju koristite na vlastitom računaru, lokacija alata, kao i nazivi `home` direktorijuma, mogu da se razlikuju. Važno je napomenuti da je ovaj korak veoma bitan, jer bez njega kros-kompajler neće biti globalno vidljiv unutar terminala.

Da biste aktuelizovali promjene u trenutnoj sesiji, potrebno je da pokrenete sljedeću komandu:

```
. ~/.bashrc
```

**Napomena:** Prethodnu komandu je potrebno pokrenuti samo jednom, i to ako ne želite da zatvorite i ponovo pokrećete trenutnu instancu terminala. Kada ponovo pokrenete terminal, putanja će biti automatski aktuelizovana.

Konačno, da biste potvrdili da imate pristup kros-kompajleru iz terminala, tj. da je sve što je prethodno opisano napravljeno kako treba, možete pokrenuti sljedeću komandu:

```
arm-linux-gnueabihf-gcc --version
```

koja, kao rezultat, treba da ispiše sljedeći izlaz:

```
arm-linux-gnueabihf-gcc (crosstool-NG crosstool-ng-1.22.0-88-g8460611) 4.9.3
Copyright (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## Dodatna literatura
TBA
