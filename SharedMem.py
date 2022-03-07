import sharedmem as sm

class SharedMemError(OSError):
    pass

class SharedMem:

    shmid=0
    size=0

    def __init__(self,size:int) -> None:
        if size<=0:
            raise SharedMemError("Size must be a positive number")
        self.shmid=sm.create(size)
        self.size=size
        if self.shmid<=0:
            raise SharedMemError("Shared memory creation failed")
    
    def readbyte(self) -> int:
        if (ret:=sm.readbyte(self.shmid))==-1:
            raise SharedMemError("Shared memory read failed")
        return ret
    
    def readword(self) -> int:
        if self.size<2:
            raise SharedMemError("Shared memory too small")
        if (ret:=sm.readword(self.shmid))==-1:
            raise SharedMemError("Shared memory read failed")
        return ret
    
    def readdword(self) -> int:
        if self.size<4:
            raise SharedMemError("Shared memory too small")
        if (ret:=sm.readdword(self.shmid))==-1:
            raise SharedMemError("Shared memory read failed")
        return ret
    
    def readbytes(self) -> str:
        string=""
        if (string:=sm.readbytes(self.shmid))==-1:
            raise SharedMemError("Shared memory read failed")
        return string
    
    def writebyte(self,byte:int) -> None:
        if sm.writebyte(self.shmid,byte)==-1:
            raise SharedMemError("Shared memory write failed")

    def writebytes(self,bytes:str) -> None:
        if len(bytes)>self.size:
            raise SharedMemError("Too many bytes to write")
        if sm.writebytes(self.shmid,bytes)<0:
            raise SharedMemError("Shared memory write failed")
    
    def writeword(self,word:int) -> None:
        if self.size<2:
            raise SharedMemError("Shared memory too small")
        if sm.writeword(self.shmid,word)==-1:
            raise SharedMemError("Shared memory write failed")
    
    def writedword(self,dword:int) -> None:
        if self.size<4:
            raise SharedMemError("Shared memory too small")
        if sm.writedword(self.shmid,dword)==-1:
            raise SharedMemError("Shared memory write failed")
    
    def getshmid(self) -> int:
        return self.shmid
    
    def __del__(self):
        if self.shmid<=0:
            return
        if sm.delete(self.shmid)==-1:
            raise SharedMemError("Shared memory close failed")
