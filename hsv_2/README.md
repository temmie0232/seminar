g++ -c hsv_filter.cpp -o hsv_filter.o
g++ -c main.cpp -o main.o
g++ hsv_filter.o main.o -o main

g++ hsv_filter.cpp main.cpp -o main

g++ hsv_filter.cpp main.cpp -o main && ./main