#define led_white 10
#define led_yellow 11
#define led_blue 12
void setup()
{
  // put your setup code here, to run once:
  pinMode(led_white, OUTPUT);
  analogWrite(led_white, 255);
  pinMode(led_yellow, OUTPUT);
  analogWrite(led_yellow, 255);
  pinMode(led_blue, OUTPUT);
  analogWrite(led_blue, 255);
  Serial.begin(9600);
  Serial.write("KDE Neon 12");
}

void config_led(char led_id, int led_brightness)
{
  switch (led_id)
  {
    case 'w':
      analogWrite(led_white, led_brightness);
      break;
    case 'y':
      analogWrite(led_yellow, led_brightness);
      break;
    case 'b':
      analogWrite(led_blue, led_brightness);
      break;
    default:
      break;
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    char led_specifier = Serial.read();
    int led_brightness = Serial.parseInt();
    config_led(led_specifier, led_brightness);
  }

}
