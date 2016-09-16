# cs430_project1
#README
created by steven gruenewald

input.ppm is a p3 format image i created
input6.ppm is a p6 format.

ppmrw reads in the file and gathers all the header information then saves them
to global variables. then calls upon a buffer to read in the data of the PPM
file to a struct in memory depending on its format.

then the file writes to output.ppm depending on the argv input and then the 
program is finished.
