.. _forwarding strategies:

Forwarding Strategies
=====================

ccnSIM provides simple ways to experiment with custom Interest/Data forwarding strategies.
A new forwarding strategy can be implement completely different processing or override just specific actions/events of the :ccnsim:`forwarding strategy interface <ccn::ForwardingStrategy>`.
Please refer to :ccnsim:`API documentation <ccn::ForwardingStrategy>` of the forwarding strategy interface, which lists all default actions/events.

Available forwarding strategies
+++++++++++++++++++++++++++++++

Basic forwarding strategies
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Flooding
########

Interests will be forwarded to all available faces available for a route (FIB entry).
If there are no available GREEN or YELLOW faces, interests is dropped.

Implementation name: :ccnsim:`ns3::ccn::fw::Flooding` (default)

Usage example:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::Flooding");
	 ...
	 ccnHelper.Install (nodes);

SmartFlooding
#############

If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
If not, Interest will be forwarded to all available faces available for a route (FIB entry)/
If there are no available GREEN or YELLOW faces, interests is dropped.

Implementation name :ccnsim:`ns3::ccn::fw::SmartFlooding`

Usage example:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::SmartFlooding");
	 ...
	 ccnHelper.Install (nodes);

BestRoute
#########

If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
If not, Interest will be forwarded to the highest-ranked YELLOW face.
If there are no available GREEN or YELLOW faces, interests is dropped.

Implementation name: :ccnsim:`ns3::ccn::fw::BestRoute`

Usage example:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::BestRoute");
	 ...
	 ccnHelper.Install (nodes);

Strategies with Interest limits
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following strategies enforce different granularities of Interest limits.  Each strategy is an extension of the basic one (custom strategies can also be extended with limits, refer to the source code).

Currently, ccnSIM implements two types of Interest limit enforcements, both based on a Token Bucket approach:

   - :ccnsim:`ns3::ccn::Limits::Window` (default)

        Interest token is borrowed when Interest is send out.  The token is returned only when Interest is satisfied or times out.

   - :ccnsim:`ns3::ccn::Limits::Rate`

        Interest token is borrowed when Interest is send out.  The token is returned periodically based on link capacity.

In both cases, limit is set according to the following equation:

.. math::

    \mathrm{Interest\ Limit} = Delay\ [s] \cdot
       \frac{\mathrm{Bandwidth\ [Bytes/s]}}
       {\mathrm{Data\ packet\ size\ [Bytes]} + \mathrm{Interest\ packet\ size\ [Bytes]}}

To configure packet sizes and delay parameters, use the following :ccnsim:`ccn::StackHelper` method:

      .. code-block:: c++

         // ccnHelper.EnableLimits (true, <delay>, <average interest packet size>, <average data packet size>);
         ccnHelper.EnableLimits (true, Seconds (0.2), 40, 1100);
	 ...
	 ccnHelper.Install (nodes);

Usage examples
##############

Per outgoing Face limits
%%%%%%%%%%%%%%%%%%%%%%%%

- :ccnsim:`ns3::ccn::fw::Flooding::PerOutFaceLimits`

    With :ccnsim:`Limits::Window`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::Flooding::PerOutFaceLimits"
                                          "Limit", "ns3::ccn::Limits::Window");
	 ...
	 ccnHelper.Install (nodes);


    With :ccnsim:`Limits::Rate`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::Flooding::PerOutFaceLimits"
                                          "Limit", "ns3::ccn::Limits::Rate");
	 ...
	 ccnHelper.Install (nodes);

- :ccnsim:`ns3::ccn::fw::SmartFlooding::PerOutFaceLimits`

    With :ccnsim:`Limits::Window`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::SmartFlooding::PerOutFaceLimits"
                                          "Limit", "ns3::ccn::Limits::Window");
	 ...
	 ccnHelper.Install (nodes);


    With :ccnsim:`Limits::Rate`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::SmartFlooding::PerOutFaceLimits"
                                          "Limit", "ns3::ccn::Limits::Rate");
	 ...
	 ccnHelper.Install (nodes);

- :ccnsim:`ns3::ccn::fw::BestRoute::PerOutFaceLimits`

    With :ccnsim:`Limits::Window`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::BestRoute::PerOutFaceLimits"
                                          "Limit", "ns3::ccn::Limits::Window");
	 ...
	 ccnHelper.Install (nodes);


    With :ccnsim:`Limits::Rate`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::BestRoute::PerOutFaceLimits"
                                          "Limit", "ns3::ccn::Limits::Rate");
	 ...
	 ccnHelper.Install (nodes);


