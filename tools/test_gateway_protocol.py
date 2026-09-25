import socket,subprocess,sys,time
from aether_gateway import frame,parse,PING,PONG,HELLO,JOB,RESULT
assert parse(frame(PING,7,b"ping"))==(PING,7,b"ping")
assert parse(frame(HELLO,8,b"hello"))==(HELLO,8,b"hello")
bad=bytearray(frame(PING,9,b"x"));bad[-1]^=1
try: parse(bytes(bad)); raise AssertionError("CRC failure accepted")
except ValueError: pass
p=subprocess.Popen([sys.executable,"aether_gateway.py","18765"],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
try:
    time.sleep(.4)
    s=socket.create_connection(("127.0.0.1",18765),2);s.sendall(frame(PING,11,b"ping"));data=s.recv(1024);s.close()
    assert parse(data)==(PONG,11,b"PONG")
    s=socket.create_connection(("127.0.0.1",18765),2);s.sendall(frame(JOB,12,b"QPU:bell"));data=s.recv(1024);s.close()
    assert parse(data)==(RESULT,12,b"JOB-ACCEPTED:QPU:bell")
finally:
    p.terminate();p.wait(timeout=3)
print("AETHER GATEWAY PROTOCOL HIL: PASS")
