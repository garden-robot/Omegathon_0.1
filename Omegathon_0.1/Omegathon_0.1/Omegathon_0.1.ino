byte hues[8] = {0, 21, 42, 85, 110, 170, 210, 210};
byte team = 0;

bool isPuck = false;

enum comStates {INERT, GO, RESOLVE};
byte comState = INERT;
byte internalState = INERT;



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:


  if (isPuck) {
    puckLoop();
    puckDisplay();

  }

  else {
    fieldLoop();
    // fieldDisplay();
  }



  //begin communication
  byte sendVal = (isPuck << 5) | (team << 2) | (comState);
  setValueSentOnFace(sendVal, 0);
  setValueSentOnFace(sendVal, 3);

}


void puckLoop() {

  if (buttonSingleClicked()) {
    team++;
    if (team == 7) {
      team = 1;
    }
  }
}

void fieldLoop() {
  bool hasBeamNeighbor = false;
  byte changeToBeam;

  if (comState == INERT) {
    setColor(dim(WHITE, 100)); //debug visuals
    if (buttonSingleClicked() && isAlone()) {
      isPuck = true;
      team = random(5) + 1;
    }

    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {
        byte neighborData = getLastValueReceivedOnFace(f);
        if (getIsPuck(neighborData)) {
          if (getComState(neighborData) == INERT) {
            byte neighborTeam = getTeam(neighborData);
            beamDisplay(neighborTeam, f);

          }
        }
        if (getComState(neighborData) == GO) {
          byte neighborTeam = getTeam(neighborData);
          beamDisplay(neighborTeam, f);

        }
      }
    }



  }


}


void puckDisplay() {
  setColor(OFF);
  setColorOnFace(makeColorHSB(hues[team], 255, 255), 0);
  setColorOnFace(makeColorHSB(hues[team], 255, 255), 3);

}

void fieldDisplay() {
  setColor(dim(WHITE, 100));
}

void beamDisplay(byte neighborTeam, int f) {
  setColorOnFace(makeColorHSB(hues[neighborTeam], 255, 255), f);
  setColorOnFace(makeColorHSB(hues[neighborTeam], 255, 255), (f + 3) % 6);
}
bool getIsPuck(byte val) {
  return (val >> 5);
}


byte getTeam(byte val) {
  return ((val >> 2) & 7);
}

byte getComState(byte val) {
  return (val & 3);
}
