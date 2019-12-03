const int gasPin = 1;
const int flamePin = 8;
const int noisePin = 2;

const int noiseSend = 9;
const int gasSend = 10;
const int flameSend = 11;

void setup() {
  pinMode(noisePin, INPUT);
  pinMode(gasPin, INPUT);
  pinMode(flamePin, INPUT);

  pinMode(noiseSend, OUTPUT);
  pinMode(gasSend, OUTPUT);
  pinMode(flameSend, OUTPUT);
}

void loop() {
  int fVal = digitalRead(flamePin);
  int gVal = analogRead(gasPin);
  int nVal = analogRead(noisePin);

  if (fVal == HIGH) {
    digitalWrite(flameSend, HIGH);
  } else {
    digitalWrite(flameSend, LOW);
  }

  if (gVal > 400) {
    digitalWrite(gasSend, HIGH);
  } else {
    digitalWrite(gasSend, LOW);
  }

  if (nVal > 400) {
    digitalWrite(noiseSend, HIGH);
  } else {
    digitalWrite(noiseSend, LOW);
  }

}
