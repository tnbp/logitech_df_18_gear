#include <Joystick.h>

Joystick_ Joystick = Joystick_(0x03, JOYSTICK_TYPE_GAMEPAD, 22, 0, false, false, false, false, false, false, false, false, false, false, false);

short cur_gear = 0;
short last_gear = 0;

short cur_state = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(10, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  Serial.begin(9600);
  Joystick.begin();
  set_joystick_state(0);
}

void loop() {
  unsigned int split = get_split((digitalRead(14) == HIGH), (digitalRead(15) == HIGH));
  cur_gear = position_translate(analogRead(A1), analogRead(A2), digitalRead(10), split);
  if (cur_gear != last_gear) {
    set_joystick_state(cur_gear);
    last_gear = cur_gear;
  }
  delay(10);
}

int position_translate(unsigned int x, unsigned int y, bool r, unsigned int split) {
  unsigned int gear[3][3] = { { 1, 3, 5 }, { 0, 0, 0 }, { 2, 4, 6 } };
  int gx, gy;
  if (x < 350) gx = 0;
  else if (x < 600) gx = 1;
  else gx = 2;
  if (y < 250) gy = 2;
  else if (y < 750) gy = 1;
  else gy = 0;

  unsigned int ret = gear[gy][gx];
  if (!ret) return 0;
  if (r && ret == 6) {
    ret = -1 * split;
  }
  else ret += (split - 1) * 6;
  return ret;
}

unsigned int get_split(bool s1, bool s2) {
  if (s1) return 3;
  if (s2) return 1;
  return 2;
}

void set_joystick_state(int gear) {
  Joystick.setButton(cur_state, false);
  if (gear < 0) cur_state = 22 + gear;  // reverse!
  else cur_state = gear;
  Joystick.setButton(cur_state, true);
}
