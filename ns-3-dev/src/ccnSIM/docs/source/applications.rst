.. _applications:

ccnSIM applications
===================

ccnSIM includes a few reference applications that can be used as a base for CCN simulations.

Reference applications
++++++++++++++++++++++

ConsumerCbr
^^^^^^^^^^^^^^^

:ccnsim:`ConsumerCbr` an application that generates Interest traffic with predefined pattern (constant frequency, constant average rate with inter-Interest gap distributed uniformly at random, exponentially at random, etc.).

.. code-block:: c++

   // Create application using the app helper
   ccn::AppHelper helper ("ns3::ccn::ConsumerCbr");

This applications has the following attributes:

* ``Frequency``

  .. note::
     default: ``1.0`` (1 per second)

  Either exact (for contant version) or expected (for randomized version) frequency with which Interests are generated

  .. code-block:: c++

     // Set attribute using the app helper
     helper.SetAttribute ("Frequency", DoubleValue (1.0));

* ``Randomize``

  .. note::
     default: ``"none"``

  Specify whether to do randomization for inter-Interest gap or not.  The following variants are currently supported:

  - ``"none"``: no randomization

  - ``"uniform"``: uniform distribution in range (0, 1/Frequency)

  - ``"exponential"``: exponential distribution with mean 1/Frequency

  .. code-block:: c++

     // Set attribute using the app helper
     helper.SetAttribute ("Randomize", StringValue ("uniform"));

ConsumerZipfMandelbrot
^^^^^^^^^^^^^^^^^^^^^^

.. note::
    Author: Xiaoke Jiang

An app that requests contents (names in the requests) following Zipf-Mandelbrot distribution (number of Content frequency Distribution).
This class is a subclass of :ccnsim:`ConsumerCbr`.

.. code-block:: c++

   // Create application using the app helper
   ccn::AppHelper helper ("ns3::ccn::ConsumerZipfMandelbrot");

``Frequency`` and ``Randomize`` attributes can be used in the same way as in the base :ccnsim:`ConsumerCbr` applications.

Additional attributes:

* ``NumberOfContents``

    .. note::
        default: 100

    Number of different content (sequence numbers) that will be requested by the applications


THE following pictures show basic comparison of the generated stream of Interests versus theoretical `Zipf-Mandelbrot <http://en.wikipedia.org/wiki/Zipf%E2%80%93Mandelbrot_law>`_ function (``NumberOfContents`` set to 100 and ``Frequency`` set to 100)

.. image:: _static/apps/zipf-n100-frq100-duration10.png
   :alt: Comparsion between simulation and theory with simulation duration 10 seconds

.. image:: _static/apps/zipf-n100-frq100-duration50.png
   :alt: Comparsion between simulation and theory with simulation duration 50 seconds

.. image:: _static/apps/zipf-n100-frq100-duration100.png
   :alt: Comparsion between simulation and theory with simulation duration 100 seconds

.. image:: _static/apps/zipf-n100-frq100-duration1000.png
   :alt: Comparsion between simulation and theory with simulation duration 1000 seconds


ConsumerBatches
^^^^^^^^^^^^^^^^^^^

:ccnsim:`ConsumerBatches` is an on-off-style application gen- erating a specified number of Interests at specified points of simulation.

.. code-block:: c++

   // Create application using the app helper
   ccn::AppHelper consumerHelper ("ns3::ccn::ConsumerBatches");

This applications has the following attributes:

* ``Batches``

  .. note::
     default: Empty

  Specify exact pattern of Interest packets, specifying when and how many Interest packets should be sent.
  The following example defines that 1 Interest should be requested at time 1s, 5 Interests at time 5s, and 2 Interests at time 10s.:

  .. code-block:: c++

     // Set attribute using the app helper
     helper.SetAttribute ("Batches", StringValue ("1s 1 2s 5 10s 2"));


ConsumerWindow
^^^^^^^^^^^^^^^^^^

:ccnsim:`ConsumerWindow` is an application generating a variable rate Interest traffic. It relies on an optional NACK-Interest feature and implements a simple sliding-window-based Interest generation mechanism.

