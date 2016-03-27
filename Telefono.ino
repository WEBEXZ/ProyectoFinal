#include <SoftwareSerial.h>
#define rx 2
#define tx 3
SoftwareSerial gsm(rx, tx);
char opcion = 0;
unsigned char RC = 13; //Retorno de carro
unsigned char SUB = 26;

void setup() {
  gsm.begin(2400); //Velocidad de transferencia del modulo GSM
  Serial.begin(9600);
  Serial.println("\tProyecto Telefono");
  Serial.println("\t\tMenu\n\n\t\t1.-Enviar SMS\n\t\t2.-Llamar");
}

void loop() {
  if (Serial.available() > 0) {
    opcion = Serial.read();
    Serial.println(opcion);
    switch (opcion) {
      case '1':
        enviarSMS();
        break;
      case '2':
        llamar();
        break;
      default:
        break;
    }
  }
}

void enviarSMS() {
  Serial.println("Enviando mensaje....");
  gsm.write("AT+CMGF=1"); //Establecemos el GSM en modo de texto
  gsm.write(RC);
  delay(1000);
  gsm.print("AT+CMGS=\"9514404243\"\r"); //Numero a enviar el mensaje
  delay(1000);
  gsm.print("Hola desde Arduino"); //Cuerpo del mensaje
  delay(1000);
  gsm.write(SUB);
  Serial.println("Mensaje enviado con exito");
}

void llamar() {
  Serial.println("Llamando...");
  gsm.print("ATD9514404243;\r");
  delay(12000);
  gsm.print("ATH\r");
  Serial.println("Llamada Finalizada");  
}
