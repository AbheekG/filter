CCFLAGS=-std=c++14
CC=g++

filter : filter.cc filter.hh motion_update.cc sensor_update.cc \
			ParticleFilter.cc ParticleFilter.hh
	$(CC) $(CCFLAGS) -o filter filter.cc motion_update.cc sensor_update.cc ParticleFilter.cc

.PHONY: clean

clean:
	rm -f filter

# filter: dtec.o model.o parser.o
# 	$(CC) -o dtec dtec.o model.o parser.o

# dtec.o: dtec.cc model.hh
# 	$(CC) $(CCFLAGS) -c dtec.cc

# model.o: model.cc model.hh
# 	$(CC) $(CCFLAGS) -c model.cc

# parser.o: parser.cc model.hh
# 	$(CC) $(CCFLAGS) -c parser.cc

# .PHONY: clean

# clean:
# 	rm -f *.o *~ dtec


# CPPFLAGS=-std=c++14
# SDIR=src
# CC=g++

# dtec: dtec.o model.o
# 	$(CC) -o dtec dtec.o model.o 

# dtec.o: $(SDIR)/dtec.cc
# 	$(CC) $(CPPFLAGS) -c dtec.cc

# model.o: $(SDIR)/model.cc $(SDIR)/model.hh
# 	$(CC) $(CPPFLAGS) -c model.cc