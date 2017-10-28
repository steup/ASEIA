**A**bstract **Se**nsor **I**nformation **A**rchitecture (ASEIA)
========================================================

[![Build Status](https://travis-ci.org/steup/ASEIA.svg?branch=master)](https://travis-ci.org/steup/ASEIA)

Goal
----

This library provides the baseline facilities to extend any *Publish/Susbcribe* system to use *Abstract Sensor Events* as a machine readable interface between individually developed components.

Features
--------

**ASEIA** provides the following features on the *Abstract Sensor Events*:

* Statically typed events (*Event*) to provide easy to use interfaces
* Dynamically typed Events (*MetaEvent*)to provide flexible computations
* *Transformer* abstraction to provide often used computations in general way
* KnowlegdeBase System to automatically search fitting *Transformations* based on existing and requested *EventTypes*
* Virtual Arithmetics to provide an abstract computation framework to be used by the *Transformations* (*MetaValue*)
