#include <stdlib.h>

//#define ___DEBUG
#define NTRI 4
typedef struct
{
  int ID; //1, 2, 3
  int pinID; //22, 24, 67...
  int onSetTms;
} tTriggerPin;

char inData[256]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
bool newMsg = false;
char msgBuf[256];

tTriggerPin triggers[4];

int trigDelay[4];
int trigOutputPin[4] = {22, 24, 26, 28};

void processSet();


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i=0; i<4; i++)
  {
    triggers[i].pinID = 22 + 2*i;
    triggers[i].ID = i+1;
    triggers[i].onSetTms = 0;
    pinMode(triggers[i].pinID, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()>0)
  {
    inChar = Serial.read(); //read a character
    inData[index] = inChar;
    if(inData[index] =='\n')
    {
      newMsg = true;
      inData[index+1] = '\0';
      break;
    }
    index++;
  }

  //set 0 100 200 300\n (set the delays for trig outputs 1, 2, 3, 4, to 0, 100, 200, 300 ms)
  //trg\n (trigger all outputs now)
  if(newMsg)
  {
    index = 0;
    newMsg = false;
    //
    if(strncmp(inData, "set", 3) ==0)
    {
      Serial.print("set delay ack\n");
      processSet();
      rankingByOnsetT();
    }
    else if(strncmp(inData, "trg", 3) ==0)
    {
        Serial.print("trg ack\n");
        processTrg();
    }
    else
      Serial.print("Unknown message\n");
  }
  delay(10);
}


void processSet()
{
  char digBuf[10];
  char *ind = inData;
  int trigInd = 0;
  //Serial.print(inData);
  while( *ind != '\n') //do not compare with '\0'
  {
    //trim ' ' and non-digital chars
    while(*ind <'0' || *ind >'9')
      ind++;

    int digInd = 0;
    while(*ind >='0' && *ind <='9')
    {
        digBuf[digInd++] = *ind;
        ind++;
    }

    digBuf[digInd] = '\0';
    if(digInd > 0)
    {
      triggers[trigInd++].onSetTms = atoi(digBuf);
      digInd = 0;
    }
  }
}

void processTrg()
{
  int sameTriggerStartInd = 0;
  int sameTriggerStopInd = 0;
  digitalWrite(triggers[0].pinID, HIGH); //start first trigger
  for (int i = 1; i < NTRI; i++)
  {
    int deltaOnSetT = triggers[i].onSetTms - triggers[i - 1].onSetTms;
    if (deltaOnSetT >= 1)
    {
      //Current trigger has different onset with current-1 trigger
      delay(1);
      //shutdown all the prevous triggers
      for (int k = sameTriggerStartInd; k < i; k++)
        digitalWrite(triggers[k].pinID, LOW);

      delay(deltaOnSetT - 1);
      digitalWrite(triggers[i].pinID, HIGH);
      sameTriggerStartInd = i;
      sameTriggerStopInd = i;
    }
    else
    {
      //same onSet trigger
      sameTriggerStopInd = i;
      digitalWrite(triggers[i].pinID, HIGH);
    }
  }
  delay(1);

  for (int i = sameTriggerStartInd; i <= sameTriggerStopInd; i++)
  {
    digitalWrite(triggers[i].pinID, LOW);
  }
}

void rankingByOnsetT()
{
  for (int i = 0; i < NTRI - 1; i++)
  {
    for (int j = 0; j < NTRI - 1 - i; j++)
    {
      if (triggers[j].onSetTms > triggers[j + 1].onSetTms)
      {
        tTriggerPin temp = triggers[j + 1];
        triggers[j + 1] = triggers[j];
        triggers[j] = temp;
      }
    }
  }

#ifdef ___DEBUG
  for (int i=0; i<4; i++)
  {
    sprintf(msgBuf, "Trig ID = %d, pin ID = %d, delay = %d ms\n",
      triggers[i].ID, triggers[i].pinID, triggers[i].onSetTms);
    Serial.print(msgBuf);
  }
 #endif
}
