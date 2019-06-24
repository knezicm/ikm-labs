# Lista eliminatornih ispitnih pitanja #

## Uvod u industrijske komunikacione mreže ##

1. Da li se kod 4-20mA standarda koristi analogni ili digitalni prenos podataka?
2. Kakva je razlika između ISO OSI i TCP/IP komunikacionog modela?
3. Šta je protokol?
4. Šta je servis?
5. Objasniti vezu između PDU i SDU.
6. Širina propusnog opsega (*bandwidth*).
7. Propusnost (*throughput*).
8. Vrijeme propagacije signala (*propagation time*).
9. Vrijeme predaje (*transmission time*).
10. Vrijeme ciklusa i RTT (*Round Trip Time*).

## Interfejsi ##

1. Da li je UART serijski ili paralelni interfejs?
2. Koliko linija se koristi za prenos podataka kod UART interfejsa?
3. Da li UART koristi simpleksnu, poludupleksnu, ili punu dupleksnu komunikaciju?
4. Koja je uloga *start* i *stop* bita kod UART komunikacije?
5. Ako se koristi sljedeća konfiguracija UART interfejsa: 8 bita podataka, bez provjere parnosti i jedan *stop* bit, odrediti trajanje slanja jednog karaktera pri bitskoj brzini od 9600 bps.
6. Ako se UART karakteri šalju jedan za drugim bez pauze, koliki je *overhead* pri prenosu informacija (izraziti procentualno)? Objasniti kako ste došli do rezultata.
7. Podrazumijvano se kod UART interfejsa šalje prvo LSB bit podataka. Da li je to tačno?
8. Gdje se obavlja odmjeravanje podatka kod UART interfejsa: (1) na početku bitskog intervala, (2) u sredini bitskog intervala, ili (3) na kraju bitskog intervala?
9. Objasniti zašto bitska brzina kod UART interfejsa može da ima toleranciju maksimalno 2.5%.
10. Objasnite kako biste automatski detektovali bitsku brzinu UART interfejsa.
11. Navedite najmanje tri greške koje se mogu detektovati kod UART komunikacije.
12. Šta je *break condition*?
13. Navedite najmanje tri tipične bitske brzine UART interfejsa.
14. Šta je transiver?
15. Kakve su razlike između RS-232 i RS-485 protokola?
16. Zašto se parice upredaju?
17. Kakva je razlika između *multi-drop* i *multi-point* topologije?
18. Nacrtati šemu za terminaciju RS-485 magistrale.
19. Koliki je maksimalan broj prijemnika i predajnika koji se može povezati na RS-422 magistralu?
20. Koliki je maksimalan broj prijemnika i predajnika koji se može povezati na RS-485 magistralu?
21. Koliko linija se koristi za prenos podataka kod SPI interfejsa?
22. Koliko *master* uređaja može da postoji u jednoj SPI mreži?
23. Da li SPI koristi simpleksnu, poludupleksnu, ili potpunu dupleksnu komunikaciju?
24. Koliki je minimalan broj signalnih linija koji se može koristiti za komunikaciju preko SPI interfejsa?
25. Koje su osnovne prednosti i mane *daisy-chain* (ulančano) u odnosu na paralelno (magistrala) povezivanje SPI uređaja?
26. Da li se kod SPI uređaja koristi sinhrona ili asinhrona komunikacija?
27. Objasniti parametre CPOL i CPHA. Ilustrovati konkretnim primjerom.
28. Da li I2C koristi simpleksnu, poludupleksnu, ili potpunu dupleksnu komunikaciju?
29. Koliko linija se koristi za prenos podataka kod I2C interfejsa?
30. Koliko *master* uređaja može da postoji u jednoj I2C mreži?
31. Nacrtati strukturu jednostavne I2C mreže koja se sastoji iz jednog *master* i tri *slave* čvora.
32. Koja ivica kod I2C interfejsa duže traje? Zašto?
33. Kolika je dužina adrese I2C čvora (u bitima)?
34. Objasniti na koji način se razrješava konflikt na I2C magistrali.
35. Navesti osnovne elemente I2C transakcije. Skicirati jednu jednostavnu I2C transakciju.
36. Da li se kod I2C uređaja koristi sinhrona ili asinhrona komunikacija?
37. Koja je uloga *start* i *stop* bita kod I2C komunikacije?
38. Šta je *repeated start* bit i koja je njegova uloga u I2C transakciji?
39. Šta je *sample point*, a šta *setup point*?
40. Kakva je razlika između *in-band* i *out-of-band* signalizacije? Navesti primjer za jedan i drugi slučaj.

## Industrijski protokoli ##

1. Navesti osnovne servise na sloju linka podataka.
2. Šta je omeđavanje okvira i zašto je ono potrebno?
3. Navesti načine omeđavanja okvira.
4. Šta je umetanje bajtova?
5. Šta je i čemu služi ESC bajt?
6. Šta je umetanje bita?
7. Ako u polju podataka nekog okvira imamo sekvencu bita `01110111110111011111111100000111`, napisati sekvencu koja se dobije nakon umetanja bita. Posebno označiti umetnute bite.
8. Ako u primljenom okviru imamo sekvencu bita `01110111110011101111101111000001111`, napisati sekvencu koja se dobije nakon izbacivanja umetnutih bita. Posebno označiti umetnute bite.
9. Koliki maksimalni *overhead* možemo imati prilikom umetanja bajtova i pod kojim uslovima se on može pojaviti?
10. Koliki maksimalni *overhead* možemo imati prilikom umetanja bita i pod kojim uslovima se on može pojaviti?