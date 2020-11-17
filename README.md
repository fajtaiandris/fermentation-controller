[![arduino-cli](https://img.shields.io/badge/arduino--cli-0.12.1-brightgreen)](https://github.com/arduino/arduino-cli)

# Fermentálás szabályzó

## Funkciók és állapotok

|Jelző szín  |Funkció                                       |Állapotok        |
|------------|----------------------------------------------|-----------------|
|ZÖLD        | Jelenlegi hőmérséklet megjelenítése          | `1`             |
|NARANCS     | Minimum és maximum hőmérséklet megjelenítése | `2, 3`          |
|PIROS       | Minimum és maximum hőmérséklet beállítása | `4, 5, 6, 7, 8, 9` |

|Állapot | Feladat          |
|--------|------------------|
|0       | alapállapot      |
|1       | jelenlegi hőmérséklet megjelenítésel |
|2       | minimum hőmérséklet megjelenítése |
|3       | maximum hőmérséklet megjelenítése |
|4       | minimum hőmérséklet megjelenítése |
|5       | minimum hőmérséklet csökkentése |
|6       | minimum hőmérséklet növelése    |
|7       | maximum hőmérséklet megjelenítése  |
|8       | maximum hőmérséklet csökkentése    |
|9       | maximum hőmérséklet növelése |

A nyílakon lévő számok a gombnyomások számáit jelölik.
![Állapotok](./automata.svg)

## Kapcsolási rajz

[Megtalálod itt.](./schematics.txt)

## Tennivalók
- [ ] Mappákba rakni a függőségeket és a forráskódot
- [ ] Valami build rendszer
- [ ] CI
- [ ] EEPROM inicializáló program új alaplapokhoz
- [ ] Relé kapcsolása
- [ ] Kód kommentelése
- [ ] [GCC toolchain](https://thinkingonthinking.com/an-arduino-sketch-from-scratch/) Arduino-Cli helyett
