#include <fB_flatBrain.h>
#include <fB_Relay.h>
#include <Wire.h>
#include <Timer.h>


#define CS1		0x01	
#define CS2		0x02
#define CS3		0x04	

#define RS1		0x01	
#define RS2		0x02
#define RS3		0x04	
#define RS4		0x08	
#define RS5		0x10	
#define RS6		0x11

#define EXISTVOLTS				10.0
#define INITCHARGEVOLTS1		13.0
#define INITCHARGEVOLTS2		13.0
#define LOWVOLTS1				12.5
#define LOWVOLTS2				11.5
#define PWRVOLTS				13.8

Timer timer;

fB_Relay *R1, *R2, *R3,*R4, *R5, *R6;

float v1, v2, v3;
uint8_t   Cstate;
uint8_t   Rstate;
uint8_t changeFlag = 0;



void updateDisplay() {
		R1->updateState();
		R2->updateState();
		R3->updateState();
		R4->updateState();
		R5->updateState();
		R6->updateState();
		R1->updateVolts();
		R2->updateVolts();
		//R3->updateVolts();
		updateBusVolts();
		//updateState();
}
void updateState() {

	if(R1->state == ON) Rstate |= RS1; else Rstate &= ~RS1;
	if(R2->state == ON) Rstate |= RS2; else Rstate &= ~RS2;
	if(R3->state == ON) Rstate |= RS3; else Rstate &= ~RS3;
	if(R4->state == ON) Rstate |= RS4; else Rstate &= ~RS4;
	if(R5->state == ON) Rstate |= RS5; else Rstate &= ~RS5;
	if(R6->state == ON) Rstate |= RS6; else Rstate &= ~RS6;
	v1 = R1->Volts;
	v2 = R2->Volts;

	if(v1 > PWRVOLTS){
		if(!(Rstate & RS1) || !(Rstate & RS2 )) Cstate |= CS1; 
	}
	else Cstate &= ~ CS1; 
	
	if(v2 > PWRVOLTS){
		if(!(Rstate & RS2) || !(Rstate & RS1 )) Cstate |= CS2; 
	}
	else Cstate &= ~ CS2; 

}

void initState() {
	R1->turn(OFF);
	R2->turn(ON);
	R3->turn(OFF);
}


void setup(){

   Serial.begin(9600);
  
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);


	//alarm.disable();
	alarm.bootBeepDisable();

	flatBrainInit();
	menu.showPage(RSTATUS);

	//i2c.scan();
	//Card(YCRD)->LED(HIGH);
	R1 = new fB_Relay(Y1,Y1S,V1,RSTATUS);
	R2 = new fB_Relay(Y2,Y2S,V2,RSTATUS);
	R3 = new fB_Relay(Y3,Y3S,V3,RSTATUS);
	R4 = new fB_Relay(Y4,Y4S,NULL,RSTATUS);
	R5 = new fB_Relay(Y5,Y5S,NULL,RSTATUS);
	R6 = new fB_Relay(Y6,Y6S,NULL,RSTATUS);
	Tag(V0)->dVal->value = 0;
	Tag(V1)->dVal->value = 0;
	Tag(V2)->dVal->value = 0;
	Tag(V3)->dVal->value = 0;

	timer.every(2000,updateDisplay);
	//initState();

}



void loop() {
	menu.checkButtonCode();
	
	if(curr.pageTag== RSTATUS) timer.update();
/*
	if(Cstate) {
		if(Cstate & CS1) {  // main charge power
			if(v2 < INITCHARGEVOLTS2) {
				R1->turn(ON);
				R2->turn(ON);
			}
		}
		else if(Cstate & CS2) {  
			if(v1 < INITCHARGEVOLTS1) {
				R2->turn(ON);
				R1->turn(ON);
			}
		}
	}
	
	else { // not charging from external source
		if(v2 < LOWVOLTS2 &&  Rstate & RS2) {
			R4->turn(OFF);
			updateState();
			if(v2 < LOWVOLTS2) R6->turn(OFF);
			updateState();
			if(v2 < LOWVOLTS2) R2->turn(OFF);
		}
		if(v1 < LOWVOLTS1 &&  Rstate & RS1) {
			if(v2 > LOWVOLTS2) {
				R2->turn(ON);
				R1->turn(OFF);
			}
			else {
				R4->turn(OFF);
				updateState();
				if(v1 < LOWVOLTS2) R6->turn(OFF);
				updateState();
				if(v1 < LOWVOLTS2) R1->turn(OFF);
			}
		}
	}
*/
}


 void updateBusVolts() {
	static double oldVolts = 0;
	fB_Tag* pV;
	pV = Tag(V0);
	pV->read();
//dbug(F("uVS %P val:%f"),pV->Ptitle,pV->dVal->value);
	if(pV->dVal->value <0) pV->dVal->value = 0;
	if((fabs(oldVolts - pV->dVal->value)> 0.1) && curr.pageTag == RSTATUS) menu.refreshRow(V0);
	oldVolts = pV->dVal->value;

 }

