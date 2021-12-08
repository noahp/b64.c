
CFLAGS += -Wall -Wextra #-fsanitize-address-use-after-scope -fsanitize=address,undefined

OBJS = fuzz.o decode.o encode.o buffer.o

fuzz: CFLAGS+=-fsanitize=address,undefined,fuzzer -DFUZZING
fuzz: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
