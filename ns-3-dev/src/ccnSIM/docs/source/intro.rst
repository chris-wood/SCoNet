.. ccnSIM: NS-3 based CCN simulator
.. ============================================================

============
Introduction
============

.. sidebar:: ccnSIM components

    .. image:: _static/ccnsim-components.*
        :width: 100%

The ccnSIM is NS-3 module that implements Named Data Networking (CCN) communication model, the clean slate Internet design. ccnSIM is specially optimized for simulation purposes and has a cleaner and more extensible internal structure comparing to the existing CCN implementation (CCNx).

Following the CCN architecture, ccnSIM is implemented as a new network-layer protocol model, which can run on top of any available link-layer protocol model (point-to-point, CSMA, wireless, etc.).

.. note::
    It will also be possible to run ccnSIM on top of network-layer (IPv4, IPv6) and transport-layer (TCP, UDP) protocols.
    However, it is not yet implemented and patches are welcome.

.. This flexibility allows ccnSIM to simulate scenarios of various homogeneous and heterogeneous networks (e.g., CCN-only, CCN-over-IP, etc.).

The simulator is implemented in a modular fashion, using separate C++ classes to model behavior of each network-layer entity in CCN: :ccnsim:`pending Interest table (PIT) <Pit>`, :ccnsim:`forwarding information base (FIB) <Fib>`, :ccnsim:`content store <ContentStore>`, :ccnsim:`network <NetDeviceFace>` and :ccnsim:`application <AppFace>` interfaces, :ccnsim:`Interest forwarding strategies <ForwardingStrategy>`, etc.
This modular structure allows any component to be easily modified or replaced with no or minimal impact on other components.
In addition, the simulator provides an extensive collection of interfaces and helpers to perform detailed tracing behavior of every component, as well as CCN traffic flow.

The current wire format for the Interest and Data packets used by ccnSIM is defined in :ref:`ccnSIM packet format`.
ccnSIM also has an option to be compatible to wire format used by `CCNx implementation <http://named-data.net/>`_ (CCNx binary XML encoding).  However currently, this option is deprecated, but can be reintroduced in the future as an optional wire format.

More documentation
------------------

Overall structure of ccnSIM is described in our `technical report <http://lasr.cs.ucla.edu/afanasyev/data/files/Afanasyev/ccnSIM-TR.pdf>`_.

`ccnSIM API documentation <doxygen/index.html>`_

Also, you can `join our mailing list <http://www.lists.cs.ucla.edu/mailman/listinfo/ccnsim>`_ to see and participate in discussions about ccnSIM implementation and simulations in general.   
Do not forget to check mailling list `archives <http://www.lists.cs.ucla.edu/pipermail/ccnsim/>`_.  


Support
-------

The code of ccnSIM is in active development.  Bug reports (issues) as well as new feature implementation are always welcome. 

To file a bug report, please use `GitHub Issues <https://github.com/CCN-Routing/ccnSIM/issues>`_.

To create new feature, please fork the code and submit Pull Request on GitHub.

And of course, `our mailing list <http://www.lists.cs.ucla.edu/mailman/listinfo/ccnsim>`_ is the best way to communicate with and get support from ccnSIM team and other users of ccnSIM.

A very short guide to the code
------------------------------

All the CCN related code is in ``ns-3/src/ccnSIM``

+-----------------+---------------------------------------------------------------------+
| Folder          | Description                                                         |
+=================+=====================================================================+
| ``model/``      | implementation of CCN base: :ccnsim:`L3Protocol`, faces             |
|                 | (:ccnsim:`Face`, :ccnsim:`NetDeviceFace`, forwarding                |
|                 | :ccnsim:`AppFace`),                                                 |
|                 | strategies (:ccnsim:`ForwardingStrategy`,                           |
|                 | :ccnsim:`Flooding`, :ccnsim:`SmartFlooding`, :ccnsim:`BestRoute`),  |
|                 | etc.                                                                |
+-----------------+---------------------------------------------------------------------+
| ``apps/``       | applications (in NS-3 sense) that can be installed on the nodes.    |
|                 | Right now we have one producer (:ccnsim:`Producer`) and a           |
|                 | collection  of consumer (:ccnsim:`ConsumerCbr`,                     |
|                 | :ccnsim:`ConsumerWindow`,                                           |
|                 | :ccnsim:`ConsumerBatches`).  See doxygen documentation or           |
|                 | source  code for details                                            |
+-----------------+---------------------------------------------------------------------+
| ``helper/``     | a number of :doc:`useful helpers <helpers>`                         |
+-----------------+---------------------------------------------------------------------+
| ``examples/``   | contain :doc:`several example scenarios <examples>`                 |
+-----------------+---------------------------------------------------------------------+
| ``utils/``      | helper classes, including implementation of generalized data        |
|                 | structures                                                          |
+-----------------+---------------------------------------------------------------------+
| ``plugins/``    | a number of plugins that may be helpful to run simulation scenarios |
+-----------------+---------------------------------------------------------------------+

Logging
-------

Almost every component in ccnSIM exports logging interface, so in debug compilation it is possible to track many internal details. 
For example, logging of :ccnsim:`Face` and :ccnsim:`Consumer` shows everything what happens in :ccnsim:`Face` and :ccnsim:`Consumer` classes::

    NS_LOG=ccn.Face:ccn.Consumer ./waf --run=ccn-simple

Refer to the source code and NS-3 documentation to see what logging interfaces are available and about details how enable one or more logging interfaces.

