#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

/// @brief Funkcja odpowiada za obługę zapytań i odpowiedzi po magistrali Serial. Wywoływana w loop. Nieblokująca
void SerialResponse();

/// @brief Funkcja inicjalizuje pamięć i wczytuje zmienne
void InitializeMemory();

// Funkcje zapisu ustawień
void Save_WiFiName();
void Save_WiFiPassword();
void Save_WiFiModuleActivated();
void Save_EmailHostName();
void Save_EmailHostPort();
void Save_EmailName();
void Save_EmailPassword();
void Save_EmailTarget();
void Save_FlowTarget();
void Save_Armored();
void Save_BelowTargetDedected();

/// @brief Funkcja wysyła na serial wszystkie zapisane zmienne
void Print_AllPreferences();

/// @brief Inicjalizacja wifi i email
void InitializeEmailAndWiFi();

/// @brief Podepnij do wifi używająć nowych danych
void ConnectToWiFi();

/// @brief Odepnij od wifi
void DisconnectWiFi();

/// @brief Czy połączenie z wifi aktywne
bool IsConnectedToWiFi();

/// @brief Wysyła wiadomość email
/// @param text Zawartość email
/// @param title Tytuł email
/// @return Czy udało się wysłać wiadomość
bool SendEmail(String title, String text);

/// @brief Uzbraja lub rozzbrja czujnik
/// @param arm Czy uzbroić
void ArmFlowSensor(bool arm);

/// @brief Ustawia wykrycie zbyt niskiego poziomu
void TriggerBelowTargetDedected();


#endif /* MY_FUNCTIONS_H */