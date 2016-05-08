IDIR       = .
ODIR       = obj
SDIR       = .

CXX        = g++

#CXXFLAGS  += -I. 
CXXFLAGS  += -I. -std=c++0x
## Optimization flag
CXXFLAGS += -g #-O3
## Enable the maximun warning
CXXFLAGS += -Wall -Wextra -Weffc++ -g

## Include ROOT
CXXFLAGS  += $(shell root-config --cflags)

CXXDEPFLAGS = -MMD -MP

LD         = g++
LDFLAGS    =

LIBS       = $(shell root-config --glibs) -lgsl

#OBJS       = $(patsubst %, $(ODIR)/%, $(OBJ))

PROGRAMS = RobotCodeFinal

all: mkobj $(PROGRAMS)


mkobj:
	@mkdir -p obj


$(ODIR)/%.o : $(SDIR)/%.C
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

RobotCodeFinal: $(ODIR)/RobotCodeFinal.o
	$(LD) $^ $(LIBS) -o $@

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.so $(ODIR)/*.d $(PROGRAMS) core 

-include $(ODIR)/*.d
