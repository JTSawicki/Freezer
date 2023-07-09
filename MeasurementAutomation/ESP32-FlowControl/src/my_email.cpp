#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "my_variables.hpp"
#include "my_functions.hpp"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

void ConnectToWiFi();

void InitializeEmailAndWiFi() {
    ConnectToWiFi();
}

void ConnectToWiFi() {
    if (!wifi_module_activated)
        return;
    if (IsConnectedToWiFi())
        return;
    
    WiFi.begin(wifi_name.c_str(), wifi_password.c_str());
    WiFi.setAutoReconnect(true);
}

void DisconnectWiFi() {
    if (!IsConnectedToWiFi())
        return;
    
    WiFi.disconnect();
}

bool IsConnectedToWiFi() {
    if (!wifi_module_activated)
        return false;
    
    if (WiFi.status() == WL_CONNECTED)
        return true;
    return false;
}

void smtpCallback(SMTP_Status status) {
    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
}

bool SendEmail(String title, String text) {
    if (!wifi_module_activated)
        return false;
    
    if (!IsConnectedToWiFi())
        return false;
    
    smtp.debug(1);
    smtp.callback(smtpCallback);

    /* Declare the session config data */
    ESP_Mail_Session session;

    /* Set the session config */
    session.server.host_name = email_host_name;
    session.server.port = email_host_port;
    session.login.email = email_name;
    session.login.password = email_password;
    session.login.user_domain = "";

    session.time.ntp_server = F("pool.ntp.org,time.nist.gov");
    session.time.gmt_offset = 3;
    session.time.day_light_offset = 0;

    /* Declare the message class */
    SMTP_Message message;

    /* Set the message headers */
    message.sender.name = "ESP32-FlowSensor";
    message.sender.email = email_name;
    message.subject = title.c_str();
    message.addRecipient("Sara", email_target);

    /*Send HTML message*/
    message.text.content = text.c_str();
    message.text.charSet = "us-ascii";
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    /* Connect to server with the session config */
    if (!smtp.connect(&session, false))
        return false;
    
    /* Start sending Email and close the session */
    MailClient.sendMail(&smtp, &message);
}
