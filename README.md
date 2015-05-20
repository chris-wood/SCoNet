# CCNX1-SIM
CCNX 1.0 simulator

This archive is currently a bit of a mess.   It is heavily modified ndnSim (which only has a few ndn elements left) 
based upon ns3.    Current checkin is the full tree for development.
The tree will be trimmed shortly

To run, disable examples and tests using waf.   Also, disable python.

./waf configure --disable-tests --disable-examples --disable-python
