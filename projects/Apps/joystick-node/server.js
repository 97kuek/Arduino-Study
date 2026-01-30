const express = require('express');
const http = require('http');
const { Server } = require("socket.io");
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.static('public'));

// シリアルポート設定
const PORT_NAME = 'COM3';
const BAUD_RATE = 9600;

// 利用可能なポートを表示
SerialPort.list().then(ports => {
    console.log('Available Serial Ports:');
    ports.forEach(port => {
        console.log(` - ${port.path}\t${port.pnpId || ''}\t${port.manufacturer || ''}`);
    });
    console.log(`Attempting to connect to ${PORT_NAME}...`);

    // ポート接続試行
    try {
        const port = new SerialPort({ path: PORT_NAME, baudRate: BAUD_RATE });
        const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

        port.on('open', () => {
            console.log(`Serial Port ${PORT_NAME} opened at ${BAUD_RATE} baud`);
        });

        port.on('error', (err) => {
            console.error('Error: ', err.message);
            console.log('Please check your PORT_NAME in server.js');
        });

        parser.on('data', (data) => {
            try {
                const jsonData = JSON.parse(data);
                io.emit('joystick-data', jsonData);
            } catch (e) {
                // JSONパースエラーは無視 (起動直後などの不完全なデータ)
                // console.error('Data parse error:', e); 
            }
        });

    } catch (err) {
        console.error(`Failed to create SerialPort: ${err.message}`);
    }
});

io.on('connection', (socket) => {
    console.log('A user connected');
});

server.listen(3000, () => {
    console.log('Listening on *:3000');
});
