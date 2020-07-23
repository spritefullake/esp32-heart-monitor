# ESP32 Arduino ECG Device
## Instructions

* Make sure your network discovery is on. (For windows, go to Sharing options in your settings: Network and Sharing Center > Advanced sharing settings

* Write a Secrets.h file that looks like the following (substitute in your network SSID and password, respectively:
````
const char * networkName = "your network name";
const char * networkPswd = "your network password";
````
* Find the IP address for your ESP32 device
There are various ways to achieve this; It will look something like: 192.168.x.xx where x are numbers.
 
