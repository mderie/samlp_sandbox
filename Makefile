
SOURCES = $(wildcard src/*.cpp)

include ../../plugin.mk

ifeq ($(OS),Windows_NT)
	FLAGS += -D__WINDOWS_MM__
	LDFLAGS += -lwinmm
else
	FLAGS += -D__LINUX_ALSA__
	LDFLAGS += -lpthread -lasound
endif

dist: all
	mkdir -p dist/MidiMapper
	cp LICENSE* dist/MidiMapper/
	cp plugin.* dist/MidiMapper/
	cp -R res dist/MidiMapper/
