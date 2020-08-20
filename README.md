# ESP32 Arduino ECG Device
## Instructions

* Make sure your network discovery is on. (For windows, go to Sharing options in your settings: Network and Sharing Center > Advanced sharing settings

* Write a Secrets.h file that looks like the following (substitute in your network SSID and password, respectively:
````
const char * networkName = "your network name";
const char * networkPswd = "your network password";
const char * siteAddress = "your computer's ip address if you're running locally";
````
* Find the IP address for the computer you want to display the data on
There are various ways to achieve this; It will look something like: 192.xxx.x.xx where x are numbers.

You can go to [the heart display page](/https://github.com/spritefullake/express-rt-heart-display/blob/master/bin/www) for more info on finding your local IP address.

