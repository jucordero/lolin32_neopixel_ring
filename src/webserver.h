#include <Arduino.h>
// #include <colors.h>

const char html_head[] PROGMEM = R"rawliteral( 
<!DOCTYPE html><html>
<head>
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
    <link rel=\"icon\" href=\"data:,\">
    <style>
        html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
            .button {background-color: #4CAF50; color: white; padding: 16px 40px; font-size: 30px; margin: 2px; cursor: pointer; height : 150px; width : 200px;}
            .button2 {background-color: #555555;}
    </style>
</head>
<body>
    <h1>ESP32 Lamp Server</h1>
)rawliteral";

void startServer(){

    for(uint16_t pos; pos < 20; pos++)
        strip.SetPixelColor(pos, red(peak));
    strip.Show();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        strip.RotateRight(1);
        strip.Show();
    }
    server.begin();
}

void checkInconmingConnections(){
    WiFiClient client = server.available();   // Listen for incoming clients

    if (client) {                             // If a new client connects,

        currentTime = millis();
        previousTime = currentTime;
        String currentLine = "";                // make a String to hold incoming data from the client

        while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected

            currentTime = millis();
            if (client.available()) {             // if there's bytes to read from the client,
            char c = client.read();             // read a byte, then
            header += c;

            if (c == '\n') {                    // if the byte is a newline character

                // if the current line is blank, you got two newline characters in a row.
                // that's the end of the client HTTP request, so send a response:
                if (currentLine.length() == 0) {

                    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                    // and a content-type so the client knows what's coming, then a blank line:
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println("Connection: close");
                    client.println();
            
                    for (uint8_t i; i < anims; i++){

                        char state_index[15];
                        sprintf(state_index, "GET /%s/on", names[i]);
                        if (header.indexOf(state_index) >= 0){
                            clear_grid();
                            loopCounter = 0;
                            states[animation_code] = false;
                            animation_code = i;
                            states[animation_code] = true;
                        }
                    }

                    if (header.indexOf("GET /brightness/plus") >= 0 && peak < 130)
                        peak = peak + 10;

                    else if (header.indexOf("GET /brightness/minus") >= 0 && peak > 10)
                        peak = peak - 10;
                    
                    else if (header.indexOf("GET /angle/plus") >= 0) {
                        if (angle > 18) angle = 0;
                        else angle++;
                    }

                    else if (header.indexOf("GET /angle/minus") >= 0) {
                        if (angle < 0) angle = 19;
                        else angle--;
                    }

                    else if (header.indexOf("GET /prob/plus") >= 0 && prob < 45)
                        prob = prob + 5;

                    else if (header.indexOf("GET /prob/minus") >= 0 && prob > 5)
                        prob = prob - 5;

                    // Display the HTML web page

                    client.println(html_head);

                    for (uint8_t i; i < anims; i++){

                        if (i == 4){
                            client.print("<p><a href=\"/angle/minus\"><button class=\"button\">-</button></a>");
                            if (states[i])    client.printf("<a href=\"/%s/on\"><button class=\"button\">%s</button></a>", names[i], names[i]);
                            else              client.printf("<a href=\"/%s/on\"><button class=\"button button2\">%s</button></a>", names[i], names[i]);
                            client.print("<a href=\"/angle/plus\"><button class=\"button\">+</button></a></p>");
                        }

                        else if (i == 2 || i == 5 || i == 8){
                            client.print("<p><a href=\"/prob/minus\"><button class=\"button\">-</button></a>");
                            if (states[i])    client.printf("<a href=\"/%s/on\"><button class=\"button\">%s</button></a>", names[i], names[i]);
                            else              client.printf("<a href=\"/%s/on\"><button class=\"button button2\">%s</button></a>", names[i], names[i]);
                            client.print("<a href=\"/prob/plus\"><button class=\"button\">+</button></a></p>");
                        }

                        else {
                            if (states[i])    client.printf("<p><a href=\"/%s/on\"><button class=\"button\">%s</button></a></p>", names[i], names[i]);
                            else              client.printf("<p><a href=\"/%s/on\"><button class=\"button button2\">%s</button></a></p>", names[i], names[i]);
                        }
                    }

                    client.printf("<body><h2>Brightness %d</h2>", peak);
                    client.printf("<body><h2>Probability %d</h2>", prob);
                    client.println("<p><a href=\"/brightness/minus\"><button class=\"button\">-</button></a> <a href=\"/brightness/plus\"><button class=\"button\">+</button></a></p>");
                    client.println("</body></html>");
                    // The HTTP response ends with another blank line
                    client.println();
                    break;
                } 
                else currentLine = "";
            }
            else if (c != '\r')  // if you got anything else but a carriage return character,
                currentLine += c;      // add it to the end of the currentLine
        }
    }
    header = ""; // Clear the header variable
    client.stop(); // Close the connection
    }
}