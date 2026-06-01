const ex = require('express');
const hp = require('http');
const { Server } = require('socket.io');
const { spawn } = require('child_process');

const a = ex();
const s = hp.createServer(a);
const io = new Server(s, { cors: { origin: "*" } });


const p = spawn('./build/mcts_engine');


p.stderr.on('data', (data) => console.error(`[C++ ERROR]: ${data}`));
p.on('close', (code) => console.log(`[C++ PROCESS DIED] Code: ${code}`));

io.on('connection', (c) => {
    console.log("React UI Connected!");
    
    c.on('req', (d) => {
        console.log(`React sent move: ${d}`);
        p.stdin.write(d + "\n");
    });
});

p.stdout.on('data', (d) => {
    console.log(`C++ sent counter-move: ${d}`);
    io.emit('res', d.toString().trim());
});

s.listen(3000, () => console.log("Node Bridge Listening on port 3000"));