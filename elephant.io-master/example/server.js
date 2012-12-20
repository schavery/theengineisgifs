var io = require('C:\\Users\\guy-pod\\node_modules\\socket.io').listen(61001);

io.sockets.on('connection', function (socket) {
  console.log('user connected');

  socket.on('action', function (data) {
    console.log('here we are in action event and data is: ' + data);
  });
});