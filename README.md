**Manuál - Snake**

    Hra Snake je jednoduchá hra pro jednoho hráče. Hráč ovládá klávesami hada na obdélníkové ploše, která je nekonečná, a proto se had
    zanořením na jednom okraji objeví na protějším. Cílem hry je získat co nejdelšího hada, který narůstá spolknutím jídla. V případě  
    spolknutí jídla zablikají obě RGB LEDky zeleně a na LED pásce se rozsvítí další dioda. Hra končí v momentě, kdy hráč narazí do těla 
    hada. V tomto případě se RGB LEDky rozsvítí červeně. Pokud hráč bude chtít v menu upravit barvu nebo rychlost hada, tak je úspěšná 
    změna hlášena modrým bliknutím RGB LEDek.
    

**Ovládání hry**

    - 'w' --> Pohyb nahoru
    - 's' --> Pohyb dolu
    - 'a' --> Pohyb doleva 
    - 'd' --> Pohyb doprava
    - 'q' --> Ukončení   

**Ovládání menu**

    - 'w' --> Pohyb nahoru
    - 's' --> Pohyb dolu
    - 'a' --> Pohyb doleva (Při výběru rychlosti a barvy)
    - 'd' --> Pohyb doprava (Při výběru rychlosti a barvy)
    - ENTER --> Pro zvolení zvýrazněného pole

**Hlavní stránka menu**

    - "PLAY" --> Začátek hry
    - "Options" --> Nastavení hry
    - "Quit" --> Vypnutí hry

**Options**

    - "SPEED" --> Výběr rychlosti hada
    - "COLORS" --> Výběr barvy hada
    - "BACK" --> Návrat na hlavní stránku

**Colors**

    - Stisknutím kláves 'a' a 'd' si vyberete barvu 
    - "SET" --> Uložení barvy
    - "BACK" --> Návrat do Options

**Speed**

    - Stisknutím kláves 'a' a 'd' si vyberete rychlost 
    - "SET" --> Uložení rychlosti
    - "BACK" --> Návrat do Options

**End Screen**

    - "SCORE" --> Skóre, kterého jste dosáhli v minulé hře 
    - "PLAY AGAIN" --> Nová hra
    - "MAIN MENU" --> Návrat do menu
    - "Quit" --> Vypnutí hry

**Kompilace a spuštění**

    - V souboru se nachází Makefile, kde v případě připojování přes server je třeba nastavit na 12 řádku IP adresu. 
    - Kompilace probíhá v příkazové řádce pomocí příkazu "make". Spuštění probíhá pomocí příkazu make run.
    - Brute room with MZ_APOs: https://cw.felk.cvut.cz/courses/apo/
    
Hra byla vytvořena studenty Matyáš Manur a Tomáš Majer jako semestrální práce pro předmět Architektura Počítačů
