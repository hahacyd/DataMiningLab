ifndef GXX
	GXX := g++
endif

ifndef CXXFLAGS
	CXXFLAGS := -std=c++11 -g -O2 #-pg   # 
endif

CxxSources := $(shell find ./ -name "*.cpp" ) #自动寻找当前目录下的源文件

Objs := $(CxxSources:.cpp=.o)

analysize:$(Objs)
	$(GXX)	-o $@ $(CXXFLAGS)  $(Objs)

run:analysize
	./analysize
-include $(CxxSources:.cpp=.d)

%.d : %.cpp
	rm -f $@; \
	$(GXX) -MM $(INCLUDE) $< > $@.$$$$; \
	sed -e 's,^.*:,$*.o $@:,g'  < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY : clean

clean:
	@echo $(CxxSources)
	rm $(CxxSources:.cpp=.d) $(CxxSources:.cpp=.o) analysize