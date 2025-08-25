# Makefile
BIN=challenge

# 보안 설정:
# -fPIE -pie         : PIE 활성화
# -fno-stack-protector: 스택 카나리 비활성화
# -Wl,-z,relro,-z,now : Full RELRO (GOT overwrite 불가)
# -no-pie 는 사용하지 않음 (PIE 켜야 함)
CFLAGS=-O0 -g -fno-omit-frame-pointer -fno-stack-protector -fPIE
LDFLAGS=-pie -Wl,-z,relro,-z,now

all: $(BIN)

$(BIN): challenge.c
\tgcc $(CFLAGS) challenge.c -o $(BIN) $(LDFLAGS)

clean:
\trm -f $(BIN)

.PHONY: all clean
