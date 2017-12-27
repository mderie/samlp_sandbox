Welcome to MidiMapper a plugin for VCV Rack - A bunch of C++ lines coded while the children sleep...



NB : I didn't release anything since decades, hope you'll like it as much as I had fun write it !-)

The project aims to map rotary buttons of "exotic" midi device into buttons inside the any rack plugins.

Motivation : since I'm the proud owner of a BCR2000 from Behringer (32 endless rotary buttons among other nice feature),
I wanna to freely assign any button to any "pot" inside the rack whatever the target module.

==> Mission accomplished, it is done & it works ! Warning : it is a POC, ie : it lacks try...catch !

The idea is to put the debug mode to 1 in order to check how the RtMidi tiny library names your midi in device.
Put this in the MidiMapper.ini. Then for each module that you want to control externally, well you have to find
out the control index and edit the same configuration file accordingly. No midi learn :(
Some controls work with signed values : ie VCO.2 the frequency, goes from -54.000000 to 54.000000.
In this case just add its index in the SignFactor with the 1 flag.
Don't forget to put back debug mode to 0 else you will explode your disk space !

Many thanks to the git "initiative" (the project is here http://github.com/mderie/MidiMapper)
and obviously to the VCV Rack author (I've started to write a software modular synth, but then I discover VCV Rack :)
I took also the liberty to reuse part of the AV500 switch module...

Last but not least, I'm reachable here : mderie@gmail.com
(Sam Le Pirate member of TFL-TDV on the demoscene, but who cares ?-)

Christmas 2017 update : support of the midi learn mode
------------------------------------------------------

Eternal thanks goes to my friend VCO (private joke here !-) who helps with this
Actually I adapt his code into mine in order to perform the midi mapping
right inside VCV rack ! Then everything is saved into a new file called
"LastSessionParameters.ini" which can merged to "MidiMapper.ini"
by cherry picking the content of the "Mapping" section :)

==> Again I've achieved my personal goal...
I know that there is here and there some false assumptions (ie : dangling pointer :(