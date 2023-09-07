# PVJ_esp
A repo to do some esp coding
This repo is my first Git repo so no judging! 
I created this for my personal use to store all the IoT code I did over here nd to not mix it up with my other docs and it's easier to locate

'''
The first file is a simple freeRTOS example program that will generate two tasks and they will print out a line themselves 
'''

'''
The second file named by date august 28 is an experimental file that has the code of a part of a project where the code reads the lines of a csv and then will 
trigger MQTT messages to the system and send as MQTT works via a publish subscribe model.

The CSV file had the timings of the day and if the timing smatched to a particular value then the system takes readings from it's surroundings and then will be able to intimate 
the main server about the same using the client.publish() function and it also prints on the serial window.

'''

''' 
The third file name IoT Irrigation is also a college project where we had an irrigation system based off of the sensors like soil moisture and then other
weather sensors like DHT11 and also the rainfall sensors. This code will monitor the surroundings at regular intervals of time and then it will check certain parameters required 
for the irrigation of a field and once the paramerters are met it will trigger the irrigation servo motor to rotate and then close. 
There is a lot of scope to be improved there as that was again a college project the fact that the motor rotated was satisfactory for my prof.

Oh also the data was uploaded to ThingSpeak server to visualize bringing the IoT element to this project.
'''

'''
The next file is just the same as the irrigation system minus the motor it's just monitoring the weather and returning the system the data and I'm just printing out some statements like 
"It's raining" etc 
'''
overall this repo is for me if you're a beginner who wants to code for ESP and IoT you can see and copy these codes.
