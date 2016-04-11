#include<Keypad.h>
const byte filas = 4;
const byte columnas = 3;
byte pinesFilas[filas] = {13, 12, 11, 10};
byte pinesColumnas[columnas] = {9, 8, 7};

char teclas[filas][columnas] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);
char tecla;
int count = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  tecla = teclado.getKey();
  if (tecla != NO_KEY) {
    letra(count,tecla);
    count++;
  }
}

void letra(int veces, char l) {
  switch (veces) {
    case 0:
      switch(l){
        case '1':
          Serial.print('A');
          break;
        case '2':
          Serial.print('D');
          break;
        case '3':
          Serial.print('G');
          break;
        case '4':
          Serial.print('J');
          break;
      }
      break;

    case 1:
      switch(l){
        case '1':
          Serial.print('B');
          break;
        case '2':
          Serial.print('E');
          break;
        case '3':
          Serial.print('H');
          break;
        case '4':
          Serial.print('K');
          break;
      }
      break;

    case 2:
      switch(l){
        case '1':
          Serial.print('C');
          break;
        case '2':
          Serial.print('F');
          break;
        case '3':
          Serial.print('I');
          break;
        case '4':
          Serial.print('L');
          break;
      }
      break;

    case 3:
      Serial.print(l);
      break;
    default:
      count = -1;
      break;
  }
}
