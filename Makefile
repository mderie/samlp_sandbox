
SOURCES = $(wildcard Source/*.cpp)

include ../../plugin.mk

dist: all
	mkdir -p dist/MidiMapper
	cp LICENSE* dist/MidiMapper/
	cp plugin.* dist/MidiMapper/
	cp -R res dist/MidiMapper/
