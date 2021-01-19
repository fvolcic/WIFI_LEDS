# WiFiLED4

# GroundStation 2020 Edition

## About
<ul>This project was created to contorl LED lights via wifi on an MQTT server. </ul>

## Platform information
<ul> This code was written in C++ using the arduino IDE. This code will only run on an esp32 currently, as the code is written to take advantage of the ESP32's multicore functionality.</ul>

## TODO
<ol>
 <li>Write code for a working file system that uses SPIFFS on the ESP32</li>
 <li>Fix a ledstrip.write bug for the gradient. (there is an issue where the gradients are calculated slower than they are written to the strip. This happens because they are running on different cores.)</li>
 <li>Add a white avoidace algorithm for the gradient. (First make an algorithm that avoids white by adding a midpoint, but after add an algorithm that uses bezier curves in 3d space to avoid the grey line.</li>
 <li>Create an object that is dispalyed on the GPS graph that represents the drop zones.</li>
</ol>
