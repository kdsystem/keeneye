//by sms - https://www.youtube.com/watch?v=mvfTxRGTHWc
//https://github.com/mewanindula/GPS-Tracking-System
// board manual - http://www.fut-electronics.com/wp-content/uploads/2016/06/SIM808_GPS_GSM_manual.pdf
// all about http://acoptex.com/project/264/basics-project-053d-sim808-gsm-gprs-gps-bluetooth-evolution-board-evb-v32-at-acoptexcom/#sthash.mO6WPDNK.dpbs
//https://seeeddoc.github.io/Mini_GSM-GPRS_Plus_GPS_Breakout-SIM808/
//https://wiki.eprolabs.com/index.php?title=SIM_808_GSM_GPRS_Module
//

AT+SAPBR=3,1,"CONTYPE","GPRS"
OK
AT+SAPBR=3,1,"APN","internet.mts.ru"
OK
AT+SAPBR=3,1,"USER","mts"
OK
AT+SAPBR=3,1,"PWD","mts"
OK
AT+SAPBR=1,1
OK





AT+HTTPINIT
OK
AT+HTTPPARA="CID",1
OK
AT+HTTPPARA="URL","https://alex-exe.ru/links/links.html"
OK
AT+HTTPACTION=1 (1=POST)
OK
+HTTPACTION:0,200,1647
AT+HTTPREAD
+HTTPREAD:1647
<html>
<head>
<title>Alex_EXE - Сайты</title>
<link rel="StyleSheet" href="http://alex-exe.net.ru/wp-content/themes/redplanet-10/style.css" type="text/css"/>
</head>
<body>
<center>
<table border=1 cellspacing=0 cellpadding=0>
………………………………………………………
</html>
OK
AT+HTTPTERM
OK
