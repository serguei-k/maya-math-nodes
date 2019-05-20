.. Maya Math Nodes documentation master file, created by
   sphinx-quickstart on Mon Jul 30 02:22:57 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Maya Math Nodes Documentation
========================================

.. toctree::
   :maxdepth: 1
   :caption: User Documentation

   node_reference
   expression_language

Maya Math Nodes is a plugin for Autodesk Maya that provides a set of atomic nodes to perform various common math operations.
The purpose of these nodes is to streamline the creation of complex and highly performant rigging systems.

To see the list of nodes made available by the plugin, please refer to the :ref:`Node Reference <node_reference>` section.

Additionally, this plugin provides a simple expression language that can be used to describe a series of mathematical operations inline, which can then be interpreted to generate a math node network for you, see :ref:`Expression Language <expression_language>` section for details.

.. note::
   At this time there are no distributable binaries available for download.
   However, it is fairly easy to build it directly from the `source code <https://github.com/serguei-k/maya-math-nodes>`_.
