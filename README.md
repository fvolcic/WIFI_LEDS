# WiFi LEDS


# WiFi LED Strips	# WiFi LED Strips


## About	## About
<ul>This project was created to contorl LED lights via wifi on an MQTT server. </ul>	<ul>This project was created to contorl LED lights via wifi on an MQTT server. The goal was to provide a simple API that controls all the functionality of the LED strip</ul>


## Platform information	## Platform information
<ul> This code was written in C++ using the arduino IDE. This code will only run on an esp32 currently, as the code is written to take advantage of the ESP32's multicore functionality.</ul>	<ul> This code was written in C++ using the arduino IDE. This code will only run on an esp32 currently, as the code is written to take advantage of the ESP32's multicore functionality.</ul>


## Install Information
<ul> IDE Install - To run this code, download the code, make sure all the files are in the same folder, then open with Arduino IDE. Once open, set the IP of the MQTT server along with the ssid and password of the network you will be connecting to. Once complete, upload to the board. Thats all that is needed to run this code. </ul>

## TODO	## TODO
<ol>	<ol>
 <li>Write code for a working file system that uses SPIFFS on the ESP32</li>	 <li>Write code for a working file system that uses SPIFFS on the ESP32</li>
 <li>Fix a ledstrip.write bug for the gradient. (there is an issue where the gradients are calculated slower than they are written to the strip. This happens because they are running on different cores.)</li>	 <li>Fix a ledstrip.write bug for the gradient. (there is an issue where the gradients are calculated slower than they are written to the strip. This happens because they are running on different cores.)</li>
 <li>Add a white avoidace algorithm for the gradient. (First make an algorithm that avoids white by adding a midpoint, but after add an algorithm that uses bezier curves in 3d space to avoid the grey line.</li>	 <li>Add a white avoidace algorithm for the gradient. (First make an algorithm that avoids white by adding a midpoint, but after add an algorithm that uses bezier curves in 3d space to avoid the grey line.</li>
 <li>Create an object that is dispalyed on the GPS graph that represents the drop zones.</li>	 <li>Create an object that is dispalyed on the GPS graph that represents the drop zones.</li>
</ol>	</ol>

## MQTT API - COMMAND LIST AND DESCRIPTION
<ul> Message Format - [3 Number Message Length][4 Character Action Code][Action Attributes]<br>Ex: 009SOLI255000255<br>
The message for each command must be the exact same as the message length unless otherwise stated.</ul>
<ol>
 <li> Solid Color Action <br> - Description: Updates the color of the entire LED strip based on the color in the message. <br> - Message Length: 009 <br> - Action Code: SOLI <br> - Message: rgb color {3Bytes - R}{3 Bytes - G}{3 Bytes - B}</li> 

 <li> Gradient Action <br> - Description: Updates the LED strip to be a gradient between the 2 colors in the MQTT message <br> - Message Length: 018 <br> - Action Code: GRAD <br> - Message: 2 rgb colors {3Bytes - R1}{3 Bytes - G1}{3 Bytes - B1}{3Bytes - R2}{3 Bytes - G2}{3 Bytes - B2}</li> 

 <li> Moving Gradient Action <br> - Description: Similar to the Gradient Action, but the moving gradient makes a gradient that moves fluidly across the LED strip. <br> - Message Length: 018 <br> - Action Code: MGRD <br> - Message: 2 rgb colors {3Bytes - R1}{3 Bytes - G1}{3 Bytes - B1}{3Bytes - R2}{3 Bytes - G2}{3 Bytes - B2}</li> 

 <li> Brightness Action <br> - Description: Updates the brightness of the LED strip <br> - Message Length: 003 <br> - Action Code: BRIG <br> - Message: Brightness. Range[000-031] - Format: {3 number brightness}</li> 

 <li> Clear Action <br> - Description: Deletes all the actions that are currently running on the led strip. <br> - Message Length: 000 <br> - Action Code: CLEA <br> - Message: </li>

 <li> Point Action <br> - Description: Makes every N-th LED a given color. Based on the MQTT Message <br> - Message Length: 012 <br> - Action Code: PONT <br> - Message: 2 Variables. LED color, Distance Between LEDs. Format: {3Bytes - R}{3 Bytes - G}{3 Bytes - B}{3 Bytes - Dist} </li> 

 <li> Party Action <br> - Description: A cool party mode.. Try it out! <br> - Message Length: 018 <br> - Action Code: STRN <br> - Message: 2 Variables. Col1, Col2. Format: {3Bytes - R1}{3 Bytes - G1}{3 Bytes - B1}{3Bytes - R2}{3 Bytes - G2}{3 Bytes - B2} </li> 

 <li> Brightness Gradient Action <br> - Description: Applies a brightness gradient across the LED strip <br> - Message Length: 003 <br> - Action Code: BGRD <br> - Message: 1 Variable - speed. Format: {3 Bytes - Movement Speed} </li> 

  <li> Sub Strip Action <br> - Description: This is a relativly complex action. This action creates a action within itself, and applies the action to a small section of the LED strip. For example, you could generate a moving gradient that only applied to pixel 0015 to pixel 0055 <br> - Message Length: 128 <br> - Action Code: SACT <br> - Message: 3 Variables. V1: Start Led Index, V2: End Led Index, V3: LED Action message. Format: {4 Bytes - Start}{4 Bytes - End}{Next Action Message}<br>Additional Information. Example of useage. This create a moving gradient that ranges from pixel 0005 to 0015. 128SACT00050015018MGRD255000000000255000.  </li> 

  <li> Update Mode Action <br> - Description: Puts the LED strip into Update Mode <br> - Message Length: 000 <br> - Action Code: UPDA <br> - Message:  </li> 
</ol>

## OTA Updates with Arduino API

<ul> To update the LED lights using OTA, the board must first be put into update mode. To do this, send the board the Update Mode Action, which can be found in the MQTT API documentation. Once this has been done, the board will be ready to be updated. Go to the arduino IDE, and select the network port that corresponds to the ESP32 you would like to update. Once this is done, the ESP will update and reset itself running the new code. Update mode will automatically exit after 4 minutes, and can also be exited by rebooting the ESP. </ul>
