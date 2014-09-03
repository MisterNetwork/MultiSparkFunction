Multi Spark Function
=========================
##By Bob "Dawgs" Blaustein

   bob@misternetwork.com


##Allows more than 4 spark functions with predefined spark function.  
Add more functions as you need.
Also integrated with PushingBox and notification toggle.

Function "mutiwig"  (Multi-Widget)
Command string format is:   

multiwig (Function, value, switch 0/1 or parameter:)
xxxxx,nnn,nnn
For example: 

* NOTFY,000,1    (PushingBox Notification - ON)
* DREAD,A20,0    (digitalRead pin A2) 
* DWRIT,D20,1    (digitalWrite pin D2 - High/On)
* AWRIT,A20,255  (AnalogWrite pin A2 - 255) 



Built-in Functions:
* DWRIT  digitalWrite
* DREAD  digitalRead
* AWRIT  analogWrite
* AREAD  analogRead
* NOTFY  PushingBox Notification on/off 
* SLEEP  Shuts off all processing
* add your own as you need...

* Based partly on the original "Tinker" firmware


