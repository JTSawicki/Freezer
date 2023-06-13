#include <Arduino.h>
#include "my_variables.hpp"
#include "my_functions.hpp"

String message;
char command[2];

void GeneralCommands();
void WiFiCommands();
void EMailCommands();

/// @brief Funkcja odpowiada za komunikację z PC
void SerialResponse() {
    if (!Serial.available()) return;

    message = Serial.readStringUntil('\n');
    if (message.length() < 2) return;
    command[0] = message[0];
    command[1] = message[1];
    message = message.substring(2);

    switch(command[0]) {
        case '0':
            GeneralCommands();
            break;
        case '2':
            WiFiCommands();
            break;
        case '3':
            EMailCommands();
            break;
    }
}

void GeneralCommands() {
    switch(command[1]) {
        case '1':
            Serial.println("ESP32-Flow");
            break;
        case '2':
            Serial.println(flow, 2);
            break;
        case '3':
            Serial.println(flow_target, 2);
            break;
        case '4':
            if (below_target_dedected)
                Serial.println("true");
            else
                Serial.println("false");
            break;
        case '5':
            flow_target = message.toFloat();
            Save_FlowTarget();
            break;
        case '6':
            ArmFlowSensor(true);
            break;
        case '7':
            ArmFlowSensor(false);
            break;
        case '8':
            Print_AllPreferences();
            break;
        case '9':
            if (armored)
                Serial.println("true");
            else
                Serial.println("false");
            break;
    }
}

void WiFiCommands() {
    switch(command[1]) {
        case '1':
            if (IsConnectedToWiFi())
                Serial.println("true");
            else
                Serial.println("false");
            break;
        case '2':
            wifi_name = message;
            Save_WiFiName();
            break;
        case '3':
            wifi_password = message;
            Save_WiFiPassword();
            break;
        case '4':
            ConnectToWiFi();
            break;
        case '5':
            wifi_module_activated = true;
            Save_WiFiModuleActivated();
            ConnectToWiFi();
            break;
        case '6':
            wifi_module_activated = false;
            Save_WiFiModuleActivated();
            DisconnectWiFi();
            break;
    }
}

void EMailCommands() {
    switch(command[1]) {
        case '1':
            SendEmail("ESP32-FlowSensor Test", "Test message from flow sensor");
            break;
        case '2':
            email_host_name = message;
            Save_EmailHostName();
            break;
        case '3':
            email_host_port = message.toInt();
            Save_EmailHostPort();
            break;
        case '4':
            email_name = message;
            Save_EmailName();
            break;
        case '5':
            email_password = message;
            Save_EmailPassword();
            break;
        case '6':
            email_target = message;
            Save_EmailTarget();
            break;
    }
}