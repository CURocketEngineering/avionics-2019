# Base Telemetry
### TODO
* Decide between keeping in python2 or converting to python3
* `mpl.py` and `graphs/`
* Decide what needs to be represented on base station
  * Graphs
    * Altitude, acceleration, velocity
  * Numbers/Animations?
    * GPS, tilt, ...
  * Other
    * Air pressure, temperature, other 9DOF
* Save json to file on base station

### Running
#### Compiling and Running
1. `python main.py`
#### Debugging
1. 

### Information
* JSON Formatting
  * example: `{"sensors":{"gyro":{"x":0,"y":0,"z":0},"acc":{"x":0.000025,"y":0.000025,"z":38.01759},"mag":{"x":0,"y":0,"z":0},"att":{"r":6.451225e-7,"p":-6.451225e-7,"y":-0.108385},"bar":{"p":101324.9,"dp":0.890625,"alt":0.001904,"gnd":1237.593,"temp":0,"hum":0},"gps":{"lat":0,"lon":0,"hour":0,"min":0,"sec":0,"day":0,"mon":0,"year":0}},"type":"telemetry","time":804}`
  * Rocket will return self-contained json
    * Will always contain `type`, `time`, and `state`
      * `type` will be 1 of: state, telemetry, 
      * `state` will be 1 of the states: ignite, 
    * Telemetry contains ... TODO

### Components
#### Current Components
* Antenna thing???

### Questions for Foster
