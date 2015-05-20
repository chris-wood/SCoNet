[ccnSIM documentation](http://ccnsim.net)
=========================================

For more information, including downloading and compilation instruction, please refer to documentation in ``doc/`` or on http://ccnsim.net

Getting Started
---------------

### Portability

ccnSIM has been successfully compiled and used under Ubuntu Linux 12.04 (boost libraries **1.48**, with default version 1.46 compilation will probably fail), 12.10 (default version of boost 1.49), 13.04 (default version of boost 1.49), Fedora 18, Mac OS 10.7 and 10.8 (gcc-4.2 apple/llvm, macports gcc 4.7, boost 1.49-1.54).

### Requirements

1. ccnSIM requires the customized version of NS-3 simulator (a number of patches required to make ccnSIM work with the latest development branch of NS-3).

2. Boost libraries should be installed on the system:

  * For Ubuntu
  
    * 12.04
  
            sudo aptitude install libboost1.48-all-dev
  
    * 12.10, 13.04, and newer versions
  
            sudo aptitude install libboost-all-dev
  
  * For Fedora (for Fedora 18 and later only):
  
          sudo yum install boost-devel
  
  * For MacOS (macports):
  
          sudo port instal boost

  **!!! ccnSIM requires boost version at least 1.48.**   Many linux distribution (Fedora 16, 17 at the time of this writing) ship an old version of boost, making it impossible to compile ccnSIM out-of-the-box.  Please install the latest version, following these simple instructions (http://ccnsim.net/faq.html#installing-boost-libraries).

  **For Ubuntu 12.04**  Ubuntu 12.04 ships with two versions of boost libraries and it is known that if both are installed, then compilation of ccnSIM will most likely fail.  Please install ``libboost1.48-dev-all`` package and uninstall ``libboost-dev-all``.  If you want to install the latest version of boost libraries, then uninstall both ``libboost1.48-dev-all`` and ``libboost-dev-all``, so the libraries do not interfere with each other.

  If you do not have root permissions to install boost, you can install it in your home folder.  However, you need to be make sure that `libboost_iostreams` library is successfully compiled and is installed.  Please refer to the following example http://ccnsim.net/faq.html#installing-boost-libraries for the hints how to successfully compile and install boost libraries on Ubuntu Linux.

3. If you are planning to use other modules, like visualizer, a number of additional dependencies should be installed.  For example, in
order to run visualizer module (http://www.nsnam.org/wiki/index.php/PyViz), the following should be installed:

  * For Ubuntu (tested on Ubuntu 12.04, 12.10, 13.04, should work on later versions as well):
  
          sudo apt-get install python-dev python-pygraphviz python-kiwi
          sudo apt-get install python-pygoocanvas python-gnome2
          sudo apt-get install python-gnomedesktop python-rsvg ipython
  
  * For Fedora (tested on Fedora 16):
  
          sudo yum install pygoocanvas python-kiwi graphviz-python
  
          # easy_install method, since pygraphviz is not (yet?) packaged into Fedora (https://bugzilla.redhat.com/show_bug.cgi?id=740687)
          sudo yum install graphviz-devel
          sudo yum install python-pip
          sudo easy_install pygraphviz
  
  * For MacOS (macports):
  
          sudo port install  py27-pygraphviz py27-goocanvas

### Downloading ccnSIM source

Download a custom branch of NS-3 that contains all necessary patches, python binding generation library (optional), and clone actual ccnSIM code and place it in src/ folder:

    mkdir ccnSIM
    cd ccnSIM
    git clone git://github.com/cawka/ns-3-dev-ccnSIM.git ns-3
    git clone git://github.com/cawka/pybindgen.git pybindgen
    git clone git://github.com/CCN-Routing/ccnSIM.git ns-3/src/ccnSIM

There are quite a few modification to the base NS-3 code that are necessary to run ccnSIM, and the code is periodically synchronized with the official developer branch.  Eventually, all the changes will be merged to the official branch, but for the time being, it is necessary to use the customized branch.

If you have problems connecting to github, you can try to clone from google servers:

    mkdir ccnSIM
    cd ccnSIM
    git clone https://code.google.com/p/ccnsim.ns3-base/ ns-3
    git clone https://code.google.com/p/ccnsim.pybindgen/ pybindgen
    git clone https://code.google.com/p/ccnsim/ ns-3/src/ccnSIM

### Compiling and running ccnSIM

ccnSIM uses standard NS-3 compilation procedure.  Normally the following commands should be sufficient to configure and build ccnSIM with python bindings enabled:

    cd <ns-3-folder>
    ./waf configure --enable-examples
    ./waf

On MacOS (with macports), you may need to modify the configure command to use macports version of python:

    cd <ns-3-folder>
    ./waf configure --with-python=/opt/local/bin/python2.7 --enable-examples
    ./waf

Python bindings is an optional and not very stable feature of NS-3 simulator.  It is possible to disable python bindings compilation either to speed up compilation or to avoid certain compilation errors (e.g., "Could not find a task generator for the name 'ns3-visualizer'"):

    cd <ns-3-folder>
    ./waf configure --disable-python --enable-examples
    ./waf

For more configuration options, please refer to ``./waf --help``.

To run :doc:`sample ccnSIM simulations <examples>`:

    ./waf --run=ccn-simple

or:

    ./waf --run=ccn-grid

If you have compiled with python bindings, then you can try to run these simulations with visualizer:

    ./waf --run=ccn-simple --vis

or:

    ./waf --run=ccn-grid --vis

**Do not forget to configure and compile NS-3 in optimized mode (``./waf configure -d optimized``) in order to run actual simulations.**


### Simulating using ccnSIM

While it is possible to write simulations directly inside NS-3 (in ``scratch/`` folder) or ccnSIM (in ``examples/``), the recommended way is to write your simulation scenarios, as well as any custom extensions, separately from the NS-3 or ccnSIM core.

For example, you can use the following template to write your extensions, simulation scenarios, and metric processing scripts: (http://github.com/cawka/ccnSIM-scenario-template):

    mkdir ccnSIM
    cd ccnSIM
    git clone git://github.com/cawka/ns-3-dev-ccnSIM.git ns-3
    git clone git://github.com/cawka/pybindgen.git pybindgen
    git clone git://github.com/CCN-Routing/ccnSIM.git ns-3/src/ccnSIM
    
    # Build and install NS-3 and ccnSIM
    cd ns-3
    ./waf configure -d optimized
    ./waf
    
    sudo ./waf install
    cd ..
    
    git clone git://github.com/cawka/ccnSIM-scenario-template.git scenario
    cd scenario
    export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
    export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
    
    ./waf configure
    
    ./waf --run <scenario>

For more detailed information, refer to README file (https://github.com/cawka/ccnSIM-scenario-template/blob/master/README.md).

### Examples of template-based simulations

1. ccnSIM examples from http://ccnsim.net website and more:

- http://github.com/cawka/ccnSIM-examples or
- http://code.google.com/p/ccnsim.ccnsim-examples/

2. Script scenarios and graph processing scripts for simulations used in "A Case for Stateful Forwarding Plane" paper by Yi et al. (http://dx.doi.org/10.1016/j.comcom.2013.01.005):

- http://github.com/cawka/ccnSIM-comcom-stateful-fw or
- http://code.google.com/p/ccnsim.ccnsim-comcom/

3. Script scenarios and graph processing scripts for simulations used in "Rapid Traffic Information Dissemination Using Named Data" paper by Wang et al. http://dx.doi.org/10.1145/2248361.2248365

- http://github.com/cawka/ccnSIM-nom-rapid-car2car or
- http://code.google.com/p/ccnsim.ccnsim-nom-rapid/

- Rocketfuel-based topology generator for ccnSIM preferred format (randomly assigned link delays and bandwidth, based on estimated types of connections between nodes):

- http://github.com/cawka/ccnSIM-sample-topologies or
- http://code.google.com/p/ccnsim.ccnsim-sample-topo/
