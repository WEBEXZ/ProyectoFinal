#include<Keypad.h>
const byte filas = 4;
const byte columnas = 3;
byte pinesFilas[filas] = {13,12,11,10};
byte pinesColumnas[columnas] = {9,8,7};

char teclas[filas][columnas] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}  
};

Keypad teclado = Keypad(makeKeymap(teclas),pinesFilas,pinesColumnas, filas, columnas);
char tecla;

void setup() {
  Serial.begin(9600);
}

void loop() {
  tecla = teclado.getKey();
  if(tecla != NO_KEY){
    Serial.print(tecla);
  }
}
