# Laboratorijska vježba 1: Kroskompajliranje i prenos na ciljnu platformu
## Priprema za vježbu
Za realizaciju vježbe, neophodno je pokrenuti razvojnu platformu zasnovanu na *Ubuntu 16.04 LTS* operativnom sistemu (ili ekvivalentnu konfiguraciju), koja je preinstalirana i izvršava se kao virtuelna mašina pod nazivom *rpi-linux-dev* u okviru *VMWare Workstation Player* softvera za virtuelizaciju. Ukoliko koristite sopstveni računar sa virtuelnom mašinom (ili sa direktnom instalacijom neke *Linux* distribucije), potrebno je da obezbijedite da su instalirani alati potrebni za realizaciju vježbe kako je opisano u [uvodu u laboratorijske vježbe](https://github.com/knezicm/ikm-labs).

Nakon prijavljivanja na sistem, prvi korak je kloniranje udaljenog repozitorijuma u kojem se nalaze svi fajlovi neophodni za izradu laboratorijske vježbe (u daljem tekstu se pretpostavlja da se korisnik nalazi u `/home/student` ili ekvivalentnom direktorijumu). U tu svrhu, u terminalu je neophodno pokrenuti sljedeću komandu:

```
git clone https://github.com/knezicm/ikm-labs/
```

Kao efekat pomenute komande, u okviru trenutnog direktorijuma će se formirati direktorijum `ikm-labs` i u njemu će se kreirati struktura udaljenog repozitorijuma. Svi fajlovi neophodni za realizaciju vježbe se nalaze u okviru foldera čija je putanja `~/ikm-labs/lab1`.

**Napomena:** Kloniranje udaljenog repozitorijuma na navedeni način potrebno je uraditi samo pri realizaciji prve vježbe. U narednim vježbama, repozitorijum je potrebno samo ažurirati korišćenjem komande `git pull` (uz eventualno odbacivanje prethodno napravljenih lokalnih izmjena, što će biti pojašnjeno kasnije).

## Zadatak 1: Kroskompajliranje
U ovom zadatku student treba da se upozna sa procedurama koje su neophodne za podešavanje razvojne platforme (*Ubuntu 16.04 LTS* virtuelna mašina na PC računaru), kako bi se omogućilo kroskompajliranje za ciljnu platformu (u našem slučaju, to je *Raspberry Pi*).

Prvi korak podrazumijeva prelazak u direktorijum `lab1`. Nakon toga, student treba da se upozna sa strukturom ovog direktorijuma, pronađe izvorni fajl koji je potrebno kroskompajlirati i identifikovati dodatne fajlove koji definišu proces kroskompajliranja na razvojnoj platformi.

Nakon što se student upozna sa strukturom direktorijuma i identifikuje segmente koji su ključni za proces kroskompajliranja, može se pristupiti prevođenju izvornog fajla u izvršni oblik koji je pogodan za izvršavanje na ciljnoj platformi. U tu svrhu, u trenutnom direktorijumu (`~/ikm-labs/lab1`), potrebno je pokenuti sljedeću sekvencu komandi:

```
cd lab1-1/src
arm-linux-gnueabihf-gcc hello-world.c -o hello-world
```

Ukoliko se prilikom procesa prevođenja ne pojave nikakve greške i upozorenja, kao rezultat bi trebalo da se u trenutnom direktorijumu pojavi izvršni fajl pod nazivom `hello-world`. Ako pokušate da pokrenete izvršavanje ovog fajla na razvojnoj platformi, trebalo bi da dobijete informaciju o greški, jer fajl nije kompatibilan za izvršavanje na trenutnoj platformi.

Da bi mogli da pokrećemo (i zaustavljamo) aplikacije na ciljnoj platformi, potrebno je da imamo mehanizam koji nam omogućava da pristupimo njenim resursima. S obzirom da se i na razvojnoj i na ciljnoj platformi koristi *Linux* operativni sistem, u tu svrhu je najlakše koristiti `ssh` program. Kontrolu *Raspberry Pi* platforme na ovaj način, ilustrovaćemo primjerom kreiranja direktorijuma na ciljnoj platformi. Naime, da bi bolje organizovali svoje fajlove na ciljnoj platformi, koju koristi više studenata, preporučuje se da napravite direktorijum (korišćenjem `mkdir` komande) sa svojim imenom i prezimenom (npr. `milan-milanovic`) u okviru `/home/pi` direktorijuma. Takođe, preporuka je da unutar ovog direktorijuma, za svaku vježbu kreirate zaseban direktorijum sa nazivom vježbe (npr. `lab1`).

Sintaksa `ssh` komande ima sljedeći izgled:

```
ssh username@ip_address
/// unijeti korisničku šifru ///
```

nakon čega možemo pristupiti konzoli kao da smo direktno povezani sa ciljnom platformom.

**Napomena:** Prilikom prvog prijavljivanja, `ssh` će zahtijevati da kompletirate proces autentifikacije udaljenog računara sa navedenim parametrima. U tom slučaju samo trebate da potvrdite autentifikaciju (opcija `yes`). Prilikom svakog narednog prijavljivanja, ovaj korak se neće zahtijevati.

U konkretnom slučaju, s obzirom da je podrazumijevano korisničko ime *pi*, uz pretpostavku da je IP adresa *Raspberry Pi* platforme podešena na `192.168.23.205`, potrebno je koristiti sljedeću varijantu komande:

```
ssh pi@192.168.23.205
/// unijeti korisničku šifru ///
mkdir milan-milanovic
cd milan-milanovic
mkdir lab1
```

pri čemu je podrazumijevana šifra korisnika *pi* podešena da bude *raspberry*.

**Napomena:** Prethodna sekvenca komandi omogućava povezivanje sa ciljnom platformom i kreiranje direktorijuma neophodnih za realizaciju ostatka vježbe. Navedeno ime (`milan-milanovic`) trebate zamijeniti sopstvenim.

Da bi prekinuli vezu sa ciljnom platformom, dovoljno je unijeti komandu:

```
exit
```

i vratićete se u terminal lokalnog računara.

U slučaju pristupa od kuće preko Interneta, potrebno je da tražite od predmetnog nastavnika pristupnu adresu (koju unosite umjesto IP adrese) i broj porta preko kojeg se pristupa *Raspberry Pi* platformi. U tom slučaju, komanda ima sljedeći oblik:

```
scp username@hostname -p port_number
```

pri čemu ćete `hostname` i `port_number` trebate zamijeniti sa onim koje ste dobili od predmetnog nastavnika.

Da biste mogli da izvršite prethodno dobijeni fajl, potrebno je prvo da ga prenesete na ciljnu platformu. To se može postići korišćenjem `scp` programa. Ovaj program omogućava siguran prenos fajlova sa lokalnog na udaljeni računar, na sličan način kao što se koristi `cp` program. Opšta sintaksa za korišćenje komande je slična sintaksi `ssh` komande, s tim da sadrži neke dodatne elemente.

```
scp /path/to/local/source/file username@ip_address:/path/to/remote/destination/file
```

U konkretnom slučaju, s obzirom da je podrazumijevano korisničko ime *pi* i da je IP adresa *Raspberry Pi* platforme podešena na `192.168.23.205`, potrebno je koristiti sljedeću komandu:

```
scp /home/student/ikm-labs/lab1/lab1-1/hello-world pi@192.168.23.205:/home/pi/milan-milanovic/lab1
```

pri čemu `milan-milanovic` trebate zamijeniti nazivom direktorijuma koji ste prethodno kreirali na ciljnoj platformi.

U slučaju pristupa od kuće preko Interneta, kao i u slučaju `ssh` komande, koristi se nešto drugačiji oblik komande:

```
scp -P port_number /home/student/ikm-labs/lab1/lab1-1/hello-world username@hostname:/home/pi/milan-milanovic/lab1
```

Kada je izvršni fajl prenesen na ciljnu platformu, može se pokrenuti njegovo izvršavanje. Potrebno je ponovo se povezati na ciljnu platformu i unijeti sljedeću sekvencu komandi:

```
ssh pi@192.168.23.205
/// unijeti korisničku šifru /////
cd ./milan-milanovic/lab1
./hello-world
```

Ako je sve urađeno kako treba, kao rezultat izvršavanja programa, u terminalu bi trebalo da se pojavi poruka "Hello, World!".

## Zadatak 2: Kroskompajliranje korišćenjem `make` alatke
Kao što smo vidjeli, prevođenje izvornog koda za ciljnu platformu se svodi na varijante komande `arm-linux-gnueabihf-gcc` kroskompajlera. Međutim, u slučaju kada imamo kompleksniji projekat sa više modula izvornog koda koji se uz linkuju sa dinamičkim bibliotekama, ovaj proces postaje zamoran i podložan greškama. Tako, na primjer, ako izmijenite nešto u jednom programskom modulu, potrebno je da ponovo prevedete sve programske module koje zavise od promjene koju ste napravili. Ove zavisnosti je veoma teško ispratiti čak i kod projekata srednje kompleksnosti koji imaju nekoliko desetaka programskih modula. Osim toga, sve komande se moraju unositi ručno, što definitivno "ubija" kreativnost svakog pojedinca. S obzirom da je riječ o ponovljivom procesu koji se vrlo lako može automatizovati, razvijeni su efikasniji pristupi koji koriste odgovarajuće alate za automatizaciju. Jedan od njih je `make` alat.

Na osnovu informacija o organizaciji i korišćenju `make` alata i kreiranja odgovarajućeg `Makefile` fajla, modifikovaćemo primjer iz prethodnog zadatka tako da se koristi pomoćna biblioteka `util` za štampanje stringa. Svi resursi ove biblioteke se nalaze u okviru direktorijuma `lab1-2/util`. Prvo trebate proučiti strukturu fajla `Makefile` kako bi naučili kako da je prilagodimo ako želimo da prevedemo dinamičku ili statičku biblioteku. Na osnovu stečenog znanja, prevedite biblioteku korišćenjem alatke `make` tako da dobijete statičku i dinamičku biblioteku. Binarne verzije biblioteka treba da se smjeste u direktorijum `lab1-2/util/lib`.

Dodajte novo pravilo u `Makefile` koje omogućava instalaciju (kopiranje) dinamičke biblioteke u odgovarajući direktorijum na ciljnoj *Raspberry Pi* platformi, a zatim iskoristite ovu novu funkcionalnost da instalirate prevedenu biblioteku na ciljnu platformu.

Prilagodite `Makefile` dat u folderu `lab1-2/hello` tako da se omogući prevođenje i linkovanje datog programa korišćenjem `make` alatke. Prevedite varijante programa sa statičkim i dinamičkim linkovanjem, a zatim ih prenesite na ciljnu platformu i testirajte njihovo izvršavanje.

## Zadatak 3: Kroskompajliranje sa uključenom bibliotekom
U prvom zadatku, pokazali smo kako se može kroskompajlirati izvorni kod na razvojnoj PC platformi, tako da generisani izvršni fajl može da se izvrši na ciljnoj *Raspberry Pi* platformi, pri čemu je korišćen jednostavan primjer bez spoljnih biblioteka koje je potrebno dinamički povezati sa izvršnim fajlom. U drugom zadatku smo dodali opciju kroskompajliranja programa i biblioteka uz pomoć `make` alatke koja omogućava efikasniji proces prevođenja.

U ovom zadatku ćemo vidjeti kako možemo iskoristiti prethodno stečeno  znanje za prevođenje i linkovanje programa koji koriste spoljnje biblioteke.

Kada se koristi neka spoljnja biblioteka i kada se koristi dinamičko povezivanje, potrebno je uraditi neke dodatne korake prije nego što se prevedeni program, koji zavisi od dinamičkih biblioteka, može izvršiti na ciljnoj platformi. Prije svega, osim same aplikacije, potrebno je kroskompajlirati i samu dinamičku biblioteku. Zatim, potrebno je prebaciti oba binarna fajla (i aplikaciju i biblioteku) na ciljnu platformu. Konačno, operativni sistem na ciljnoj platformi mora na odgovarajući način bude upoznat gdje se nalazi biblioteka da bi dinamičko povezivanje moglo da se obavi.

U konkretnom primjeru, koristićemo [*wiringPi*](http://wiringpi.com/) kao spoljnju biblioteku koju se dinamički linkuje sa aplikacijom koja je koristi. Ova biblioteka omogućava pristup hardverskim resursima na *Raspberry Pi* platformi i biće povremeno korišćena u narednim laboratorijskim vježbama. U tom smislu, preporuka je da se student upozna sa funkcijama za rad sa funkcijama koje omogućavaju kontrolu GPIO (engl. *General Purpose Input Output*) sistema *Raspberry Pi* platforme. Da bi mogli da koristimo ovu biblioteku, prvo moramo da preuzmemo njen izvorni kod.

Najlakši način za preuzimanje izvornog koda *wiringPi* biblioteke je da kloniramo njen repozitorijum sljedećom komandom:

```
git clone --depth 1 https://github.com/WiringPi/WiringPi
```

Nakon preuzimanja, bibilioteku možemo kroskompajlirati tako što pređemo u folder `/path/to/wiringPi/wiringPi` i pokrenemo komandu:

```
make CC=arm-linux-gnueabihf-gcc
```

Ovdje je potrebno napomenuti da `/path/to/wiringPi` trebate zamijeniti stvarnom putanjom do direktorijuma u kojem se nalaze resursi `wiringPi` biblioteke. Takođe, opcijom `CC=arm-linux-gnueabihf-gcc` mi zapravo govorimo `make` alatu da koristi kroskompajler umjesto podrazumijevanog `gcc` kompajlera x86 platforme. Ako proučite strukturu `Makefile` fajla koji se nalazi u ovom folderu, vidjećete da je `CC` zapravo varijabla koja se koristi da definiše korišćeni prevodilac.

**Napomena:** Dinamička biblioteka dobijena nakon kroskompajliranja osim ekstenzije `.so` sadrži i broj verzije biblioteke, koji treba odstraniti iz naziva fajla kako bi pri kroskompajliranju i dinamičkom linkovanju našeg programa biblioteka bila vidljiva. Odstranjivanje se postiže prostim preimenovanjem fajla (koriščenjem alata `mv`) u naziv bez verzije.

Ovako prevedena biblioteka može se koristiti u našim programima. Važno je napomenuti da je fajl dinamičke biblioteke potrebno prebaciti na ciljnu platformu u neki od sistemskih direktorijuma za čuvanje biblioteka (npr. `/usr/lib`). Za pristup ovim folderima, neophodne su administratorske privilegije pa se za prenos koristi komanda:

```
scp /path/to/wiringPi/lib/libwiringPi.so root@192.168.23.xxx:/usr/lib
```

gdje `xxx` označava dio IP adrese koji je jedinstven za svaki *Raspberry Pi*. Alternativno, pri daljinskom pristupu ciljnoj platformi, potrebno je da koristite odgovarajući *hostname* i *port* kao što je to ranije pojašnjeno.

Ovdje treba napomenuti da *Rasbian* dolazi preinstaliran bez podešene šifre za administratorski korisnički nalog (`root`), tako da komanda iznad u većini slučajeva ne može da se koristi. U tom slučaju, biblioteku trebate kopirati korišćenjem `scp` komande u `home` direktorijum korisnika `pi`, a zatim je, nakon što se logujete sa ovim korisničkim nalogom, prebaciti u `/usr/lib` (korišćenjem `sudo` da bi imali administratorske privilegije).

**Napomena:** Prenos biblioteke se može obaviti i pomoću pravila `make` alata koje je definisano u `Makefile` fajlu, na način kako je ranije objašnjeno.

Da bismo preveli dati primjer, koji se dinamički linkuje sa *wiringPi* bibliotekom, prvo je potrebno obezbijediti da se student nalazi u direktorijumu `lab1-3/src` na razvojnoj platformi, u kojem se nalazi izvorni kod neophodan za realizaciju zadatka.

U ovom zadatku je dat primjer manipulacije jednim GPIO pinom koji je konfigurisan kao izlaz. Program periodično, nakon isteka intervala od pola sekunde, invertuje stanje izlaznog pina GPIO0. Ako na ovaj pin povežemo LED diodu, aktivnost se manifestuje "žmiganjem" (engl. *blinking*). Fizička lokacija pina na *Raspberry Pi* platformi se može pronaći pomoću [*Pinout*](https://pinout.xyz/) alata. U konkretnom slučaju, GPIO0 pin se nalazi fizičkom pinu 11 (BCM 17).

Prilagodite dati `Makefile` tako da omogućite prevođenje i dinamičko linkovanje ovog programa, a zatim korišćenjem `make` alatke prevedite program i prenesite ga na ciljnu platformu slično kao u drugom zadatku.

Nakon kroskompajliranja i prenosa dobijenog izvršnog fajla (pod nazivom `blinking`) na ciljnu platformu u folder `/home/pi/student-name/lab1`, kao što je ranije opisano, trebalo bi da se dobije efekat "žmiganja" na povezanoj LED diodi.

## Korišćenje osciloskopa daljinskim pristupom
U slučaju kada niste u prilici da radite direktno u laboratoriji, tj. da povežete LED diodu na pin kao što je objašnjeno u trećem zadatku, i na taj način verifikujete ispravnost programa, možete koristiti osciloskop da potvrdite da se na pinu greneriše periodična povorka impulsa. Za to je potrebno instalirati odgovarajuću softversku podršku koja nam omogućava da kontrolišemo osciloskop i da preuzmemo i vizuelizujemo preuzete podatke. Stoga, prvo trebate na razvojnoj platformi preuzeti *Python* skripte sa repozitorijuma https://github.com/smiljanic997/ikm-remote-osc

```
git clone https://github.com/smiljanic997/ikm-remote-osc
```

Nakon što smo klonirali ovaj repozitorijum, trebamo da instaliramo sljedeće softverske pakete:

- PIP alat za instalaciju *Python* paketa (`sudo apt-get install python3-pip`),
- *Paramiko* paket (`sudo pip3 install paramiko`),
- *Numpy* paket (`sudo apt install python3-numpy`) i
- *Matplotlib* paket (`sudo apt install python3-matplotlib`).

Po instalaciji pomenutih softverskih paketa, treba da se premjestimo u direktorijum `ikm-remote-osc` i da se prebacimo na granu `client_stable_v1`:

```
cd ikm-remote-osc
git checkout client_stable_v1
```

Sve je spremno za pokretanje skripte koja se preko `ssh` programa povezuje sa *Raspberry Pi* platformom na koju je povezan osciloskop i pokreće odgovarajuće *Python* skripte na platformi kako bi se osciloskop mogao podesiti s ciljem preuzimanja željenih podataka za prikazivanje talasnog oblika koji je izmjeren.

**Napomena:** Skripte neophodne za rad sa osciloskop su preinstalirane na *Raspberry Pi* platformi. Sve navedeno iznad se odnosi isključivo na razvojnu platformu.

Skripta se na razvojnoj mašini pokreće u sljedećem obliku:

```
python3 read_rpi.py <hostname> <port>
```
gdje je `<hostname>` i `<port>` potrebno zamijeniti stvarnim parametrima za pristup koji ste dobili od predmetnog asistenta (npr. `proxy50.rt3.io` i `38567`).

Nakon pokretanja, dobićete pitanje da li želite da zadržite prethodna ili da proslijedite nova podešavanja osciloskopa. Osciloskop se uvijek podešava da radi u EDGE režimu trigerovanja sa SINGLE opcijom zadržavanja mjerenja. Ostali parametri koje možete podešavati su:

- kanal (prvi, drugi ili oba),
- horizontalna rezolucija (sec/div),
- vertikalna rezolucija (V/div),
- vertikalni ofset (V),
- horizontalni offset (sec),
- ivica trigerovanja (pozitivna ili negativna) i
- nivo tigerovanja (V).

Nakon podešavanja parametara osciloskopa, skripta se povezuje sa *Raspberry Pi* platformom (kao potvrdu ispravnog povezivanja dobićete poruku `INFO : Authentication (password) successful!`) i postavlja se u režim mjerenja, nakon čega možete pokrenuti kod koji želite da testirate (poruka `Pokreni C kod`). Da bi pokrenuli kod, potrebno je da se u drugom terminalu povežete sa *Raspberry Pi* platformom pomoću `ssh` programa kao ranije i da pokrenete program koji ste prethodno prenijeli na ciljnu platformu.

Ukoliko je sve kako treba, kada osciloskop detektuje događaj za trigerovanje u skladu sa njegovim podešavanjima, nakon nekoliko sekundi će se pojaviti slika sa izmjerenim talasnim oblikom. Ako ste zadovoljni sa rezultatom, sliku trebate sačuvati (posljednja ikonica u donjem lijevom uglu), jer će inače biti izgubljena. Nakon zatvaranja prozora sa slikom, možete pokrenuti novu sesiju mjerenja.

**Napomena:** Ukoliko nije ispunjen uslov za trigerovanje, skripta će čekati 60 sekundi prije nego što prikaže sliku sa mjerenjima. Ovo je indikator da podešavanja osciloskopa nisu adekvatna, a samim tim ni dobijena mjerenja nisu relevantna. Mjerenje treba ponoviti sa drugačijim podešavanjima osciloskopa. Ukoliko se izvršavanje skripte prekine nasilno (npr. sekvencom `Ctrl+C`), serverska aplikacija ostaje pokrenuta još 60 sekundi, tako da je potrebno sačekati to vrijeme prije nego što se mjerenje ponovi.

Dati kod u trećem zadatku treba prilagoditi tako da se kašnjenje (funkcija `delay()`) podesi da bude umjesto 10, umjesto 500 milisekundi (da bi se skratila dužina trajanja sesije mjerenja). Program ponovo prevesti u skladu sa instrukcijama i prenijeti na ciljnu platformu. Pokrenuti sesiju mjerenja, podesiti parametre osciloskopa tako da se dobije talasni oblik koji čini nekoliko impulsa signala, a zatim na ciljnoj platformi pokrenuti kod kako bi se mjerenje obavilo.

**Napomena:** U datom programskom kodu se mijenja pin BCM 17 (što odgovara pinu 0 u kontekstu numeracije koju koristi biblioteka *wiringPi*) kao što je ranije objašnjeno. U konkretnom slučaju, sonda osciloskopa može biti povezana sa nekim drugim GPIO pinom. Ovo je potrebno provjeriti sa predmetnim asistentom prije mjerenja.
