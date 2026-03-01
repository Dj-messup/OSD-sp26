CFLAGS = -Wall -Wextra -g -std=c11

all : forkgame

#run in container and use nice
run : all
	nice -n 10 ./forkgame --rounds=5 --strategy=RPSRP --prob-min=1e-6

forkgame : forkgame.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean :
	rm -f forkgame

.PHONY : all run clean