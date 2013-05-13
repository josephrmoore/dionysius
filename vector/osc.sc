OSCFunc.trace(false);
NetAddr.langPort // get current listening port



// 1.
(
var syn, sound;
syn = SynthDef.new("example3", {arg out = 0, freq = 440, amp = 0.2,
dur = 1;
var sin, env_gen, env;
env = Env.triangle(dur, amp);
env_gen = EnvGen.kr(env);
sin = SinOsc.ar(freq, mul: env_gen);
Out.ar(out, sin);
});
syn.load(s);
)

// 2.
thisProcess.openUDPPort(5456); // see if port is available
n = NetAddr.new("127.0.0.1", 5456);    // create the NetAddr
o = OSCFunc({ arg msg, time, addr, recvPort; [msg, time, addr, recvPort].postln;}, '/playtone', n);
(
f = { |msg, time, replyAddr, recvPort|
    if(msg[0] == '/playtone') {
		Synth.new("example3",[\freq, msg[1], \amp, msg[2], \dur, msg[3]]);
    };
};
thisProcess.addOSCRecvFunc(f);
);

// stop posting.
thisProcess.removeOSCRecvFunc(f);





