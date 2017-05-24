# map_analyse
Analyses maps created with ROS
Aim of this project is to define accuracy and recurrence of used mapping method

to run memory leak test: ctest -T memcheck

To install boost-program-options:
	Download from http://www.boost.org/users/history/version_1_64_0.html
	Unpack to /usr/local/lib/
	cd /usr/local/lib/boost_1_64_0
	./bootstrap.sh --with-libraries=program_options
	./b2

Minimum configuration to run:
	argument from command line with map path, e.g. -m ../maps/mapa.pgm

TEST
