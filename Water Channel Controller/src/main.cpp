#include "utils/AsyncFSM.h"
#include "utils/Console.h"
#include <math.h>

#include "model/ButtonImpl.h"
#include "model/Pot.h"
#include "model/ServoImpl.h"
#include "model/LcdImpl.h"

#include "Arduino.h"

#define BUTTON_PIN 8
#define SERVO_PIN 6
#define POT_PIN A0

#define POT_MOVING 2
#define SERIAL_INCOMING 3

const byte numChars = 30;
char receivedChars[numChars];
boolean newData = false;
Pot *pot;

/*
 * La classe ArduinoAsyncFSM permette di creare un handler per la FSM. Questo permette di gestire la logica delle transizioni della FSM
 * Quando viene registrato un interrupt, viene gestita la reazione della macchina a seconda dello stato corrente e del tipo di interrupt.
 * Sono stati individuati tre stati della macchina:
 * - MANUAL: la valvola può essere aperta o chiusa tramite il potenziometro.
 * - AUTOMATIC: la valvola è gestita per mezzo del server principale.
 * - ADMIN: la valvola è gestita dall'admin desktop. In questo caso si è deciso di non permettere alla macchina di modificare lo stato,
 *          per evitare una concorrenza. Similmente quando è impostato lo stato MANUAL, l'admin desktop non potrà modificare lo stato.
 * Sono state individuate le seguenti transizioni:
 * - SERIALE: quando la seriale riceve dei dati, essi vengono analizzati ed eventualmente avviene il cambio di stato.
 * - BOTTONE: quando il bottone viene premuto viene cambiato lo stato da AUTOMATIC a MANUAL e viceversa.
 * - POTENZIOMETRO: quando il potenziometro viene mosso in modalità MANUAL viene cambiato l'angolo della valvola.
 */
class ArduinoAsyncFSM : public AsyncFSM
{

public:
  ArduinoAsyncFSM(Button *button, Console *console, ServoMotor *servo, LcdMonitor *lcd, Pot *pot)
  {
    currentState = AUTOMATIC;
    this->button = button;
    this->servo = servo;
    this->lcd = lcd;
    button->registerObserver(this);
    servo->on();
    lcd->on();
  }

  void handleEvent(Event *ev)
  {
    Serial.print("");
    switch (currentState)
    {
    /**
     *  Nello stato AUTOMATIC posso avere due transizioni: la pressione del bottone e l'arrivo di dati sulla seriale
     *  Nel primo caso si ha un cambiamento di stato, da AUTOMATIC a MANUAL e un invio tramite seriale di questo
     *  cambiamento.
     *  Nel caso della seriale, in questo stato può ricevere solo due tipi di informazione circa lo stato della
     *  macchina: può ricevere che è in stato AUTOMATIC o ADMIN. Nel primo caso la seriale non deve fare nulla,
     *  nel secondo deve cambiare lo stato ad ADMIN.
     */
    case AUTOMATIC:
      handleAutomaticState(ev);
      break;

    /**
     *  Nello stato MANUAL posso avere tre transizioni: la pressione del bottone, l'arrivo di dati sulla seriale
     *  e il movimento del potenziometro.
     *  Nel primo caso si ha un cambiamento di stato, da MANUAL a AUTOMATIC e un invio tramite seriale di questo
     *  cambiamento. .
     *  Nel caso della seriale, in questo permettiamo l'arrivo di solo un tipo di informazione: se infatti
     *  ci dovesse essere il caso in cui il server calcola uno stato di allerta, allora si dovrebbe passare allo
     *  stato automatico per evitare problematica. Lo stato ADMIN è disattivato.
     */
    case MANUAL:
      handleManualState(ev);
      break;

    /**
     *  Nello stato ADMIN la macchina può solo ricevere dati tramite la seriale. La serial può ricevere due tipi
     *  di informazione riguardo allo stato di sistema: può essere ADMIN o AUTOMATIC.
     */
    case ADMIN:
      break;

    default:
      console->log("Errore, stato non riconosciuto");
      break;
    }
    Serial.print("");
    servo->setPosition(angle);
    lcd->write(currentModeToString(), angle);
  }

