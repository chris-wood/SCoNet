ccnSIM helpers
==============

Helpers are very important components of ccnSIM, especially for writing simulation scenarios.
The following summarizes helpers and their basic usage.

StackHelper
---------------

:ccnsim:`StackHelper` is used to install ccnSIM network stack on requested nodes, as well to provide a simple way configure several important parameters of CCN simulation.

Example:

.. code-block:: c++

        ccn::StackHelper ccnHelper;
        NodeContainer nodes;
        ...
        ccnHelper.Install (nodes);

Routing
+++++++

All forwarding strategies require knowledge of where Interests can be forwarded (Forwarding Information Base).
Unlike IP routing, this knowledge may be imprecise, but without such knowledge forwarding strategies will not be able to make any decision and will drop any Interests.

.. note::
   By default, all nodes have empty FIB.  You need either to manually configure routes, use global routing controller, or (not recommended) enable default routes.

Manually routes
^^^^^^^^^^^^^^^

Routes can be configured manually using :ccnsim:`StackHelper::AddRoute` static methods of :ccnsim:`StackHelper`.

These routes **should** be created **after** installing CCN stack on a node:

  .. code-block:: c++

     ccnHelper.Install (nodes);
     ...
     Ptr<Node> node = ...     // FIB entry will be added to FIB on this node
     std::string prefix = ... // some prefix
     Ptr<ccn::Face> face = ... // CCN face that belongs to the node and through which prefix is accessible
     int32_t metric = ...     // some routing metric
     ccn::StackHelper::AddRoute (node, prefix, face, metric);

Global routing controller
^^^^^^^^^^^^^^^^^^^^^^^^^

To simplify FIB management in large topologies, ccnSIM contains a global routing controller (:ccnsim:`helper <GlobalRoutingHelper>` and :ccnsim:`special interface <GlobalRouter>`), similar in spirit to ``Ipv4GlobalRoutingHelper``.

There are several necessary steps, in order to take advantage of the global routing controller:

* install :ccnsim:`special interfaces <GlobalRouter>` on nodes

   .. code-block:: c++

     NodeContainer nodes;
     ...
     ccn::GlobalRoutingHelper ccnGlobalRoutingHelper;
     ccnGlobalRoutingHelper.Install (nodes);

* specify which node exports which prefix using :ccnsim:`GlobalRoutingHelper::AddOrigins`

   .. code-block:: c++

     Ptr<Node> producer; // producer node that exports prefix
     std::string prefix; // exported prefix
     ...
     ccnGlobalRoutingHelper.AddOrigins (prefix, producer);

* calculate and install FIBs on every node using :ccnsim:`GlobalRoutingHelper::CalculateRoutes`

   .. code-block:: c++

     cdnGlobalRoutingHelper.CalculateRoutes ();

Default routes
^^^^^^^^^^^^^^

In simple topologies, like in :doc:`examples <examples>`, or when
simulating broadcast environment, it is possible to set up *default*
FIB entries using :ccnsim:`StackHelper::SetDefaultRoutes` call.
More specifically, every installed CCN stack will have a FIB entry to ``/`` prefix, containing all available faces.

The following should be done before installing stack on a node:

  .. code-block:: c++

     ccnHelper.SetDefaultRoutes (true);
     ...
     ccnHelper.Install (nodes);


Content Store
+++++++++++++

ccnSIM comes with several different in-memory :ccnsim:`content store <ccn::ContentStore>` implementations, featuring different cache replacement policies.

To select a particular content store and configure its capacity, use :ccnsim:`SetContentStore <ccn::StackHelper::SetContentStore>` helper method:

      .. code-block:: c++

         ccnHelper.SetContentStore ("<content store implementation>",
                                    ["<optional parameter>", "<optional parameter's value>" [, ...]]);
	 ...
	 ccnHelper.Install (nodes);

In simulation scenarios it is possible to select one of :ref:`the existing implementations of the content store or implement your own <content store>`.


Pending Interest Table
++++++++++++++++++++++

The current version of ccnSIM provides :ccnsim:`templated realizations <ccn::pit::PitImpl>` of :ccnsim:`PIT abstraction <ccn::Pit>`, allowing optional bounding the number of PIT entries and different replacement policies (i.e., perform different actions when limit on number of PIT entries is reached).

