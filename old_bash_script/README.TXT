##########################################################################
			DDM: Data Distribution Management

			    http://pads.cs.unibo.it
##########################################################################

This archive contains all the source code, the shell scripts, the raw
data obtained in the experiments execution and the graph sources used 
in the paper: 

  M. Marzolla, G. D'Angelo, M. Mandrioli, A Parallel Data Distribution
  Management Algorithm, to appear in proc. IEEE/ACM International
  Symposium on Distributed Simulation and Real Time Applications
  (DS-RT 2013), oct 30—nov 1, 2013, Delft, the Netherlands

More in detail:

1) sort_matching_standard/

Contains a slightly modified version of the Sort Matching DDM algorithm.
The original version of this implementation can be found at:
https://github.com/zAvo/sort-matching

The Sort Matching DDM source code is distributed under the GNU Affero 
General Public License.

2)  interval_tree/

Contains a preliminary implementation of the Interval Tree Matching (ITM) 
DDM algorithm that is described in the paper. 

3) _graphs/

Contains the raw data and gnuplot sources for building all the graphs 
shown in the paper.

4) _utils/

Contains the source code of some support utils used for data extraction.

5) scripts in the root folder

Are used for the batch evaluation of the DDM algorithms.

--------------------------------------------------------------------------
All the resources described in 2), 3), 4) and 5) are distributed under 
the GNU General Public License version 3 (GPLv3)
--------------------------------------------------------------------------
