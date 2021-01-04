#define PUMP 2
#define SENSOR1 3
#define SENSOR2 4
#define DEBUG 1

int sensor1 = 1;
int sensor2 = 1;
int pump = 0;

bool sensorEmpty(int sensor) {
  if (sensor == SENSOR1) {
      return sensor1 == 1;
  } else {
      return sensor2 == 1;
  }
}

void updateData() {
  sensor1 = digitalRead(SENSOR1);
  sensor2 = digitalRead(SENSOR2);
  pump = digitalRead(PUMP);
}

void setPump(bool on) {
  if (on) {
    digitalWrite(PUMP, HIGH);
  } else {
    digitalWrite(PUMP, LOW);
  }
}

bool hasToChangePump(bool on) {
  if(on) {
    return sensorEmpty(SENSOR1) && sensorEmpty(SENSOR2);
  } else {
    return !sensorEmpty(SENSOR1) && !sensorEmpty(SENSOR2);
  }
}

bool confirmChangePump(int times, bool on) {
  delay(1000);
  updateData();
  if(times <= 1) {
    return hasToChangePump(on);
  }
  return hasToChangePump(on) && confirmChangePump(times - 1, on);
}

void debug() {
  if(DEBUG) {
    Serial.print("Sensor 1: ");
    Serial.print(sensorEmpty(SENSOR1));
    Serial.print(", Sensor 2: ");
    Serial.print(sensorEmpty(SENSOR2));
    Serial.print(", Bomba: ");
    Serial.print(pump);
    Serial.println();
  }
}

void setup() {
    if(DEBUG) {
      Serial.begin(9600);
    }
    pinMode(SENSOR1, INPUT_PULLUP);
    pinMode(SENSOR2, INPUT_PULLUP);
    pinMode(PUMP, OUTPUT);
}

void loop() {
  updateData();
  debug();
  if(hasToChangePump(!pump)) {
    if(confirmChangePump(5, !pump)) {
      setPump(!pump);
    }
  }
  delay(1000);
}
