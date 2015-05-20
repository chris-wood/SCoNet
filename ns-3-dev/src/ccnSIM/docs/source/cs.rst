.. _content store:

Content Store
+++++++++++++

ccnSIM comes with several different in-memory :ccnsim:`content store <ccn::ContentStore>` implementations, featuring different cache replacement policies.

.. note:

    The default content store uses LRU replacement policity and constrained with 100 cached ContentObjects.

To select a particular content store and configure its capacity, use :ccnsim:`SetContentStore <ccn::StackHelper::SetContentStore>` helper method

Simple content stores
^^^^^^^^^^^^^^^^^^^^^

Least Recently Used (LRU) (default)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ccnsim:`ccn::cs::Lru`.

Usage example:

      .. code-block:: c++

         ccnHelper.SetContentStore ("ns3::ccn::cs::Lru",
                                    "MaxSize", "10000");
	 ...
	 ccnHelper.Install (nodes);

First-In-First-Out (FIFO)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ccnsim:`ccn::cs::Fifo`

Usage example:

      .. code-block:: c++

         ccnHelper.SetContentStore ("ns3::ccn::cs::Fifo",
                                    "MaxSize", "10000");
	 ...
	 ccnHelper.Install (nodes);

Random
~~~~~~

Implementation name: :ccnsim:`ccn::cs::Random`

Usage example:

      .. code-block:: c++

         ccnHelper.SetContentStore ("ns3::ccn::cs::Random",
                                    "MaxSize", "10000");
	 ...
	 ccnHelper.Install (nodes);

.. note::

    If ``MaxSize`` parameter is omitted, then will be used a default value (100).

.. note::

    If ``MaxSize`` is set to 0, then no limit on ContentStore will be enforced

Nocache
~~~~~~~

:ccnsim:`Trivial implementation <ccn::cs::Nocache>` of the ContentStore that does not really do any caching.

Usage example:

      .. code-block:: c++

         ccnHelper.SetContentStore ("ns3::ccn::cs::Nocache");
	 ...
	 ccnHelper.Install (nodes);

Content stores with entry lifetime tracking
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to evaluate lifetime of the content store entries, the special versions of the content store need to be used:

Least Recently Used (LRU)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ccnsim:`ccn::cs::Stats::Lru`.

Usage example:

      .. code-block:: c++

         void
         CacheEntryRemoved (std::string context, Ptr<const ccn::cs::Entry> entry, Time lifetime)
         {
             std::cout << entry->GetName () << " " << lifetime.ToDouble (Time::S) << "s" << std::endl;
         }

         ...

         ccnHelper.SetContentStore ("ns3::ccn::cs::Stats::Lru",
                                    "MaxSize", "10000");
	 ...
	 ccnHelper.Install (nodes);

         // connect to lifetime trace
         Config::Connect ("/NodeList/*/$ns3::ccn::cs::Stats::Lru/WillRemoveEntry", MakeCallback (CacheEntryRemoved));


First-In-First-Out (FIFO)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ccnsim:`ccn::cs::Stats::Fifo`.

Usage example:

      .. code-block:: c++

         void
         CacheEntryRemoved (std::string context, Ptr<const ccn::cs::Entry> entry, Time lifetime)
         {
             std::cout << entry->GetName () << " " << lifetime.ToDouble (Time::S) << "s" << std::endl;
         }

         ...

         ccnHelper.SetContentStore ("ns3::ccn::cs::Stats::Fifo",
                                    "MaxSize", "10000");
	 ...
	 ccnHelper.Install (nodes);

         // connect to lifetime trace
         Config::Connect ("/NodeList/*/$ns3::ccn::cs::Stats::Fifo/WillRemoveEntry", MakeCallback (CacheEntryRemoved));

Random
~~~~~~

Implementation name: :ccnsim:`ccn::cs::Stats::Random`

Usage example:

      .. code-block:: c++

         void
         CacheEntryRemoved (std::string context, Ptr<const ccn::cs::Entry> entry, Time lifetime)
         {
             std::cout << entry->GetName () << " " << lifetime.ToDouble (Time::S) << "s" << std::endl;
         }

         ...

         ccnHelper.SetContentStore ("ns3::ccn::cs::Stats::Random",
                                    "MaxSize", "10000");
	 ...
	 ccnHelper.Install (nodes);

         // connect to lifetime trace
         Config::Connect ("/NodeList/*/$ns3::ccn::cs::Stats::Random/WillRemoveEntry", MakeCallback (CacheEntryRemoved));

.. _Content Store respecting freshness field of ContentObjects:

Content stores respecting freshness field of ContentObjects
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If simulations need Content Store which respects freshness of ContentObjects, the following versions of content store should be used:

.. note:

    Please note that currently, Freshness granularity is 1 second and maximum value is 65535 second. Value means infinity.

Least Recently Used (LRU)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ccnsim:`ccn::cs::Freshness::Lru`.

Usage example:

      .. code-block:: c++

         ...

         ccnHelper.SetContentStore ("ns3::ccn::cs::Freshness::Lru",
                                    "MaxSize", "10000");
	 ...

First-In-First-Out (FIFO)
~~~~~~~~~~~~~~~~~~~~~~~~~

Implementation name: :ccnsim:`ccn::cs::Freshness::Fifo`

Usage example:

      .. code-block:: c++

         ...

         ccnHelper.SetContentStore ("ns3::ccn::cs::Freshness::Fifo",
                                    "MaxSize", "10000");
	 ...

Random
~~~~~~

Implementation name: :ccnsim:`ccn::cs::Freshness::Random`

Usage example:

      .. code-block:: c++

         ...

         ccnHelper.SetContentStore ("ns3::ccn::cs::Freshness::Random",
                                    "MaxSize", "10000");
	 ...

Example
~~~~~~~

The following example demonstrates a basic usage of a customized content store (``ccn-simple-with-content-freshness.cc``).
In this scenario two simple consumers (both installed on a consumer node) continually request the same data packet.
When Data producer specify unlimited freshness, Content keeps getting satisfied from local caches, while if freshness is specified, Interests periodically are getting through to the Data producer.

.. aafig::
    :aspect: 60
    :scale: 120

      +----------+                +--------+                +----------+
      |          |     1Mbps      |        |      1Mbps     |          |
      | Consumer |<-------------->| Router |<-------------->| Producer |
      |          |         10ms   |        |         10ms   |          |
      +----------+                +--------+                +----------+


.. literalinclude:: ../../examples/ccn-simple-with-content-freshness.cc
    :language: c++
    :linenos:
    :lines: 20-27,43-

To run this scenario, use the following command::

        NS_LOG=DumbRequester:ccn.cs.Freshness.Lru ./waf --run=ccn-simple-with-content-freshness