Per FIB entry, per outgoing face limits
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

- :ccnsim:`ns3::ccn::fw::Flooding::PerOutFaceLimits::PerFibLimits`

    With :ccnsim:`Limits::Window`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::Flooding::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ccn::Limits::Window");
	 ...
	 ccnHelper.Install (nodes);


    With :ccnsim:`Limits::Rate`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::Flooding::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ccn::Limits::Rate");
	 ...
	 ccnHelper.Install (nodes);

- :ccnsim:`ns3::ccn::fw::SmartFlooding::PerOutFaceLimits::PerFibLimits`

    With :ccnsim:`Limits::Window`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::SmartFlooding::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ccn::Limits::Window");
	 ...
	 ccnHelper.Install (nodes);


    With :ccnsim:`Limits::Rate`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::SmartFlooding::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ccn::Limits::Rate");
	 ...
	 ccnHelper.Install (nodes);

- :ccnsim:`ns3::ccn::fw::BestRoute::PerOutFaceLimits::PerFibLimits`

    With :ccnsim:`Limits::Window`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::BestRoute::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ccn::Limits::Window");
	 ...
	 ccnHelper.Install (nodes);


    With :ccnsim:`Limits::Rate`:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::BestRoute::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ccn::Limits::Rate");
	 ...
	 ccnHelper.Install (nodes);

.. _Writing your own custom strategy:

Writing your own custom strategy
++++++++++++++++++++++++++++++++

First step in creating your own strategy is to decide which existing strategy you want to extend.  You can either use realize :ccnsim:`forwarding strategy interface <ccn::ForwardingStrategy>` (:ccnsim:`ccn::ForwardingStrategy::DoPropagateInterest` call must be implemented) or extended one of the available forwarding strategies (:ccnsim:`fw::BestRoute` or :ccnsim:`fw::Flooding`).
The following example assumes that we are realizing :ccnsim:`forwarding strategy interface <ccn::ForwardingStrategy>`.

The follwoing are template strategy h/cc files:

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.h
   :language: c++
   :linenos:
   :lines: 1-36,51-55,59-

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.cc
   :language: c++
   :linenos:
   :lines: 1-40,42-50,75-76,115-
   :emphasize-lines: 21,27

After having the template, we can fill the necesasry functionality.

Let us say, that we want Interest be forwarded to first two best-metric faces specified by FIB.
That is, if node has two or more alternative paths to forward the Interests, this Interest will be forwarded to the best two neighbors.
The following implementation of CustomStrategy::DoPropagateInterest accomplishes the task:

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.cc
   :language: c++
   :linenos:
   :lines: 45-75
   :emphasize-lines: 7-30

After the compilation, you can use ``"ns3::ccn::fw::CustomStrategy"`` as a parameter to :ccnsim:`ccn::StackHelper::SetForwardingStrategy` helper method.

 .. as well as NS_LOG=ccn.fw.CustomStrategy when running in a debug mode

Extending strategy
++++++++++++++++++

If you need more customization for the forwarding strategy, there are many forwarding strategy events that can be overriden.
For example, if we want to perform special logging of all forwarded, timed out, and satisfied Intersts, we can override the following events (for more events, refer to :ccnsim:`ForwardingStrategy API documentation <ForwardingStrategy>`):

- :ccnsim:`DidSendOutInterest <ForwardingStrategy::DidSendOutInterest>`, which fired just after forwarding the Interest

- :ccnsim:`WillEraseTimedOutPendingInterest <ForwardingStrategy::WillEraseTimedOutPendingInterest>`, which fired just before PIT entry is removed by timeout

- :ccnsim:`WillSatisfyPendingInterest <ForwardingStrategy::WillSatisfyPendingInterest>`, which fired just before Interest will be satisfied.

The highlighted ares of the following code demonstrates how it can be impelmented:

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.h
   :language: c++
   :linenos:
   :emphasize-lines: 37-50,56-58

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.cc
   :language: c++
   :linenos:
   :emphasize-lines: 41,77-114


Example of using custom strategy
++++++++++++++++++++++++++++++++

Please refer to :ref:`this example <11-node 2-bottleneck topology with custom forwarding strategy>`.

