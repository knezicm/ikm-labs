# Laboratorijska vježba 6: Protokoli CAN i CANopen #

## Priprema za vježbu ##


## Zadatak 1: ##

```
sudo nano /boot/config.txt
```

kernel >= 4.4.x

```
dtparam=spi=on
dtoverlay=mcp2515-can0,oscillator=10000000,spimaxfrequency=1000000,unterrupt=25
```

stariji kerneli

```
dtparam=spi=on
dtoverlay=mcp2515-can0,oscillator=10000000,spimaxfrequency=1000000,unterrupt=25
dtoverlay=spi-bcm2835-overlay
```

Nakon toga, slLIN modul se od strane *Linux* operativnog sistema vidi kao standardni mrežni interfejs, što znači da se može kontrolisati i podešavati standardnim komandama. Tako, na primjer, ako želimo da omogućimo, pregledamo statusne i druge informacije ili onemogućimo interefejs, koristimo sljedeće komande:

```
sudo ip link set can0 up type can bitrate 125000	# enable interface
ip link show dev can0						    	# print info
sudo ip link set sllin0 down    					# disable interface
```

## Zadatak 2:##

