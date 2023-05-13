#include <ESP32Servo.h>
#include "src/genetic_alg_lib/genetic_alg_lib.hpp"
#include "src/referencia_lib/referencia_lib.hpp"

// Pinos
#define PIN_MOTOR1 25
#define PIN_MOTOR2 33
#define PIN_ENCODER 22
#define PIN_FREIO 32

// PWM
const int CANAL_PWM = 1;
const int FREQUENCIA_PWM = 1000;
const int RESOLUCAO_PWM = 10;

// Parametros Sensor
const float TA = 0.08;
const int TA_MS = 80;
const float MIN_PWM = 0;
const float MAX_PWM = pow(2, RESOLUCAO_PWM) - 1;
const int PULSOS_POR_VOLTA = 3;
const float COEFICIENTE_VELOCIDADE = ((1 / TA) * 60) / PULSOS_POR_VOLTA; //(Ta[ms] * coeficienteRPM) / PulsosPorVolta
const int VELOCIDADE_MAX = 9750;

// Variaveis Globais
unsigned long PULSOS_ENCODER = 0;

// Controladores FUZZY
const int TAMANHO_POPULACAO = 20;
GAsandbox SIMULACAO(TAMANHO_POPULACAO, MIN_PWM, MAX_PWM, 10); //10% chance mutar

//Freio
Servo FREIO;
const int ANGULO_MAX = 115;
const int ANGULO_MIN = 90;

double SOMATORIO_EK = 0;
double SOMATORIO_UK = 0;

// Referencia
Referencia REFERENCIA(0, VELOCIDADE_MAX, degrau_unitario);

// Funcoes
void IRAM_ATTR interrupcao_encoder() { PULSOS_ENCODER++; }
float captura_velocidade();
void imprimi_informacoes(float velocidadeAtual, float sinalControle, float referencia, bool monitorSerial);
void roda_por_T(int T, int indice);

void setup()
{
  // SERIAL
  Serial.begin(115200);

  // PINOS
  pinMode(PIN_ENCODER, INPUT_PULLUP);
  pinMode(PIN_MOTOR2, OUTPUT);
  digitalWrite(PIN_MOTOR2, LOW);

  // PWM
  ledcAttachPin(PIN_MOTOR1, CANAL_PWM);
  ledcSetup(CANAL_PWM, FREQUENCIA_PWM, RESOLUCAO_PWM);
  ledcWrite(CANAL_PWM, MAX_PWM - MIN_PWM);

  //FREIO
  FREIO.attach(PIN_FREIO);
  FREIO.write(ANGULO_MIN);

  // //INTERRUPCAO
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER), interrupcao_encoder, FALLING);

  // CONTROLADOR

  // REFERENCIA
  REFERENCIA.velocidade = 0.5;

  delay(1000);

  while(SIMULACAO.numeroGeracao < 50)
  {
    for(int i = 0; i < SIMULACAO.tamanhoPopulacao; i++)
    {
      Serial.println("GERACAO N: " + String(SIMULACAO.numeroGeracao) + " Individuo N: " + (i+1));
      roda_por_T(2, i);
      SIMULACAO.populacao[i]->somatorioEk = SOMATORIO_EK;
      SIMULACAO.populacao[i]->somatorioUk = SOMATORIO_UK;
    }

    SIMULACAO.iteracao();
  }
}

void loop()
{
}

void freiar()
{
  FREIO.write(ANGULO_MAX);
  while(captura_velocidade() > 0)
    delay(20);
  delay(50);
  FREIO.write(ANGULO_MIN);
}

float captura_velocidade()
{
  PULSOS_ENCODER = 0; // reseta contador
  delay(80);          // conta TA_S de pulsos
  return (COEFICIENTE_VELOCIDADE * PULSOS_ENCODER);
}

void imprimi_informacoes(float velocidadeAtual, float sinalControle, float referencia, bool monitorSerial)
{
  Serial.print("Uk ");
  Serial.print(sinalControle);
  Serial.print(" Max ");
  Serial.print(VELOCIDADE_MAX);
  Serial.print(" Min ");
  Serial.print(0);
  Serial.print(" Referenncia ");
  Serial.print(referencia);
  Serial.print(" Atual ");
  Serial.println(velocidadeAtual);
}

void roda_por_T(int T, int indice)
{
  SOMATORIO_EK = 0;
  SOMATORIO_UK = 0;
  for (int i = 0; i < (int)(T / TA); i++)
  {
    float velocidadeAtual = captura_velocidade();

    float sinalControle = SIMULACAO.populacao[indice]->Calcula_Acao_Controle(REFERENCIA.modulo, velocidadeAtual);
    ledcWrite(CANAL_PWM, MAX_PWM - sinalControle);

    // imprimi_informacoes(velocidadeAtual, sinalControle, REFERENCIA.modulo, false);

    REFERENCIA.atualiza_referencia();

    SOMATORIO_EK += REFERENCIA.modulo - velocidadeAtual;
    SOMATORIO_UK += sinalControle;
  }
  ledcWrite(CANAL_PWM, MAX_PWM - MIN_PWM);
  delay(100);

  if(captura_velocidade() > 0)
    freiar();
}