#include <Arduino.h>
#include <Preferences.h>
#include "my_variables.hpp"
#include "my_functions.hpp"

#define WIFI_NAME "WN"
#define WIFI_PASSWORD "WP"
#define WIFI_MODULE_ACTIVATED "WMA"
#define EMAIL_HOST_NAME "EHN"
#define EMAIL_HOST_PORT "EHP"
#define EMAIL_NAME "EN"
#define EMAIL_PASSWORD "EP"
#define EMAIL_TARGET "ET"
#define FLOW_TARGET "FT"
#define ARMORED "A"
#define BELOW_TARGET_DEDECTED "BTD"
Preferences preferences;

void InitializeMemory() {
    preferences.begin("my", false);
    wifi_name = preferences.getString(WIFI_NAME, "");
    wifi_password = preferences.getString(WIFI_PASSWORD, "");
    wifi_module_activated = preferences.getBool(WIFI_MODULE_ACTIVATED, false);
    email_host_name = preferences.getString(EMAIL_HOST_NAME, "");
    email_host_port = preferences.getInt(EMAIL_HOST_PORT, 0);
    email_name = preferences.getString(EMAIL_NAME, "");
    email_password = preferences.getString(EMAIL_PASSWORD, "");
    email_target = preferences.getString(EMAIL_TARGET, "");
    flow_target = preferences.getFloat(FLOW_TARGET, 0);
    armored = preferences.getBool(ARMORED, false);
    below_target_dedected = preferences.getBool(BELOW_TARGET_DEDECTED, false);
    preferences.end();
}

void Save_WiFiName(){
    preferences.begin("my", false);
    preferences.putString(WIFI_NAME, wifi_name);
    preferences.end();
}

void Save_WiFiPassword(){
    preferences.begin("my", false);
    preferences.putString(WIFI_PASSWORD, wifi_password);
    preferences.end();
}

void Save_WiFiModuleActivated() {
    preferences.begin("my", false);
    preferences.putBool(WIFI_MODULE_ACTIVATED, wifi_module_activated);
    preferences.end();
}

void Save_EmailHostName(){
    preferences.begin("my", false);
    preferences.putString(EMAIL_HOST_NAME, email_host_name);
    preferences.end();
}

void Save_EmailHostPort(){
    preferences.begin("my", false);
    preferences.putInt(EMAIL_HOST_PORT, email_host_port);
    preferences.end();
}

void Save_EmailName(){
    preferences.begin("my", false);
    preferences.putString(EMAIL_NAME, email_name);
    preferences.end();
}

void Save_EmailPassword(){
    preferences.begin("my", false);
    preferences.putString(EMAIL_PASSWORD, email_password);
    preferences.end();
}

void Save_EmailTarget() {
    preferences.begin("my", false);
    preferences.putString(EMAIL_TARGET, email_target);
    preferences.end();
}

void Save_FlowTarget(){
    preferences.begin("my", false);
    preferences.putFloat(FLOW_TARGET, flow_target);
    preferences.end();
}

void Save_Armored(){
    preferences.begin("my", false);
    preferences.putBool(ARMORED, armored);
    preferences.end();
}

void Save_BelowTargetDedected(){
    preferences.begin("my", false);
    preferences.putBool(BELOW_TARGET_DEDECTED, below_target_dedected);
    preferences.end();
}

void Print_AllPreferences() {
    Serial.print("wifi_name: ");
    Serial.println(wifi_name);
    Serial.print("wifi_password: ");
    Serial.println(wifi_password);
    Serial.print("email_host_name: ");
    Serial.println(email_host_name);
    Serial.print("wifi_module_activated: ");
    Serial.println(wifi_module_activated);
    Serial.print("wifi_connected: ");
    Serial.println(IsConnectedToWiFi());

    Serial.print("email_host_port: ");
    Serial.println(email_host_port);
    Serial.print("email_name: ");
    Serial.println(email_name);
    Serial.print("email_password: ");
    Serial.println(email_password);
    Serial.print("email_target: ");
    Serial.println(email_target);
    Serial.print("flow_target: ");
    Serial.println(flow_target, 2);
    Serial.print("armored: ");
    Serial.println(armored);
    Serial.print("below_target_dedected: ");
    Serial.println(below_target_dedected);
}