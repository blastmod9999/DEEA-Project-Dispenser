Proiect DEEA
Dispenser automat de bauturi
Nica Antonio-Ionut
323CA

1.	Descrierea aparatului
      Urmatorul dispozitiv are ca scop automatizarea procesului de turnare a bauturilor intr-un bar, restaurant, reducand semnificativ asteptarea la coada sau eroarea umana in cantitatea turnata.
       Modul de functionare al dispenserului este complet automat , singura interactiune care are nevoie de un operator este calibrarea initiala , unde trebuie selectat folosind encoderul pozitiile de start , si ale tuturor pozitiilor pentru pahare. Dupa aceasta faza de calibrare, fiecare utilizator isi selecteaza singur cantitatea si aseaza paharul sau paharele pe pozitiile dorite mai apoi turnarea se va face strict pe acele pozitii. Pe display sunt afisate datele catre utilizator si este afisat tot procesul de turnare.
       Tratarea situatiilor neasteptate de exemplu rasturnare / lovire rezulta in oprirea definitiva a dispozitivului pana la interventia unui operator.


3.

<img width="1079" height="1102" alt="image" src="https://github.com/user-attachments/assets/c4f4387d-fbea-40b9-9660-c4e6a71a960f" />

2.	Detaliarea functionarii
      Aparatul se foloseste de doua microcontrollere : ESP32 (logica principala) si Arduino Uno (miscarea si turnarea). Cele doua microcontrollere comunica prin serial.
	2.1.Descrierea rolului ESP32 : 
ESP32 este “creierul” acesta se ocupa de toate datele primite de la senzori si trimite comenzi catre Arduino Uno pentru miscare si turnare. Acesta primeste semnale de la Termistor (temperatura bauturii) si de la Fotorezistor (detectarea prezentei paharului) . Semnalele de la acestea doua sunt amplificate prin folosirea unui circuit integrat LM358 la care acestea sunt conectate. Microcontrollerul se ocupa si de senzori cititi digital precum Senzorul de vibratii (detectare socuri / rasturnare) care are rol in protectia montajului (Safety) , Encoder rotativ care se ocupa de inputul utilizatorului si de calibrarea initiala, 2 Microswitch-uri alaturi de un Fotorezistor (digital) care se ocupa de detectarea a restul paharelor si Display OLED 7pin care afiseaza datele catre utilizator.

2.2.Descrierea rolului Arduino
	Arduino este cel care duce partea “grea” a montajului, acesta se ocupa de instalatiile de putere precum Stepper Motor (pozitionare) + driver module care misca bratul dispenserului punandu-l pe pozitiile stabilite de ESP32 si Pompa cu diafragma R385 (9V) semnalul catre pompa este controlat printr un amplificator operational LM358 + un tranzistor 2N2222 si un TIP31 (de putere) care primeste semnalul de la Arduino (30% , 60%, 90%, 100%) controleaza puterea cu care se toarna si protejeaza microcontrolerul de cei 9V de la baterie destinati pompei.

3.Senzori prezenti
		1.Fotorezistor (analog + digital)
		2.Microswitch  (digital)
		3.Senzor Vibratii SW-420 (digital)
		4.Senzor de pozitie (digital) : Encoder rotativ
		5.Termistor (analog) 

4. Blocuri analogice
		1.Fotorezistor prin LM358 – 2 (repetor de tensiune)
		2.Termistor prin LM358 – 2 (preamplificator ne-inversor)
		3.Pompa R385 prin LM358 + Darlington (TIP31 + 2N2222)
 
5.Schema completa a aparatului

<img width="978" height="1400" alt="image" src="https://github.com/user-attachments/assets/8d8e2fa8-deb0-4335-9d88-cfc660b57ad7" />

6.Bill of Materials (220 – 280 RON)
 <img width="861" height="1208" alt="image" src="https://github.com/user-attachments/assets/f932d5f2-6355-4488-908c-8c0444d66c5a" />

7.Link Filmulet functionare + cod Github
Link videoclip functionare :
https://youtu.be/Xdkoyxmtp8U


