all: bbst

bbst: bbst.cpp
	g++ -std=c++0x -o bbst bbst.cpp 
	@echo Compilation Successful!

clean: 
	rm bbst
	@echo All programs removed. Compile again.
	
