BINARY=terminalwrap

.PHONY: all build install clean

all: build

build:
	go build -o $(BINARY) main.go

install: build
	install -Dm755 $(BINARY) /usr/bin/$(BINARY)

clean:
	rm -f $(BINARY)