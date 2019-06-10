# Teme projektnih zadataka #

## 2019 ##
U okviru projektnog zadatka, potrebno je realizovati *Modbus RTU* mrežu koja se sastoji iz jednog *master* i dva *slave* čvora. Čvorovi se na fizičkom sloju povezuju preko RS-485 infrastrukture. Čvorovi *Master* i *Slave #1* se specifično realizuju, dok se kao *Slave #2* koristi već realizovan uređaj koji posjeduje 4 relejna izlaza.

### Funkcionalnost *Slave #1* čvora ###
Čvor treba da periodično očitava analognu vrijednost, koju odmjerava A/D konvertor povezan preko SPI interfejsa (MCP3204), i tako očitanu vrijednost smješta u odgovarajuću promjenljivu. Period odmjeravanja treba da bude konfigurabilan u opsegu od 1-100 ms, takođe preko odgovarajuće promjenljive. U okviru iste promjenljive, treba predvidjeti polja za podešavanje adrese *slave* uređaja, kao i bitske brzine (prema proizvoljno odabranom formatu). Pomenute promjenljive je potrebno mapirati sa podacima aplikacionog sloja definisanim *Modbus* specifikacijom.

Promjenljiva u kojoj se čuva analogna procesna veličina treba da bude tipa *Input Register*, dok promjenljiva u kojoj se podešava period odmjeravanja, adresa *slave* uređaja i bitska brzina, treba da bude tipa *Holding Register*. Detalji o postupku mapiranja procesnih promjenljivih u okviru *libmodbus* biblioteke, može se pronaći u [dokumentaciji](https://libmodbus.org/docs/v3.1.1/modbus_mapping_new.html), odnosno analizom postojećih primjera u repozitorijumu biblioteke (npr. `unit-test-server.c`).

### Funkcionalnost *Master* čvora ###
Čvor treba da koordiniše razmjenu podataka u okviru *Modbus* mreže. Osim toga, ovaj čvor treba da realizuje funkcionalnost kontrolera sistema, koja je opisana sljedećom logikom:

- Prilikom pokretanja programa, definišu se željena zadana vrijednost procesne veličine, vrijeme ciklusa kontrolera u milisekundama, bitska brzina i adrese *slave* uređaja (npr. `./modbus-master 200 10 9600 1 2` znači da je zadana vrijednost 200, vrijeme ciklusa 10 ms, bitska brzina 9600 bps, adresa *Slave #1* čvora 1, a adresa *Slave #2* čvora 2). Vrijeme ciklusa se može podešavati u opsegu od 1 ms do 1 sekunde.
- Ako analogna vrijednost koju šalje *Slave #1* premašuje 10% zadane vrijednosti, kontroler treba da isključi aktuator realizovan u čvoru *Slave #2* (relejni izlaz po sopstvenom izboru).
- Ako je analogna vrijednost koju šalje *Slave #1* ispod 20% zadane vrijednosti, kontroler treba da uključi aktuator realizovan u čvoru *Slave #2*.
- Ukoliko se dese neke greške u komunikaciji, *Master* čvor treba da prikazuje odgovarajuče poruke u terminalu.

### Analiza i konfiguracija *Modbus* mreže ###
Opisanu *Modbus* mrežu je potrebno analizirati, kako bi se ona mogla adekvatno konfigurisati. U tom smislu, potrebno je sprovesti teorijsku analizu kašnjenja u mreži za datu topologiju, s ciljem procjene adekvatnog minimalnog vremena ciklusa koji se može koristiti za datu konfiguraciju mreže.

Osim toga, potrebno je verifikovati ispravnost rada mreže tako što će se posmatrati saobraćaj osciloskopom i odgovarajućim dekoderom za *Modbus* protokol, koji je dostupan u okviru [*Sigrok*](https://sigrok.org/) i [*PulseView*](https://sigrok.org/wiki/PulseView) softvera. U tom smislu, prvo je neophodno instalirati pomenute softverske pakete i upoznati se sa njihovim funkcionalnostima, a zatim iskoristiti ove resurse za monitoring *Modbus* saobraćaja u realizovanoj mreži.

Konačno, da bi se mogla adekvatno debagovati komunikacija sa A/D konvertorom, potrebno je proširiti funkcionalnost *Sigrok* softvera, tako da se nad SPI dekoder, nadogradi specifičan dekoder za analizu komunikacije sa MCP3204 čipom. U tom smislu, potrebno je proučiti [Protocol decoder API](https://sigrok.org/wiki/Protocol_decoder_API) za implementaciju sopstvenog dekodera protokola u *Sigrok* softveru i upoznati se sa koracima koje je u tom smislu potrebno preduzeti ([Protocol decoder HOWTO](https://sigrok.org/wiki/Protocol_decoder_HOWTO)). Takođe, neka od postojećih implementacija može da se iskoristi kao referenca (npr. primjer dekodera `rgb_led_ws281x` koji se može naći u [repozitorijumu projekta](https://sigrok.org/gitweb/?p=libsigrokdecode.git;a=tree;f=decoders/rgb_led_ws281x;h=19221e5c45aa8213cb71adbd81b385aa5ab46aa6;hb=HEAD)).
