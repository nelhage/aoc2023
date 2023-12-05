CXXFLAGS=-std=c++20  -fbracket-depth=25000
OUT=out

$(OUT)/%.i: %.txt
	mkdir -p $(OUT)
	xxd -i < $< > $@

HEADERS := list.h read_input.h

PART1_INPUT = test

$(OUT)/part1.o: part1.cc $(HEADERS) $(OUT)/$(PART1_INPUT).i $(OUT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OUT)/part1.o: CPPFLAGS=-DINPUT=\"$(OUT)/$(PART1_INPUT).i\"
