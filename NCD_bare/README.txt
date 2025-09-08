SNOLAB Bubble Detector Spectrometer Simulation - Jeffrey Glasier
----------------------------------------------------------------

1. PURPOSE
----------------------------------------------------------------
	The SNOLAB BDS Simulation was made with the intent to recreate 
the data taken by the real bubble detector spectrometers at the SNOLAB 
underground facility, then determine the conditions required to recreate 
that data.

	Specifically, the goal is to be a tool able to provide the framework 
to create a variety of simulations with minimal coding which can be used to:

-Determine the effeciencies for each energy of neutron with the detector in its
current volume

-vary the neutron spectra being ejected from the walls of the facility until 
the neutron fluence output matches the collected real data.

-Hit detectors with neutrons from various sources in order to determine where
the neutrons in our real data are derived.

-Determine the amount of shielding occuring from the detectors and 
from the acrylic lid.


This will give us both an updated measurement of the neutron flux coming from 
the walls of the facility which SNOLAB users/experiments can use when designing
shielding and programming simulations.

2.How effeciencies apply to the BDS
---------------------------------------------------------------
	Our experiment looks to determine not only the ambient neutron flux
but also the neutron flux coming out of mine wall. Say we wanted a reading of
neutrons per one metre squared of wall over a time period,
we would need a detector that fully covers a meter squared of wall and only interacts
with neutrons from that meter squared of wall while interacting with every neutron
from that meter squared of wall to get such a result without needing corrections.

	If we now consider that meter squared of wall to be a sphere and our
detector to be an inner sphere that completely covers the surface area of the rock
we will have perfect effeciency, our detector will record every neutron emerging 
from the rock. As that inner sphere (the detector), decreases in size, less and 
less neutrons will interact with it, but you can still determine the amount of
neutrons coming from the world volume by instead using an effeciency correction.

	The BDS at SNOLAB are the equivalent of a small detector completely surrounded
by a large world geometry of mine rock with neutrons shooting inwards. To get our
measurement, we need to determine the effeciency of our detectors for any room they
are placed in.	


3. MAIN
----------------------------------------------------------------
	The main file (slbds.cc) manages the overview of the simulation.
Runs, visuals, UI, and geometry are both initialized and terminated there.
It also contains the macro that runs when the green button is pressed on
the UI.


4. GEOMETRY
----------------------------------------------------------------
	Geometry is the pressure chamber with all BDS detectors contained
in a spherical world volume with particles entering from random positions and
inward angles on from the edge of the world volume.

5. PHYSICS
----------------------------------------------------------------
	The physics of the simulation are declared in the physics list.
This physics list is an altered version of that seen in the 
geant4/examples/extended/hadronic/NeutronSource. This list was chosen for
its implementation of the NeutronHP physics, which considers individual
particles rather than neutron radiation averages.

6. SENSITIVE DETECTOR HITS
----------------------------------------------------------------
	Currently the sensitive detector hits are set to record
hits when neutrons pass through any BDS detector, the hits export the 
event number and neutron energy to a root ntuple with branches in the 
fast neutron and thermal neutron ranges

7. HISTOGRAMS
----------------------------------------------------------------
	Histograms are stored in root Ntuples, ideally a macro for 
generating root plots is written for the program in the near future.

8. How to Run
----------------------------------------------------------------
	1. Make a build directory
	2. cmake ..
	3. make
	4. ./slbds
	5. /run/beamOn n_events (in UI window)
	6. view the events in dawnfile
	7. Close dawnfile and your UI
	8. root NeutronFluence.root
	9. new TBrowser (you can use whatever method of viewing ntuples you like)
	10. click on the branch you wish to review

9. How to Perform Specific BDS Simulations
----------------------------------------------------------------
	Begin by making a copy of the code. Then following the below
steps depending on the desired information.

Effeciencies
-----------

Data Matching
-----------

Neutron Sources
-----------

Lid Shielding
-----------


10. REFERENCES
----------------------------------------------------------------
	Various parts of this simulation were made using the official 
geant4 examples, the PhysicsMatters:geant4 tutorial series, and the 
Geant 4 Application Developers Guide as reference.
