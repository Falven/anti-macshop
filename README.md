# anti-macshop
###Program to remove the anti-macshop malware.

Francisco Aguilera

##Building:
The project uses CMake to generate a Makefile. The provided Makefile should work fine on department machines, however, in case you would like to run the program elsewhere, or re-generate the Makefile:

out-of-source: Create a directory somewhere outside the source to hold the build files.
`mkdir ~/Desktop/anti-macshop-build`

Then simply run Cmake with the proper generator and source directory.
`cmake -G "Unix Makefiles" ~/Desktop/anti-macshop-build/`

Change directories to the build tree.
`cd ~/Desktop/anti-macshop-build/`

Make the project.
`make`

##Invocation:

if not built, make the file executable by all

`chmod a+x rm-machsop`

`./rm-machsop`
