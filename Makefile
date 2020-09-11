PREFIX=/tmp/snes-sdk
OPTIMIZE=0
BIN?=bin
FINAL_ROM=$(BIN)/BeagleSatellaTest.sfc

CFLAGS=-Wall -I$(PREFIX)/include
ifeq ($(OPTIMIZE),1)
CFLAGS += -O
endif

BINDIR=$(PREFIX)/bin
AS=$(BINDIR)/wla-65816
LD=$(BINDIR)/wlalink
CC=$(BINDIR)/816-tcc

LIBDIR=$(PREFIX)/lib

SRC = data.asm main.c input.c init.c graph.c str.c screen1.c screen2.c

all: $(FINAL_ROM)

$(FINAL_ROM): $(SRC:%=$(BIN)/%.o)
	@mkdir -p $(dir $@)
	$(LD) -dSo $^ "$@"

$(BIN)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I. -o "$(BIN)/$*.c.s" -c $<
	$(AS) -io "$(BIN)/$*.c.s" "$@"
	@gcc -MM -MT "$@" -I. -o "$(BIN)/$*.dep" -c $<

$(BIN)/%.asm.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) -io $< $@

clean:
	rm -rf $(BIN)

-include $(shell test -d $(BIN) && find $(BIN) -name "*.dep")