.. code-block:: c++

   // Create application using the app helper
   ccn::AppHelper consumerHelper ("ns3::ccn::ConsumerWindow");


This applications has the following attributes:

* ``Window``

  .. note::
     default: ``1``

  Initial number of Interests that will be send out without waiting for the data (number of outstanding Interests)

* ``PayloadSize``

  .. note::
     default: ``1040``

  Expected size of the Data payload (necessary only when Size is specified)

* ``Size``

  .. note::
     default: ``-1``

  Amount of data to be requested (will stop issuing Interests after ``Size`` data is received)

  If ``Size`` is set to -1, Interests will be requested till the end of the simulation.

Producer
^^^^^^^^^^^^

:ccnsim:`Producer` a simple Interest-sink application, which replying every incoming Interest with Data packet with a specified size and name same as in Interest.

.. code-block:: c++

   // Create application using the app helper
   ccn::AppHelper consumerHelper ("ns3::ccn::Producer");

.. _Custom applications:

Custom applications
+++++++++++++++++++

Applications interact with the core of the system using :ccnsim:`AppFace` realization of Face abstraction.
To simplify implementation of specific CCN application, ccnSIM provides a base :ccnsim:`App` class that takes care of creating :ccnsim:`AppFace` and registering it inside the CCN protocol stack, as well as provides default processing for incoming Interest and Data packets.

.. Base App class
.. ^^^^^^^^^^^^^^^^^^


Customer example
^^^^^^^^^^^^^^^^

The following code shows how a simple ccnSIM application can be created, and how an application can send Interests and respond with ContentObjects to incoming interests.

When this application starts it sets "interest filter" (install FIB entry) for /prefix/sub, as well as sends Interest for this prefix.
When an Interest is received, it is replied with a ContentObject with 1024-byte fake payload.

For more details refer ``examples/custom-apps/custom-app.h``, ``examples/custom-apps/custom-app.cc`` and API documentation of ccnSIM and NS-3.

Header file ``examples/custom-apps/custom-app.h``:

.. literalinclude:: ../../examples/custom-apps/custom-app.h
    :language: c++
    :linenos:
    :lines: 21-29,40-

Source file ``examples/custom-apps/custom-app.cc``:

.. literalinclude:: ../../examples/custom-apps/custom-app.cc
    :language: c++
    :linenos:
    :lines: 21-

Example how to use custom app in a scenario (``ccn-simple-with-custom-app.cc``):

 .. *      +------+ <-----> (CustomApp1)
 .. *      | Node |
 .. *      +------+ <-----> (CustomApp2)
 .. *

.. literalinclude:: ../../examples/ccn-simple-with-custom-app.cc
    :language: c++
    :linenos:
    :lines: 21-28,39-
    :emphasize-lines: 26-31


To run this scenario, use the following command::

        NS_LOG=CustomApp ./waf --run=ccn-simple-with-custom-app


Producer example (Interest hijacker)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following code demonstrates how to implement a basic producer application that "hijacks" all incoming Interests.

Header file ``examples/custom-apps/hijacker.h``:

.. literalinclude:: ../../examples/custom-apps/hijacker.h
    :language: c++
    :linenos:
    :lines: 21-

Source file ``examples/custom-apps/hijacker.cc``:

.. literalinclude:: ../../examples/custom-apps/hijacker.cc
    :language: c++
    :linenos:
    :lines: 21-


After defining this class, you can use it with :ccnsim:`ccn::AppHelper`. For example:

.. code-block:: c++

    ...
    ccn::AppHelper producerHelper ("Hijacker");
    producerHelper.Install (producerNode);
    ...

Dumb requester
^^^^^^^^^^^^^^

This application continually requests the same Data packet.

Header file ``examples/custom-apps/dumb-requester.h``:

.. literalinclude:: ../../examples/custom-apps/dumb-requester.h
    :language: c++
    :linenos:
    :lines: 21-

Source file ``examples/custom-apps/dumb-requester.cc``:

.. literalinclude:: ../../examples/custom-apps/dumb-requester.cc
    :language: c++
    :linenos:
    :lines: 21-


