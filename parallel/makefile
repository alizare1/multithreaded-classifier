CC = g++ 
CFLAGS = -std=c++11 -Wall -Werror -g

all: PhonePricePrediction.out

PhonePricePrediction.out: main.cpp
	$(CC) $(CFLAGS) main.cpp -o PhonePricePrediction.out -lpthread


.PHONY: clean
clean:
	rm PhonePricePrediction.out