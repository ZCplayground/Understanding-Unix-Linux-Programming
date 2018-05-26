# server
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# connect to server
s.connect(('127.0.0.1', 9999))
# receive data
print(s.recv(1024).decode('utf-8'))
for data in [b'Chang', b'KangKang', b'Wangbin']:
    # sending data:
    s.send(data)
    print(s.recv(1024).decode('utf-8'))
s.send(b'exit')
s.close()
