#!/usr/bin/env python3
"""Reference AetherOS gateway + protocol/HIL test server. Standard library only."""
import socket,struct,threading,sys,zlib
MAGIC=0x41544852; VER=1
HELLO,HELLO_ACK,PING,PONG,AUTH,JOB,RESULT,TELEMETRY,QUEUE,ERROR=range(1,11)
HDR="<IHHIII"
def crc32(b): return zlib.crc32(b)&0xffffffff
def frame(kind,i,payload=b""): return struct.pack(HDR,MAGIC,VER,kind,i,len(payload),crc32(payload))+payload
def parse(b):
    if len(b)<20: raise ValueError("short")
    m,v,k,i,n,c=struct.unpack(HDR,b[:20])
    if m!=MAGIC or v!=VER or n!=len(b)-20 or crc32(b[20:])!=c: raise ValueError("bad frame")
    return k,i,b[20:]
def serve(conn,addr):
    try:
        data=conn.recv(4096); k,i,p=parse(data)
        if k==PING: out=frame(PONG,i,b"PONG")
        elif k==HELLO: out=frame(HELLO_ACK,i,b"AETHER-GATEWAY/1")
        elif k==AUTH: out=frame(RESULT,i,b"AUTH-OK")
        elif k==JOB: out=frame(RESULT,i,b"JOB-ACCEPTED:"+p[:180])
        else: out=frame(RESULT,i,b"OK")
        conn.sendall(out)
    except Exception as e:
        try: conn.sendall(frame(ERROR,1,str(e).encode()[:120]))
        except Exception: pass
    finally: conn.close()
def main(port=8765):
    s=socket.socket();s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1);s.bind(("0.0.0.0",port));s.listen(4)
    print("Aether gateway listening on",port,flush=True)
    while True:
        c,a=s.accept();threading.Thread(target=serve,args=(c,a),daemon=True).start()
if __name__=="__main__": main(int(sys.argv[1]) if len(sys.argv)>1 else 8765)
