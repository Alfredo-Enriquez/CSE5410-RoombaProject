int speakerPin = 2;


int length = 26;
char notes[] = "eeeeeeegcdefffgfeeeeddedg";
int beats[] = { 1,1,2,1,1,2,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2 };
int tempo = 200;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);  
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW); 
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
char names[] = { 'c', 'd', 'e', 'f', 's', 'g', 'a', 'v', 'b', 'C', 'D', 'E' };
int tones[] = { 1915, 1700, 1519, 1432, 1352, 1275, 1136, 1073, 1014, 956, 852, 758 };

  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
void xmas_tunes()
{
  for (int i = 0; i < length; i++) 
  {
    if (notes[i] == ' ') 
    {
      delay(beats[i] * tempo); // rest
    } 
    else 
    {
      playNote(notes[i], beats[i] * tempo);
    }
  }
  
  
}

void setup() {
  pinMode(speakerPin, OUTPUT); 
}

void loop() {
  int sensorVal = digitalRead(10);
  Serial.println(sensorVal);
  xmas_tunes();

}
