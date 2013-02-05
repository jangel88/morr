gampi
========

This code is part of a project to optimize task placement on compute nodes of Titan at
Oak Ridge Leadership Computing Facility. It implements a pure mutation
genetic algorithm on a distributed cluster using MPI communication. The
best result is collected on the root process and written to a file
'ga.out'. 
