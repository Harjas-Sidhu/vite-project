import{ createServer } from "http";
import { exec } from "child_process";
import { networkInterfaces } from 'os';

const nets = networkInterfaces();
const results = Object.create(null); 

for (const name of Object.keys(nets)) {
    for (const net of nets[name]) {
        const familyV4Value = typeof net.family === 'string' ? 'IPv4' : 4
        if (net.family === familyV4Value && !net.internal) {
            if (!results[name]) {
                results[name] = [];
            }
            results[name].push(net.address);
        }
    }
}

const run = () => {
    exec("C:\\Users\\harja\\OneDrive\\Desktop\\server-practice\\vite-project\\src\\assets\\core\\simulator_random.exe", (stderr) => {
        if(stderr){
            console.log(stderr);
        }
    })
}

const requestListener = (req, res) => {
    res.setHeader("Access-Control-Allow-Origin", "*");
    res.setHeader("Access-Control-Allow-Methods", "GET");
    res.setHeader("Access-Control-Allow-Headers", "Content-Type");

    if (req.method === "OPTIONS") {
        res.writeHead(204);
        res.end();
        return;
    }

    res.writeHead(200, { 'Content-Type': 'text/csv' });
    run();
    res.end("hello");
};

const server = createServer(requestListener);
server.listen(8000, results["Wi-Fi"][0], () => {
    console.log(`Server is running on http://${results["Wi-Fi"][0]}:8000`);
});