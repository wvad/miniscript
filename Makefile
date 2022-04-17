SRCDIR  = ./src
OBJDIR  = ./out

SOURCES  = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS  = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
INCLUDE  = -I ./include
DEPENDS  = $(OBJECTS:.o=.d)

app.exe: $(OBJECTS)
	g++ -static-libstdc++ -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ -pthread -std=c++17 $(INCLUDE) -o $@ -c $<

-include $(DEPENDS)
