How to run:

First, make sure your geant environment is setup.
I put something like this in my bash_profile:

'''
alias setup_geant="source /path/to/your/Geant4-10.0.1/geant4make/geant4make.sh"
'''

Then you can setup the geant environment whenever you want by running `setup_geant`.

To build, you should be able to just run:
'''
$ make
'''

To run, edit a macro file (see run.mac for example), and do:
'''
$ ./bin/TestEM1 your_macro.mac
'''
