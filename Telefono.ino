#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include "SIM900.h"
#include "sms.h"
#include "call.h"

byte rowPins[4]  {A0, A1, A2, A3}; 
byte colPins[3]  {6, 5, 4}; 

char teclas[4][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

char alfabeto[4][3] = {
  {'A', 'B', 'C'},
  {'D', 'E', 'F'},
  {'G', 'H', 'I'},
  {'*', '0', '#'}
};

LiquidCrystal_I2C LCD(0x27,16,2);
Keypad teclado = Keypad( makeKeymap(teclas), rowPins, colPins, 4, 3 );
Keypad teclado_alfa = Keypad( makeKeymap(alfabeto), rowPins, colPins, 4, 3 );
CallGSM call;
SMSGSM sms;
char x;
char pos_sms;
char telefono[10];
char sms_texto[16];

void setup() {
    LCD.init();
    LCD.backlight();
  if (gsm.begin(2400)) {
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("INICIANDO...");
    delay(2000);
  }
}

void loop() {
    x = teclado.getKey(); //Obtengo el valor del teclado
    switch(x){
      case '1':
        pedirNumero();
        break;
      case '2':
        pedirTexto();
        break;
      case '#':
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("COLGANDO");
        call.HangUp();
        LCD.clear();
        break;
      default:
        recibirMensaje();
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("1.- Llamar");
        LCD.setCursor(0, 1);
        LCD.print("2.- Mandar msj");
        while(call.CallStatus() == CALL_INCOM_VOICE){
          LCD.clear();
          LCD.setCursor(0, 0);
          LCD.print("Recibiendo Call");
          char tecla = teclado.getKey();
          if(tecla != NO_KEY){
              switch(tecla){
                case '*':
                  call.PickUp();
                  break;
                case '#':
                  call.HangUp();
                  break;
              } //FIN DE SWITCH
          } //FIN DEL IF
        } //FIN DEL WHILE
        //recibirMensaje();
        break;
    }
}

void pedirNumero(){
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("INGRESA NUMERO:");
  int i = 0;  //INICIALIZO EL CONTADOR EN 0
  String num; //DECLARO STRING QUE SERÁ EL NÚMERO
  while(i < 10){
    char digito = teclado.getKey();
    if(digito != NO_KEY){
      LCD.setCursor(i, 1); //VA RECORRIENDO DE IZQUIERDA A DERECHA
      LCD.print(digito);  //MOSTRAR EN EL LCD EL DIGITO
      num+= digito; //AGREGO AL STRING EL DIGITO
      i++; //INCREMENTO DEL CONTADOR
    } //FIN DEL IF
  } //FIN DEL WHILE
  hacerLlamada(const_cast<char*>(num.c_str()));
}

void hacerLlamada(char* numero){
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Marcando...");
  call.Call(numero);
}

void pedirTexto(){
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("INGRESA TEXTO:");
  int i = 0; //INICIALIZO EL CONTADOR EN 0
  int vueltas = 1;  //CAMBIO DE TECLAS
  String texto; //STRING DE TEXTO A ENVIAR
  while(i < 16){  //LLENAR HASTA QUE SEA 16
    LCD.setCursor(i, 1);
    int letra = teclado_alfa.getKey(); //OBTENGO EL VALOR ASCII DE LA LETRA
    if(letra != NO_KEY){
      char t; 
      if(letra == 42){
        vueltas++;
      }
      if(letra == 35){
        vueltas--;
      }
      if(vueltas == 1) {  //LETRAS DE LA A-I
        if(letra == 42 || letra == 35){
        }else if(letra == 48){  //PARA DAR UN ESPACIO
            t = letra - 16;
            LCD.print(t);
            texto+= t;
            i++;
          }else{
          t = letra;
          LCD.print(t);
          texto+=t;
          i++;  
        } //FIN DEL ELSE
      } //FIN DEL IF VUELTAS
      if(vueltas == 2){
        if(letra == 42 || letra == 35){
        }else{
          t = letra + 9;
          LCD.print(t);
          texto+=t;
          i++;  
        } //FIN DEL ELSE
      } //FIN DEL IF VUELTAS
      if(vueltas == 3){
        if(letra == 42 || letra == 35){
        }else{
          t = letra + 18;
          LCD.print(t);
          texto+=t;
          i++;  
        } //FIN DEL ELSE
      } //FIN DEL IF VUELTAS
    } //FIN DEL IF TECLAS
  } //FIN DEL WHILE
  //PEDIR NUMERO
  LCD.clear();  //LIMPIAR LA PANTALLA
  LCD.setCursor(0, 0);
  LCD.print("NUMERO:");
  int n = 0;  //CONTADOR DEL NUMERO
  String num; //STRING DEL NUMERO
  while(n < 10){  //HASTA QUE SEA 10 DIGITOS
    char digito = teclado.getKey();
    if(digito != NO_KEY){
      LCD.setCursor(n, 1);
      LCD.print(digito);
      num+= digito;
      n++; 
    } //FIN DEL IF
  } //FIN DEL WHILE
  //ENVIAR MSJ
  mandarMensaje(const_cast<char*>(num.c_str()), const_cast<char*>(texto.c_str()));
}

void mandarMensaje(char* numero, char* texto) {
  if (sms.SendSMS(numero, texto)) {
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("SMS ENVIADO");
  }
  else {
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("SMS ENVIADO");
  }
}

void recibirMensaje() {
  pos_sms = sms.IsSMSPresent(SMS_UNREAD);
  if (pos_sms) {
    sms.GetSMS(pos_sms, telefono, sms_texto, 100);
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print(telefono);
    LCD.setCursor(0, 1);
    LCD.print(sms_texto);
    delay(2000);
  }
}
