/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi

 */

#include <SPI.h>
#include <Ethernet.h>

#define trigPin1 2 //defines ping sensor1 trigger to pinslot 2
#define echoPin1 3//defines ping sensor1 echo to pinslot 3
#define trigPin2 7 //defines ping sensor2 trigger to pinslot 7
#define echoPin2 6 //defines ping sensor2 echo to pinslot 6

//duration of the traveldistance from ping sensor to object and back to ping sensor
unsigned long duration1=0; //ping sensor1 duration
unsigned long duration2=0; //ping sensor2 duration
 
//defining attributes for distance
int distance1; //distance, to calculate distance to object from ping sensor1
int distance2; //distance, to calculate distance to object from ping sensor2

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT); //ping sensor trigger OUTPUT
pinMode(echoPin1, INPUT); //ping sensor echo INPUT
pinMode(trigPin2, OUTPUT); //ping sensor2 trigger OUTPUT
pinMode(echoPin2, INPUT); //ping sensor2 echo INPUT
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void measureDistance1() {
pinMode(trigPin1, OUTPUT); //ping sensor1 trigger OUTPUT
pinMode(echoPin1, INPUT); //ping sensor1 echo INPUT
digitalWrite(trigPin1, HIGH); //ping sensor1 sends ultrasonic wave
delayMicroseconds(500); //waits for 1 second (1000 microseconds)
digitalWrite(trigPin1, LOW); //ping sensor1 trigger is LOW
duration1 = pulseIn(echoPin1, HIGH); //ping sensor1 retrieves the sound
}
 
//method to measure distance of ping sensor2
void measureDistance2() {
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
digitalWrite(trigPin2, HIGH);
delayMicroseconds(500);
digitalWrite(trigPin2, LOW);
duration2 = pulseIn(echoPin2, HIGH);
}
 
//method to calculate distance based on information recieved with measure distance -method for ping sensor1
int calculateDistance1() {
measureDistance1(); //calls for measureDistance() method
duration1=duration1/2; //divides duration of the sound from ping sensor1 to object and back to the sensor and divides it by 2 (sound travels 2 times)
distance1 = int(duration1/29); //duration is then divided by 29 (sound travels 29 microseconds per centimeter)
delay(150); //waits for 0,5 seconds (500 microseconds)
}
 
//method to calculate distance based on information recieved with measure distance -method for ping sensor2
void calculateDistance2() {
measureDistance2();
duration2=duration2/2;
distance2 = int(duration2/29);
delay(150);
}

int visitor = 0;
void loop() {
  // listen for incoming clients

   
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin

    
                
    calculateDistance1(); //calls for calculateDistance() -method
  calculateDistance2(); //calls for calculateDistance2() -method 
          
         if (distance1 <= 10 ) {
    int myArray[1] = {1};
  if (distance2 <= 10) {
  for (int lewat=0;lewat < 1; lewat++) {
    visitor = visitor + myArray[0];
    client.print("Jumlah Pengunjung : ");
    client.println(visitor);
    delay(150);
  }
  }
    }
   else {
   visitor = visitor + 0;
    client.print("Jumlah Pengunjung : ");
    client.println(visitor);
    delay(150);
 
 }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

