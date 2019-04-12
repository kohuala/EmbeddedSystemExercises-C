// Guan, Huihua
// Embedded System Lab 2: Analog IO and Timer Excercise


// initialize the pointers to registers
  unsigned char* DDREGD;          			 // for pin 5, it is in PORTD
  unsigned char* REGA;            			 // set up PWM
  unsigned char* REGB;           			   // set up PWM
  unsigned char* OCA;             			 // reading top value
  unsigned char* OCB;             			 // regarding duty cycle
  unsigned char* TIMER_0B;         		  // pin 5 connects to OC0B

  // setting up analog input
  unsigned char* DDREGC;          			// for pin 0, it is in PORTC
  unsigned char* ADMUXREG;			// init ptr to ADMUX register
  unsigned short* ADCData;     			// init ptr to ADC data register
  unsigned char* ADCSRAREG;			// init ptr to ADCSRA register
  unsigned char* ADCSRBREG; 			// init ptr to ADCSRB register
  unsigned char* DIDR0REG;			// init ptr to DIDR0 register

void setup() {
  // setup code here, to run once:
  Serial.begin(9600);

  // pointers to registers to address
  TIMER_0B = (unsigned char*) 0x45;		// assign address to TIMER0B
  
  DDREGD = (unsigned char*) 0x2A;		// assign address to DDRD register
  REGA = (unsigned char*) 0x44;			// assign address to REGA register
  REGB = (unsigned char*) 0x45;			// assign address to REGB register
  OCA = (unsigned char*) 0x47;			// assign address to OCA register
  OCB = (unsigned char*) 0x48;			// assign address to OCB register

  ADMUXREG = (unsigned char*) 0x7C;		// assign address to ADMUX register
  ADCSRAREG = (unsigned char*) 0x7A;		// assign address to ADCSRA register
  ADCSRBREG = (unsigned char*) 0x7B;		// assign address to ADCSRB register
  ADCData = (unsigned short*) 0x78;		// assign address to ADC data register
  DIDR0REG = (unsigned char*) 0x7E;		// assign address to DIDR0 register

  //unsigned short ADCVal;
}

void loop() { 
  // setting up the analog signal: 1Hz sine 3V
  float aval;
  long x;
  x = millis();
  aval =abs(3 * sin(2 * 3.141592654 * x / 1000));
  
  delay(1);
//  Serial.println(aval);

   // setting up the PWM signal
   // PS = 64
   // TOP = 249 for 1000 Hz
   // WGM -> Fast PWM
 
 *DDREGD = _BV(5);                             
  *REGA = _BV(0) | _BV(1) | _BV(5) | _BV(7);    		// 10100011
  *REGB = _BV(1) | _BV(3) | _BV(0);             		// 00001011
  *OCA = 249;                                  			// Value to get 1000Hz+-1Hz
  *OCB = 125;                                   			// For better visibility, 50% duty cycle

  int new_aval;
  new_aval=int(aval*83);					// scaling from 0-3V, to 0-255
//  Serial.println(int(aval*83));
  *OCB=new_aval;
//  Serial.println(new_aval);

  // setting up analog input
  *DDREGC = *DDREGC & ~_BV(0);						// set A0 as input
  *ADMUXREG =  _BV(6);     						// right justified and Vcc
  *ADCSRAREG = _BV(1) | _BV(2) | _BV(6) | _BV(5) | _BV(7);    	// add the prescaler 64
  *ADCSRBREG = _BV(2); 							//  enable MUX, free running
  *ADCData = _BV(6);   							//
  *DIDR0REG =_BV(0); 							// disable pin access to A0

  unsigned short ADCVal;
  ADCVal=(*ADCData & 0x3FF);						// read

  float fADCVal;
  fADCVal=((float)ADCVal)/1023 * 3; 

 // Serial.print(abs(aval)); 					// Original rectified sinusoid
 // Serial.print(" ");
  Serial.println(fADCVal); 					// Analog voltage measured from ADC
  }





