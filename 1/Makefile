CXXFLAGS=-std=c++20  -fbracket-depth=25000
OUT=out

$(OUT)/%.i: %.txt
	mkdir -p $(OUT)
	xxd -i < $< > $@

HEADERS := list.h read_input.h

INPUT = test

$(OUT)/_input:
	@ if test -f $@ && [ $$(cat $@) = $(INPUT) ]; then \
		:; else echo $(INPUT) > $@; fi

.PHONY: $(OUT)/_input

$(OUT)/%.o: %.cc $(HEADERS) $(OUT)/$(INPUT).i $(OUT)/_input
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OUT)/%.o: CPPFLAGS=-DINPUT=\"$(OUT)/$(INPUT).i\"
