#include <U8glib.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

#define GAS_PORT A0
// Tempo de aquecimento
int tempo_aquecimento = 30;
int valor_sensor = 0;
float voltage = 0.0f;
unsigned long time;
int status = 1;
String estado;
int posicao1;
 
void draw()
{
  // Comandos graficos para o display devem ser colocados aqui
  // Seleciona a fonte de texto
  u8g.setFont(u8g_font_8x13B);
  // Mensagens iniciais
  u8g.drawRFrame(0, 18, 128, 46, 4);
  u8g.setPrintPos(10, 15);
  u8g.print("METANO   "+String(voltage, 1)+"V");
  u8g.setFont(u8g_font_ncenB08);
  u8g.drawStr( 10, 37, "Aguarde");
 
  // Animacao caneca
  if (status == 1)
  {
    u8g.drawBox(80, 25, 20, 30);
    u8g.drawHLine(77, 24, 26);
    u8g.drawRFrame(78, 25 , 24, 32, 0);
    u8g.drawRFrame(77, 25 , 26, 32, 0);
    u8g.drawHLine(76, 57, 28);
    u8g.drawHLine(76, 58, 28);
    u8g.drawRFrame(102, 30 , 7, 20, 2);
    u8g.drawRFrame(102, 28 , 9, 24, 2);
    u8g.setColorIndex(0);
    if (time == 0)
    {
      time = 1;
    }
    u8g.drawBox(79, 25, 22, time);
    u8g.setColorIndex(1);
  }
 
  // Apos o aquecimento, exibe dados do sensor
  if (status == 0)
  {
    u8g.setFont(u8g_font_fub20);
    u8g.setColorIndex(0);
    u8g.drawBox(10, 25, 110, 33);
    u8g.setColorIndex(1);
    // Centraliza o valor na tela
    if (valor_sensor <= 99)
    {
      posicao1 = 17;
    }
    else
    {
      posicao1 = 10;
    }
    // Mostra valor do sensor
    u8g.setPrintPos(posicao1, 45);
    u8g.print(String(valor_sensor)+" ppm");
    u8g.setFont(u8g_font_ncenB08);
    // Imprime mensagem no rodape
    int tamanho = estado.length();
    int posicao = (128 / 2 - 3) - ((tamanho * 5) / 2);
    u8g.setPrintPos(posicao, 60);
    u8g.print(estado);
  }
}
 
void setup(void)
{
  Serial.begin(9600);
  pinMode(GAS_PORT, INPUT);
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
}
 
void loop(void)
{
  int sensorValue = analogRead(A1); //read the A1 pin value
  voltage = sensorValue * (5.00 / 1023.00) * 2;
  // Leitura do valor do sensor
  valor_sensor = analogRead(GAS_PORT);
  time = millis() / 1000;
  u8g.firstPage();
  // Tempo de aquecimento
  if (time <= tempo_aquecimento)
  {
    time = map(time, 0, tempo_aquecimento, 0, 30);
    status = 1;
  }
  else
  {
    status = 0;
  }
  estado = "Gas: "+String(valor_sensor);
  Serial.println("Status: "+String(status));
  Serial.println("Time: "+String(time));
  Serial.println(estado);
 
  // Chama a rotina de desenho na tela
  do
  {
    draw();
  }
  while ( u8g.nextPage() );
 
  delay(1000);
}
