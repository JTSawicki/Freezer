#include <Arduino.h>
#include "my_variables.hpp"
#include "my_functions.hpp"

const float flow_factor = 1780;
volatile float flow = 0;
volatile float flow_target = 0;

volatile bool below_target_dedected = false;
volatile bool armored = false;

String wifi_name = "";
String wifi_password = "";
bool wifi_module_activated = false;

String email_host_name = "";
volatile int email_host_port;
String email_name = "";
String email_password = "";
String email_target = "";

void ArmFlowSensor(bool arm) {
    if (arm) {
        armored = true;
        below_target_dedected = false;
    } else {
        armored = false;
        below_target_dedected = false;
    }
    Save_Armored();
    Save_BelowTargetDedected();
}

void TriggerBelowTargetDedected() {
    below_target_dedected = true;
    Save_BelowTargetDedected();
    String message = "Przepływ poniżej normy: ";
    message = message + String(flow_target, 2);
    message = message + "\nWartość przepływu: ";
    message = message + String(flow, 2);
    SendEmail("ESP32-FlowSensor FlowError !!!", message);
}