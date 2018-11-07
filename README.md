![CURocket Logo](pics/logo.png)

### About
Clemson University Rocket Engineering (CURE) Avionics subteam program files. 
Powers CURE's competitive and non-competitive rockets. Used in the 
Intercollegiate Rocket Engineering Competition. Begun by Foster McLane, 2016.

### Documentation
* [`base/`](base/)
  * Code ran on the CURE base station for communication and telemetry
  * `main.py` for basic communication
  * `mpl.py` for graphical interpretation
  * `mpl_simulator.py` for simulating graphical interpretation
* [`cad/`](/cad)
  * Files for electrically engineering components in `eagle`
* [`data/`](data/)
  * Flight data
  * `jsongrapher.py` for ensuring simulation (and actual) data 
  * ['Complete simulation'](data/sim_data_graphed) for pictures of complete simulations
* [`extra/`](extra/)
  * Testing
  * GPS and XBEE `.ino` files
* [`main/`](main/)
  * Code ran on CURE rockets used for data collection, state changes, and telemetry
* [`pics/`](pics/)
  * Pictures used in this github
* [`xbee/bypass/`](xbee/bypass/)
  * Communication

### License
This project is licensed used the GNU General Public License v3.0

