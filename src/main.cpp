#include <Arduino.h>

float K = 0.5;
int averegespeed = 70;
int N = 0;
int minIRL = 400;
int maxIRL = 600;
int minIRR = 400;
int maxIRR = 600;

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
}

void console(int x, int y)
{
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
}

void go(int r, int l)
{
  digitalWrite(2, r > 0 ? 1 : 0);
  digitalWrite(4, l > 0 ? 1 : 0);
  analogWrite(3, abs(r));
  analogWrite(5, abs(l));
}

void norm(int& x, int& y)
{
  if (x < minIRL)  {    minIRL = x;  };
  if (x > maxIRL)  {    maxIRL = x;  };
  if (y < minIRR)  {    minIRR = y;  };
  if (y > maxIRR)  {    maxIRR = y;  };
 x=map(x,minIRL,maxIRL,0,1000);
  y=map(y,minIRR,maxIRR,0,1000);
}

void preg()
{
  int d1 = analogRead(A0);
  int d2 = analogRead(A1);
  norm(d1, d2);
  int E = d1 - d2;
  float corE = E * K;

  int R = averegespeed + corE;
  R = constrain(R, -250, 250);
  int L = averegespeed - corE;
  L = constrain(L, -250, 250);
  go(R, L);
  console(R, L);
}

void loop()
{
  if (analogRead(A0) < 500 && analogRead(A1) < 500)
  {
    go(averegespeed, averegespeed);
    delay(200);
    go(0, 0);
    delay(200);
    N = N + 1;
    if (N == 3)
    {
      go(0, 0);
      while (1)
        ;
    }
  }
  else
  {
    preg();
  };

  // go(200,-200);
  // preg();
}
