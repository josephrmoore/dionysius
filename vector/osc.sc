OSCFunc.trace(true);
OSCFunc.trace(false);
NetAddr.langPort // get current listening port


// 1.
(
var syn, sound;
syn = SynthDef.new("oskar", {arg out = 0, freq = 440, amp = 0.2, dur = 1, type = 0, en = 0, attack = 0.1, decay = 1, sustain = 1, release = 1, curve = -4;
var sin, saw, noi, env_gen, env;
    env = Env.perc(attack, release, amp, curve);
	if (en == 2){
        env = Env.triangle(dur, amp);
	};
	if (en == 1){
		env = Env.adsr(attack, decay, sustain, release, amp, curve);
	};
	env_gen = EnvGen.kr(env);
	sin = SinOsc.ar(freq, mul: env_gen);
	saw = Saw.ar(freq, mul: env_gen);
	noi = WhiteNoise.ar(freq, mul: env_gen);
Out.ar(out, sin);
Out.ar(out, saw);
Out.ar(out, noi);

});
syn.load(s);
)




// most basic synthdef

(
SynthDef("joe", {arg out=0;
	  // logic here
	var sine = SinOsc.ar(440, 0, 1);
	Out.ar(out, sine);
}).add;
)


Synth("joe");

// 2.
thisProcess.openUDPPort(5456); // see if port is available
n = NetAddr.new("127.0.0.1", 5456);    // create the NetAddr
o = OSCFunc({ arg msg, time, addr, recvPort; [msg, time, addr, recvPort].postln;}, '/playtone', n);
(
f = { |msg, time, replyAddr, recvPort|
    if(msg[0] == '/playtone') {
		Synth.new("oskar",[\freq, msg[1], \amp, msg[2], \dur, msg[3], \type, msg[4], \e, msg[5], \attack, msg[6], \decay, msg[7], \sustain, msg[8], \release, msg[9], \curve, msg[10]]);
    };
};
thisProcess.addOSCRecvFunc(f);
);

// stop posting.
thisProcess.removeOSCRecvFunc(f);







