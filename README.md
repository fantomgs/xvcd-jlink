"# xvcd-jlink" 

Usage: xvcd [-v] [-p port] [-s jtag_speed_in_kHz] [-i core_id]
v - verbose
port - listen port (by default is 2542)
jtag_speed_in_kHz - speed of the JTAG connection in Khz (by default 1000 kHz)
core_id - IDCODE of hardware core to search in JTAG chain (probably unnecessary feaure, just for test)

Add connection string to iMPACT or ChipScope cable plugin:
xilinx_xvc host=localhost:2542 disableversioncheck=true
