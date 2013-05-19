// OSCFunc.trace(true);
// OSCFunc.trace(false);
// NetAddr.langPort // get current listening port
//

// 1.
(
var syn, sound;
syn = SynthDef.new("oskar", {arg out = 0, freq = 440, amp = 1, dur = 1, size = 50;
var sin, saw, noi, env_gen, env, env2, env_gen2;
	env = Env.perc(1, 1, (amp*(size/100)), -2);
	env2 = Env.perc(1, 1, (amp*((100-size)/100)), -2);
	env_gen = EnvGen.kr(env);
	env_gen2 = EnvGen.kr(env2);
	sin = SinOsc.ar(freq, mul: env_gen);
	saw = Saw.ar(freq, mul: env_gen2);
	Out.ar(out, sin);
	Out.ar(out, saw);
});
syn.load(s);
)

(
var syn, sound;
syn = SynthDef.new("noise", {arg out = 0, brown = 1, log = 1, lf = 1;
var env_gen, env_gen2, env_gen3, env, env2, env3, b, l, f;
	env = Env.perc(1, 1, (brown/255)*0.1, -2);
	env2 = Env.perc(1, 1, (log/255)*0.1, -2);
	env3 = Env.perc(1, 1, (lf/255)*0.1, -2);
	env_gen = EnvGen.kr(env);
	env_gen2 = EnvGen.kr(env2);
	env_gen3 = EnvGen.kr(env3);
	b = { BPF.ar(BrownNoise.ar(0.1.dup), mul:env_gen) };
	l = { Logistic.ar(rrand(3, 3.99), mul:env_gen2) };
	f = { LFNoise2.ar(mul:env_gen3) };
	Out.ar(out, b);
	Out.ar(out, l);
	Out.ar(out, f);
});
syn.load(s);
)



// 2.




(
					var freq=rrand(50,560.3);
					var numcps=rrand(2,20);
var b, c, d, e;

b = { GbmanL.ar(MouseX.kr(1,20)) * 0.1 };

c = {
	var source, line, filter;

	source=LFNoise0.ar(400);
	line=Line.kr(10000,1000,10);
	filter=Resonz.ar(source,line,0.1);

	filter

};

d = {
	var n=2;

	Resonz.ar(
			Mix.fill(n,{



					Pan2.ar(Gendy1.ar(6.rand,6.rand,1.0.rand,1.0.rand,freq, freq, 1.0.rand, 1.0.rand, numcps, SinOsc.kr(exprand(0.02,0.2), 0, numcps/2,numcps/2), 0.5/(n.sqrt)),1.0.rand2)
			}),

			MouseX.kr(100,2000),
			MouseY.kr(0.01,1.0)

			);

};

e = { SyncSaw.ar(MouseX.kr(1,400),MouseY.kr(400,800),0.1)};

thisProcess.openUDPPort(5456); // see if port is available
n = NetAddr.new("127.0.0.1", 5456);    // create the NetAddr
o = OSCFunc({ arg msg, time, addr, recvPort; [msg, time, addr, recvPort].postln;}, '/playtone', n);
(



f = { |msg, time, replyAddr, recvPort|


    if(msg[0] == '/dionysius_background') {
		if(msg[1] == 0){
			b.play.free;
		};
		if(msg[1] == 1){
			c.play.free;
		};
		if(msg[1] == 2){
			d.play.free;
		};
		if(msg[1]>=3){
			e.play.free;
		};
		"playing".postln;
    };
	    if(msg[0] == '/dionysius_sound') {
		var freq = msg[1];
		var dur = msg[2];
		var amp = msg[3];
		var size = msg[4];
		Synth.new("oskar",[\freq, msg[1], \amp, msg[2], \dur, msg[3], \size, msg[4]]);
	};
	    if(msg[0] == '/dionysius_noise') {
		var brown = msg[1];
		var log = msg[2];
		var lf = msg[3];
		Synth.new("noise",[\brown, brown, \log, log, \lf, lf]);
	};
};
thisProcess.addOSCRecvFunc(f);
);
)

// stop posting.
thisProcess.removeOSCRecvFunc(f);







