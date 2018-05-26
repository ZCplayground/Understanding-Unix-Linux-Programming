# server
# This server receives client's input and send "Hello, XXX" back.

import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# binds a addr(ipv4addr, port number)
# 127.0.0.1 is a special ipv4 addr means local. So the client process must in the same computer.
s.bind(('127.0.0.1', 9999))

# listen() to listen the port. parameter means the max size of queue.
s.listen(5)
print('Waiting for connection...')

import threading
import time
def tcplink(sock, addr):
    print('Accept new connection from %s:%s...' % addr)
    sock.send(b'Welcome!')
    while True:
        data = sock.recv(1024)
        time.sleep(1)
        if not data or data.decode('utf-8') == 'exit':
            break
        sock.send(('Hello, %s!' % data.decode('utf-8')).encode('utf-8'))
    sock.close()
    print('Connection from %s:%s closed.' % addr)

# accept() will block until a client socket connect(). Return (ipv4addr, port) of client.
# When successfully accept a link, create a new process or thread to do with it.
while True:
    sock, addr = s.accept()
    t = threading.Thread(target=tcplink, args=(sock, addr))
    t.start()
