#define RELE_RUA 18
#define RELE_GERADOR 19
#define RELE_COMBUSTIVEL 16
#define RELE_PARTIDA 17
#define MONITOR_RUA 14
#define MONITOR_GERADOR 15
#define BOTAO_MODO 2
#define BOTAO_RELE_RUA 22
#define BOTAO_RELE_GERADOR 24
#define BOTAO_COMANDO_PARTIDA 26

unsigned long millisLiberacaoCombustivel = 0;
unsigned long millisInicioPartida = 0;
unsigned long millisEsperaEntrePartida = 0;
bool combustivelLiberado = false;
bool partidaLigada = false;
bool geradorLigado = false;
int tentativasPartida = 0;

void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(RELE_RUA, OUTPUT);
  pinMode(RELE_GERADOR, OUTPUT);
  pinMode(RELE_COMBUSTIVEL, OUTPUT);
  pinMode(RELE_PARTIDA, OUTPUT);
  pinMode(MONITOR_RUA, INPUT);
  pinMode(MONITOR_GERADOR, INPUT);
  pinMode(BOTAO_MODO, INPUT);
  pinMode(BOTAO_RELE_RUA, INPUT);
  pinMode(BOTAO_RELE_GERADOR, INPUT);
  pinMode(BOTAO_COMANDO_PARTIDA, INPUT);
}

void loop() {
  unsigned long agora = millis();

  // MODO MANUAL
  if (digitalRead(BOTAO_MODO)) {
    Serial.println("MODO MANUAL");
    // Só lê os estados dos três botões
    int valBotaoRua = digitalRead(BOTAO_RELE_RUA);
    int valBotaoGerador = digitalRead(BOTAO_RELE_GERADOR);
    int valBotaoComandoPartida = digitalRead(BOTAO_COMANDO_PARTIDA);
    int valMonitorRua = digitalRead(MONITOR_RUA);
    int valMonitorGerador = digitalRead(MONITOR_GERADOR);

    if (valMonitorGerador) {
      geradorLigado = true;
    } else {
      geradorLigado = false;
    }

    // Aciona os relês de acordo com o estado dos botões
    digitalWrite(RELE_RUA, valBotaoRua);
    digitalWrite(RELE_GERADOR, valBotaoGerador);

    // Só vai passar se o botão da partida estiver ligado
    if (valBotaoComandoPartida) {
      if (!combustivelLiberado) {
        millisLiberacaoCombustivel = millis(); // Atualiza o valor assim que o botão é pressionado
        digitalWrite(RELE_COMBUSTIVEL, HIGH);
        combustivelLiberado = true; // Depois, atualiza o estado
      }

      if (geradorLigado) {
        digitalWrite(RELE_PARTIDA, LOW);
        partidaLigada = false;
      } else {
        if (tentativasPartida < 3) {

          if (!partidaLigada) {
            if ((tentativasPartida == 0 || (agora - millisEsperaEntrePartida) > 10000) && (agora - millisLiberacaoCombustivel) > 1000) {
              millisInicioPartida = millis();
              digitalWrite(RELE_PARTIDA, HIGH);
              partidaLigada = true;
            }
          } else {
            // Mantém a partida ligada durante 3 segundos e desliga após
            if ((agora - millisInicioPartida) > 3000) {
              digitalWrite(RELE_PARTIDA, LOW);
              partidaLigada = false;
              millisEsperaEntrePartida = millis();
              tentativasPartida++;
            }
          }

        }
      }
      // Espera um segundo depois da liberação de combustível antes de dar a partida
    } else { // Caso o botão seja desligado, resetar os parâmetros e desliga os relês
      millisLiberacaoCombustivel = 0;
      millisInicioPartida = 0;
      millisEsperaEntrePartida = 0;
      digitalWrite(RELE_COMBUSTIVEL, LOW);
      digitalWrite(RELE_PARTIDA, LOW);
      combustivelLiberado = false;
      partidaLigada = false;
      tentativasPartida = 0;
    }
  } else { // MODO AUTOMATICO
    Serial.println("MODO AUTOMATICO");
    static bool ruaEstavel = true;
    static unsigned long millisUltimaQuedaEnergia = 0;
    int valMonitorRua = digitalRead(MONITOR_RUA);
    int valMonitorGerador = digitalRead(MONITOR_GERADOR);

    if (valMonitorGerador) {
      geradorLigado = true;
    } else {
      geradorLigado = false;
    }

    if (valMonitorRua) {
      if (ruaEstavel) {
        chavearParaRua(agora);
        digitalWrite(RELE_COMBUSTIVEL, LOW);
        digitalWrite(RELE_PARTIDA, LOW);
      } else {
        if ((agora - millisUltimaQuedaEnergia) > 5000) {
          ruaEstavel = true;
        }
      }
    } else {
      ruaEstavel = false;
      millisUltimaQuedaEnergia = agora;
      digitalWrite(RELE_RUA, LOW);

      // O combustivelLiberado serve pra garantir que o gerador não esteja sendo ligado por um fantasma
      if (geradorLigado && combustivelLiberado) {
        chavearParaGerador(agora);
        digitalWrite(RELE_PARTIDA, LOW);
        partidaLigada = false;
        tentativasPartida = 0;
      } else {

        if (!combustivelLiberado) {
          millisLiberacaoCombustivel = agora; // Atualiza o valor assim que o botão é pressionado
          digitalWrite(RELE_COMBUSTIVEL, HIGH);
          combustivelLiberado = true; // Depois, atualiza o estado
        }

        if (tentativasPartida < 3) {
          if (!partidaLigada) {
            if ((agora - millisLiberacaoCombustivel) > 1000 && (tentativasPartida == 0 || (agora - millisEsperaEntrePartida) > 10000)) {
              millisInicioPartida = agora;
              digitalWrite(RELE_PARTIDA, HIGH);
              partidaLigada = true;
            }
          } else {
            // Mantém a partida ligada durante 3 segundos e desliga após
            if ((agora - millisInicioPartida) > 3000) {
              digitalWrite(RELE_PARTIDA, LOW);
              partidaLigada = false;
              millisEsperaEntrePartida = agora;
              tentativasPartida++;
            }
          }
        }
      }
    }
  }
}

void chavearParaRua(unsigned long millisAgora) {

  static unsigned long millisGeradorDesligado = millis();
  digitalWrite(RELE_GERADOR, LOW);

  if ((millisAgora - millisGeradorDesligado) > 500) {
    digitalWrite(RELE_RUA, HIGH);
  }
}

void chavearParaGerador(unsigned long millisAgora) {

  static unsigned long millisRuaDesligado = millis();
  digitalWrite(RELE_RUA, LOW);

  if ((millisAgora - millisRuaDesligado) > 500) {
    digitalWrite(RELE_GERADOR, HIGH);
  }
}