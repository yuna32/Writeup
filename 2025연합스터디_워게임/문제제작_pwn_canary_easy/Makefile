CC := gcc
CFLAGS := -O2 -fno-pie -no-pie -fstack-protector-all -D_FORTIFY_SOURCE=0
LDFLAGS := -Wl,-z,relro,-z,now

all: pwn_canary_easy

pwn_canary_easy: pwn_canary_easy.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f pwn_canary_easy