To select a particular PIT implementation and configure its policies, use :ccnsim:`SetPit <ccn::StackHelper::SetPit>` helper method:

- :ccnsim:`persistent <ccn::pit::Persistent>` (default):

    New entries will be rejected if PIT size reached its limit

      .. code-block:: c++

         ccnHelper.SetPit ("ns3::ccn::pit::Persistent",
                           "MaxSize", "0");
	 ...
	 ccnHelper.Install (nodes);

- :ccnsim:`random <ccn::pit::Random>`:

    when PIT reaches its limit, random entry (could be the newly created one) will be removed from PIT;

      .. code-block:: c++

         ccnHelper.SetPit ("ns3::ccn::pit::Random",
                           "MaxSize", "0");
	 ...
	 ccnHelper.Install (nodes);

- :ccnsim:`least-recently-used <ccn::pit::Lru>`:

    the least recently used entry (the oldest entry with minimum number of incoming faces) will be removed when PIT size reached its limit.

      .. code-block:: c++

         ccnHelper.SetPit ("ns3::ccn::pit::Lru",
                           "MaxSize", "0");
	 ...
	 ccnHelper.Install (nodes);

Forwarding strategy
+++++++++++++++++++

A desired :ccnsim:`forwarding strategy <ForwardingStrategy>` parameter need to be set before installing stack on a node.

To select a particular forwarding strategy implementation and configure its parameters, use :ccnsim:`SetForwardingStrategy <ccn::StackHelper::SetForwardingStrategy>` helper method:

      .. code-block:: c++

         ccnHelper.SetForwardingStrategy ("<forwarding strategy implementation>",
                                          ["<optional parameter>", "<optional parameter's value>" [, ...]]);
	 ...
	 ccnHelper.Install (nodes);

In simulation scenarios it is possible to select one of :ref:`the existing implementations of the forwarding strategy or implement your own <forwarding strategies>`.


.. Currently, there are following forwarding strategies that can be used in simulations:

..   - :ccnsim:`Flooding` (default)

..       Interests will be forwarded to all available faces available for a route (FIB entry).
..       If there are no available GREEN or YELLOW faces, interests is dropped.

..       .. code-block:: c++

..          ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::Flooding");
.. 	 ...
.. 	 ccnHelper.Install (nodes);


..   - :ccnsim:`SmartFlooding`

..       If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
..       If not, Interest will be forwarded to all available faces available for a route (FIB entry)/
..       If there are no available GREEN or YELLOW faces, interests is dropped.

..       .. code-block:: c++

..          ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::SmartFlooding");
.. 	 ...
.. 	 ccnHelper.Install (nodes);

..   - :ccnsim:`BestRoute`

..       If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
..       If not, Interest will be forwarded to the highest-ranked YELLOW face.
..       If there are no available GREEN or YELLOW faces, interests is dropped.

..       .. code-block:: c++

..          ccnHelper.SetForwardingStrategy ("ns3::ccn::fw::BestRoute");
.. 	 ...
.. 	 ccnHelper.Install (nodes);




AppHelper
---------------

:ccnsim:`AppHelper` simplifies task of creating, configuring, and installing ccnSIM applications.


The basic usage of the :ccnsim:`AppHelper`:

* Create helper for specific applications class:

   .. code-block:: c++

      // Create helper for the consumer generating Interests with constant rate
      ccn::AppHelper consumerHelper ("ns3::ccn::ConsumerCbr");

* Assign prefix on which application operates (either generating Interests using this name or satisfying Interests for this name) using :ccnsim:`AppHelper::SetPrefix`:

   .. code-block:: c++

      consumerHelper.SetPrefix (prefix);

* Assign application-specific attributes using :ccnsim:`AppHelper::SetAttribute`:

   .. code-block:: c++

      // Set frequency parameter
      consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second

* Install application on one or more nodes:

   .. code-block:: c++

      NodeContainer nodes;
      ...
      consumerHelper.Install (nodes)


In simulation scenarios it is possible to select one of :ref:`the existing applications or implement your own <applications>`.