  void setCurrentMode(char str[])
  {
    if (strcmp(str, "AUTOMATIC") == 0)
    {
      currentState = AUTOMATIC;
    }
    else if (strcmp(str, "MANUAL") == 0)
    {
      currentState = MANUAL;
    }
    else if (strcmp(str, "ADMIN") == 0)
    {
      currentState = ADMIN;
    }
  }

  void setAngle(int angle)
  {
    this->angle = angle;
  }

  bool isManual()
  {
    return currentState == MANUAL;
  }

private:
  Button *button;
  Console *console;
  ServoMotor *servo;
  LcdMonitor *lcd;
  Mode currentState;
  int angle = 0;
  char serialStr[30];

  void handleAutomaticState(Event *ev)
  {
    if (ev->getType() == BUTTON_PRESSED_EVENT)
    {
      transitionToManual();
    }
  }

  void handleManualState(Event *ev)
  {
    if (ev->getType() == BUTTON_PRESSED_EVENT)
    {
      transitionToAutomatic();
    }
    else if (ev->getType() == POT_MOVING)
    {
      angle = pot->getValue();
    }
    sendDataOnSerial();
  }

  void transitionToManual()
  {
    currentState = MANUAL;
    sendDataOnSerial();
  }

  void transitionToAutomatic()
  {
    currentState = AUTOMATIC;
  }

  const char *currentModeToString()
  {
    switch (currentState)
    {
    case MANUAL:
      return "MANUAL";
    case AUTOMATIC:
      return "AUTOMATIC";
    case ADMIN:
      return "ADMIN";
    default:
      return "UNKNOWN";
    }
  }

  const char *intToString(int N)
  {
    static char str[10];
    itoa(N, str, 10);
    return str;
  }

  void sendDataOnSerial()
  {
    strcat(serialStr, currentModeToString());
    strcat(serialStr, "-");
    strcat(serialStr, intToString(angle));
    Serial.write(serialStr);
    Serial.write("\n");
    serialStr[0] = '\0';
  }
};

ArduinoAsyncFSM *fsm;
Console *console;

bool recvWithEndMarker()
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0)
  {
    rc = Serial.read();

    if (rc != endMarker)
    {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
      {
        ndx = numChars - 1;
      }
    }
    else
    {
      receivedChars[ndx] = '\0';
      ndx = 0;
      return true;
    }
  }
  return false;
}

void analizeNewData()
{
  char modeStr[10];
  char *token = strtok(receivedChars, "-");
  if (token != nullptr)
  {
    strcpy(modeStr, token);
    token = strtok(nullptr, "-");
    if (token != nullptr)
    {
      int angle = atoi(token);
      if ((strcmp(modeStr, "MANUAL") == 0 || strcmp(modeStr, "AUTOMATIC") == 0 || strcmp(modeStr, "ADMIN") == 0) &&
          (angle >= 0 && angle <= 1023))
      {
        fsm->setCurrentMode(modeStr);
        fsm->setAngle(angle);
        Event *ev = new Event(SERIAL_INCOMING);
        fsm->notifyEvent(ev);
      }
      else
      {
        // console->log(receivedChars);
        console->log("The message does not conform to the format: MODE-ANGLE");
      }
    }
  }
}

void setup()
{
  Button *button = new ButtonImpl(BUTTON_PIN);
  console = new Console();
  ServoMotor *servo = new ServoMotorImpl(SERVO_PIN);
  LcdMonitor *lcd = new LcdMonitorImpl();
  pot = new Pot(POT_PIN);

  Serial.begin(115200);
  while (!Serial)
  {
  }

  fsm = new ArduinoAsyncFSM(button, console, servo, lcd, pot);
}

void loop()
{
  fsm->checkEvents();
  if (recvWithEndMarker())
  {
    analizeNewData();
  }
  if (fsm->isManual())
  {
    if (pot->isMoving())
    {
      Event *ev = new Event(POT_MOVING);
      fsm->notifyEvent(ev);
    }
  }
}
