ifndef GXX
	GXX := g++
endif

ifndef CXXFLAGS
	CXXFLAGS :=   -std=c++11 -pg -g -O2 # 
endif

CxxSources := $(shell find ./ -name "*.cpp" ) #自动寻找当前目录下的源文件

Objs := $(CxxSources:.cpp=.o)

analysize:$(Objs)
	$(GXX)	-o $@ $(CXXFLAGS)  $(Objs)

-include $(CxxSources:.cpp=.d)

%.d : %.cpp
	rm -f $@; \
	$(GXX) -MM $(INCLUDE) $< > $@.$$$$; \
	sed -e 's,^.*:,$*.o $@:,g'  < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY : clean

clean:
	@echo $(CxxSources)
	rm build $(CxxSources:.cpp=.d) $(CxxSources:.cpp=.o) 