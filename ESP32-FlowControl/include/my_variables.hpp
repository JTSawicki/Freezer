#ifndef MY_VARIABLES_H
#define MY_VARIABLES_H

// Ilość pulsów na 1 litr
extern const float flow_factor;

// Obecny przepływ
extern volatile float flow;

// Cel przepływu
extern volatile float flow_target;

// Czy wykryto nieosiągnięcie celu przepływu
extern volatile bool below_target_dedected;

// Czy czujnik jest uzbrojony
extern volatile bool armored;

/// @brief Funkcja odpowiada za ustawianie stanu uzbrojenia czujnika
void ArmFlowSensor(bool arm);

// Nazwa sieci wifi
extern String wifi_name;

// Hasło sieci wifi
extern String wifi_password;

// Ustawienie aktywacji modułu email
extern bool wifi_module_activated;

// Nazwa hosta email
extern String email_host_name;

// Port hosta email
extern volatile int email_host_port;

// Adres email
extern String email_name;

// Hasło email
extern String email_password;

// Adresat wysyłanego email
extern String email_target;

#endif /* MY_VARIABLES_H */