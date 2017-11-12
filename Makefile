
SOURCES = $(wildcard Source/*.cpp)

include ../../plugin.mk

FLAGS += -D__LINUX_ALSA__
LDFLAGS += -lpthread -lasound

dist: all
	mkdir -p dist/MidiMapper
	cp LICENSE* dist/MidiMapper/
	cp plugin.* dist/MidiMapper/
	cp -R res dist/MidiMapper/
