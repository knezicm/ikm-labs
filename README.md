# Industrijske komunikacione mreže (2322)
U okviru ovog repozitorijuma, studenti mogu da pronađu sve materijale nephodne za realizaciju laboratorijskih vježbi iz predmeta Industrijske komunikacione mreže (2322). Laboratorijske vježbe su bazirane na *Raspberry Pi* platformi i *Linux* operativnom sistemu. Za razvoj i prevođenje programa koji se izvršavaju na *Raspberry Pi* platformi, studentima je na raspolaganju specifično kreirana virtuelna mašina sa *Ubuntu 16.04 LTS* operativnim sistemom (*rpi-linux-dev*), koja se pokreće u okviru *VMWare Workstation Player* softvera za virtuelizaciju.

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

## Priprema za realizaciju operativnog sistema

*Priprema kros-kompajlera na Raspbian operativnom sistemu*

Kros-kompajler je kompajler koji kreira izvršni kod za platformu koja nije ona na kojoj se izvršava kompajler (npr. kompajler koji radi na Win 7 PC-u, ali generiše kod koji radi na Android smartphone-u). Kros-kompajler je neophodan za kompajliranje koda za više platformi uz jednog razvojnog hosta. Direktno kompajliranje na ciljnoj platformi može biti neizvodivo, npr. na mikrokontroleru ugrađenog sistema, jer ti sistemi ne sadrže operativni sistem.

Koristićemo kros-kompajler da napravimo softver za Raspberry Pi na našem, glavnom računaru, koristeći svu njegovu snagu i njegove prednosti.

Koristićemo biblioteku *wiringPi* kao primjer. 

*Dodavanje alatki kros-kompajlera i platforme na naš računar*

-	Kreirati direktorijum unutar Home direktorijuma na virtuelnoj mašini, pod nazivom *student* (u njega ćemo smještati sve):
   ```
   mkdir student
   ```
-	Pristupiti praznom direktorijumu student:
  ```
  cd student \
  ```
-	Povući RaspberryPi toolchain sa: https://github.com/raspberrypi/tools
  ```
  git pull https://github.com/raspberrypi/tools
  ```
  Napomena: da bi mogli povući RaspberryPi toolchain, neophodno je instalirati *git* komandom:
   ```
     sudo apt-get install git
   ```
      
  
  
Dobijene alate ćemo koristiti da napravimo vlastite RaspberryPi aplikacije. Ovaj direktorijum (raspberrypi/tools) sadrži alatke za build-ovanje, standardne biblioteke i sistemske pozive koji će trebati našoj aplikaciji.

Koristićemo standardnu pthreads biblioteku (koja se nalazi u preuzetom toolchain-u), lokalno-kompajliranu open-source wiringPi biblioteku i trenutni C11 standard sa našim kompajlerom.

Preostalo je da povučemo i *wiringPi* biblioteku sa http://wiringpi.com/download-and-install/:
  ```
  git pull http://wiringpi.com/download-and-install/
  ```
  
- CMake 

Cmake se koristi za kreiranje makefile-ova za projekat. Cmake ćemo morati da pokrenemo bar dva puta: jednom za našu glavnu aplikaciju, i jednom za našu prilagođenu biblioteku. Svaki od ova dva slučaja će morati da ima sopstveni tekstualni fajl “*CmakeLists.txt*” u kom je specificirano kako će se kompajlirati izvorni fajl, koji je toolchain fajl itd.
Napomena: Ovi fajlovi su dostupni na repozitorujumu za svaku laboratorijsku vježbu.


## Dodatna literatura
TBA
