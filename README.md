# SCoNet
(Simulator COntent NETworking)
THis simulator is based upon ccnx 1.0 (ccnx.org)

This archive is currently a bit of a mess.   It is heavily modified ndnSim (which only has a few ndn elements left) 
based upon ns3.    Current checkin is the full tree for development.
The tree will be trimmed shortly

To run, disable examples and tests using waf.   Also, disable python.

./waf configure --disable-tests --disable-examples --disable-python

NS_LOG=ccn.cs ./waf --run scratch/paper1c

