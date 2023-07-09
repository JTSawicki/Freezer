# Komendy kontrolera przepływu
Wysyłane i odbierane komendy są po porcie COM z baudrate równym 115200.

Komendy muszą zaczynać się znakiem końca lini '\n'.

## **01 - 20** Komendy ogólne

 - 01 - Żądanie identyfikacji. Zwraca "ESP32-Flow"
 - 02 - Żądanie obecnego przepływu. Zwraca "00.00" w litrach / minutę
 - 03 - Żądanie poziomu nastawy flow rate. Zwraca "00.00" w litrach / minutę
 - 04 - Żądanie czy nastąpiło przekroczenie nastawy flow rate. Zwraca "true" lub "false"
 - 05 - Ustawienie poziomu nastawy flow rate. Nastawa w postaci "00.00"
 - 06 - Uzbraja czujnik
 - 07 - Rozbraja czujnik
 - 08 - Tekstowe podsumowanie ustawień
 - 09 - Żądanie czy czujnik uzbrojony. Zwraca "true" lub "false"

## **21 - 30** Komendy WiFi
 - 21 - Żądanie czy podpięto do internetu. Zwraca "true" lub "false"
 - 22 - Ustawienie nazwy sieci WiFi
 - 23 - Ustawienie hasła WiFi
 - 24 - Połącz ponownie do WiFi używając nowych danych
 - 25 - Aktywacja modułu WiFi(i E-Mail)
 - 26 - Deaktywacja modułu WiFi(i E-Mail)

## **31 - 40** Komendy E-Mail
 - 31 - Wysyła testową wiadomość na zapisany adres
 - 32 - Ustawienie serwera E-Mail
 - 33 - Ustawienie portu serwera E-Mail. Nastawa postaci "000"
 - 34 - Ustawienie adresu E-Mail
 - 35 - Ustawienie hasła E-Mail
 - 36 - Ustawia docelowy E-Mail