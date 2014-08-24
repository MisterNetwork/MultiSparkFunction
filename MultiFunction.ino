/**
  Multi-Function Spark Function  - allows unlimited* Spark Functions
  
  Hacked from the Tinker App
  Also includes Notification to PushingBox and toggle
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/  

int wigValueI = 0;
int wigSwitch;

//Flag on boot/restart only to avoid first-pass activation of notifications/relays
int startflag=0;  

int notifyEnable = 1;
int sleepEnable = 0;


////
//
// General code from http://www.pushingbox.com for Spark Core v1.0
//
////

  /////////////////
 // MODIFY HERE //
/////////////////
// Your secret DevID from PushingBox.com. You can use multiple DevID  on multiple Pin if you want
const char * DEVID1 = "1234567890123456";         // Scenario1 
// add more here


// Debug mode
boolean DEBUG = true;
  //////////////
 //   End    //
//////////////


const char * serverName = "api.pushingbox.com";   // PushingBox API URL
boolean pushingboxState1 = false;             // Save the last state of the Pin for DEVID1

TCPClient client;


/* This function is called once at start up ----------------------------------*/
void setup()
{
    Spark.function("multiwig", multiwidget);
    Spark.variable("startflag", &startflag, STRING);    

    Serial.begin(9600);  
 
 // pin I/O Definitions go here
 
    
    
       // prints out the network parameters over Serial
    if(DEBUG){
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());
        Serial.print("Core IP: ");
        Serial.println(WiFi.localIP());    
        Serial.print("Gateway: ");
        Serial.println(WiFi.gatewayIP());
        Serial.print("Mask: ");
        Serial.println(WiFi.subnetMask());
        Serial.print("WiFi RSSI: ");
        Serial.println(WiFi.RSSI());
    }

}

/* This function loops forever --------------------------------------------*/
void loop()
{

 
//    if (startflag){           //Flag on boot/restart only to avoid first-pass activation of notifications/relays
//                     insert  code here
//    } 

//  --------------  ACTION CONTROL LOGIC
//        Controls LEDs, Relays, and Notifications

//   if (!sleepEnable) {       // disable selected processing
//    }

    
//        
//        if (notifyEnable) {sendToPushingBox(DEVID1);}
//    if ...
//	 pushingboxState1=1;          // notification flag for PushingBox
//    else
//    pushingboxState1=0;  
    
    
    while (client.available()) {
    char c = client.read();
    Serial.print(c);
    }
    
    
    if (!startflag) startflag=1;  // Flag set after first pass
    
}

void sendToPushingBox(const char * devid) 
{
    Serial.print("closing... ");
    client.stop();
    if(DEBUG){Serial.print("connecting... ");}
    if (client.connect(serverName, 80)) {
        if(DEBUG){Serial.println("connected");}
        client.print("GET /pushingbox?devid=");
        client.print(devid);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(serverName);
        client.println("User-Agent: Spark");
        //client.println("Connection: close");
        client.println();
        client.flush();
        if(DEBUG){
            Serial.print("sent! - RSSI:");
            Serial.println(WiFi.RSSI());
            Serial.println("");
        }
    } 
    else{
        if(DEBUG){Serial.println("connection failed");}
    }
}
  


// This function multiwidget gets called whenever there is a matching API request
/*/

Command string format is:   function, value, switch 0/1 or parameter
                              xxxxx,nnn,nnn
 for example: NOTFY,000,1    (PushingBox Notification - ON)
              DREAD,A20,0    (digitalRead pin A2) 
              DWRIT,D20,1    (digitalWrite pin D2 - High/On)
              AWRIT,A20,255  (AnalogWrite pin A2 - 255) 



Functions:
DWRIT  digitalWrite
DREAD  digitalRead
AWRIT  analogWrite
AREAD  analogRead
NOTFY  PushingBox Notification on/off 
SLEEP  Shuts off all processing

.....add your own here!

/*/


int multiwidget(String wigString)
{
    if(DEBUG){Serial.print(">>multiwidget start:<<");
        Serial.println(wigString);}

//find command in string (first 5 characters)
  String wigCommand = String (wigString.substring(0,5)) ;
    if(DEBUG){Serial.print(">wigCommand:");
                Serial.print(wigCommand);}

//find value in string (next 3 characters after comma)
  String wigValueS = (wigString.substring(6,10));
        Serial.print(">wigValueS:");
                Serial.println(wigValueS);
                
// Check command start letter for D or A to prevent converting first character of value to a number
  if (wigCommand.charAt(0) != 'D' && wigCommand.charAt(0) != 'A') 
      if(DEBUG){Serial.print(">>Command Not D or A:");}
     int wigValueI = wigValueS.toInt();
        if(DEBUG){Serial.print(">wigValueI:");
                 Serial.println(wigValueI);}

//find switch value in string and convert ascii to integer

    String wigSwitchS = (wigString.substring(10,13));
    wigSwitch = wigSwitchS.toInt();

    if(DEBUG){Serial.print(">wigSwitch:");
        Serial.print(wigSwitch);
        Serial.print(">wigSwitchS:");
                Serial.println(wigSwitchS);}


// Digital Write    -------------------------------------------------------------Digital Write DWRIT 
   if (wigCommand == "DWRIT")   // Digital Write
   {
       if (DEBUG){Serial.println("DWRIT:");}
    
       	//convert ascii to integer
	int pinNumber = wigValueS.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(wigValueS.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, wigSwitch);
		return 1;
	}
	else if(wigValueS.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, wigSwitch);
		return 1;
	}
	
   }
    else   

// Digital Read     -------------------------------------------------------------Digital Read DREAD
   if (wigCommand == "DREAD")  
    {
    
    if (DEBUG){Serial.println("DREAD:");}
    
    //convert ascii to integer
	int pinNumber = wigValueS.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(wigValueS.startsWith("D"))
	{
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);
	}
	else if (wigValueS.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);
	}
	
   }
    
    else   
    
// Analog Read      -------------------------------------------------------------Analog Read AREAD
   if (wigCommand == "AREAD")
     {  
    
    if (DEBUG){Serial.println("AREAD:");}
    
 	//convert ascii to integer
	int pinNumber = wigValueS.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(wigValueS.startsWith("D"))
	{
		pinMode(pinNumber, INPUT);
		return analogRead(pinNumber);
	}
	else if (wigValueS.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT);
		return analogRead(pinNumber+10);
	}        
    
    }
    else      

// Anlog Write      -------------------------------------------------------------Analog Write AWRIT
    if (wigCommand == "AWRIT")
      {

    if (DEBUG){Serial.println("AWRIT:");}

 	//convert ascii to integer
	int pinNumber = wigValueS.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

    if(DEBUG){ 
       Serial.print(">pinNumber:");
        Serial.print(pinNumber);
       Serial.print(">wigSwitch:");
        Serial.print(wigSwitch);
    }


	if(wigValueS.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, wigSwitch);
		return 1;
	}
	else if(wigValueS.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, wigSwitch);
		return 1;
	}          

      }
    else    
    
// PushingBox Notification toggle  --------------------------------------------PushingBox Notify  NOTFY
    if (wigCommand == "NOTFY")   
    {
     if (DEBUG){Serial.print(">NOTFY:");}    
    notifyEnable = wigSwitch; 
    }  
      
    else   
    
// Shuts off all processing   ---------------------------------------------------------SLEEP
if (wigCommand == "SLEEP")   
      {
     if (DEBUG){Serial.print(">SLEEP:");}    
     sleepEnable = wigSwitch; 
      
      }

    else 
    
   return -1;
}   
