import socket,threading
from aether_gateway import frame,parse,PING,PONG,HELLO,JOB,RESULT

assert parse(frame(PING,7,b"ping"))==(PING,7,b"ping")
assert parse(frame(HELLO,8,b"hello"))==(HELLO,8,b"hello")
bad=bytearray(frame(PING,9,b"x"));bad[-1]^=1
try:
    parse(bytes(bad)); raise AssertionError("CRC failure accepted")
except ValueError:
    pass

def one_shot(port,expected_kind):
    s=socket.socket();s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1);s.bind(("127.0.0.1",port));s.listen(1)
    def run():
        c,_=s.accept()
        try:
            k,i,p=parse(c.recv(4096))
            out=frame(PONG,i,b"PONG") if k==PING else frame(RESULT,i,b"JOB-ACCEPTED:"+p[:180])
            c.sendall(out)
        finally:
            c.close();s.close()
    t=threading.Thread(target=run,daemon=True);t.start()
    c=socket.create_connection(("127.0.0.1",port),2)
    payload=b"ping" if expected_kind==PING else b"QPU:bell"
    c.sendall(frame(expected_kind,11,payload)); data=c.recv(1024); c.close(); t.join(2)
    return parse(data)

assert one_shot(18765,PING)==(PONG,11,b"PONG")
assert one_shot(18766,JOB)==(RESULT,11,b"JOB-ACCEPTED:QPU:bell")
print("AETHER GATEWAY PROTOCOL HIL: PASS")
