#include "Python.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>



static PyObject* create(PyObject* self, PyObject* args)
{
    int n;
    if (!PyArg_ParseTuple(args,"i",&n))
        return Py_BuildValue("i",-1);
    int shmid = shmget(IPC_PRIVATE,n,0640|IPC_CREAT);
    return Py_BuildValue("i",shmid);
}
static PyObject* writebyte(PyObject* self, PyObject* args)
{
    int shmid;
    unsigned char byte;
    if (!PyArg_ParseTuple(args,"iB",&shmid,&byte))
        return Py_BuildValue("i",-1);
    char* ptext = (char *)shmat(shmid, 0, 0);
    if (ptext==NULL)
    {
        return Py_BuildValue("i",-1);
    }
    *ptext=byte;
    return Py_BuildValue("i",0);
}
static PyObject* writeword(PyObject* self, PyObject* args)
{
    int shmid;
    unsigned short word;
    if (!PyArg_ParseTuple(args,"iH",&shmid,&word))
        return Py_BuildValue("i",-1);
    short* ptext = (short *)shmat(shmid, 0, 0);
    if (ptext==NULL)
    {
        return Py_BuildValue("i",-1);
    }
    *ptext=word;
    return Py_BuildValue("i",0);
}
static PyObject* writedword(PyObject* self, PyObject* args)
{
    int shmid;
    unsigned int dword;
    if (!PyArg_ParseTuple(args,"iI",&shmid,&dword))
        return Py_BuildValue("i",-1);
    unsigned int* ptext = (unsigned int *)shmat(shmid, 0, 0);
    if (ptext==NULL)
    {
        return Py_BuildValue("i",-1);
    }
    *ptext=dword;
    return Py_BuildValue("i",0);
}
static PyObject* writebytes(PyObject* self, PyObject* args)
{
    int shmid;
    char *byte;
    if (!PyArg_ParseTuple(args,"is",&shmid,&byte))
        return Py_BuildValue("i",-1);
    // printf("input:%s\n",byte);
    char* ptext = (char *)shmat(shmid, 0, 0);
    memcpy(ptext,byte,strlen(byte)+1);
    return Py_BuildValue("i",strlen(byte));
}
static PyObject* readbyte(PyObject* self, PyObject* args)
{
    int shmid;
    if (!PyArg_ParseTuple(args,"i",&shmid))
        return Py_BuildValue("i",-1);
    unsigned char* ptext = (unsigned char *)shmat(shmid, 0, 0);
    return Py_BuildValue("i",*ptext);
}
static PyObject* readword(PyObject* self, PyObject* args)
{
    int shmid;
    if (!PyArg_ParseTuple(args,"i",&shmid))
        return Py_BuildValue("i",-1);
    unsigned short* ptext = (unsigned short *)shmat(shmid, 0, 0);
    return Py_BuildValue("i",*ptext);
}
static PyObject* readdword(PyObject* self, PyObject* args)
{
    int shmid;
    if (!PyArg_ParseTuple(args,"i",&shmid))
        return Py_BuildValue("i",-1);
    unsigned int * ptext = (unsigned int *)shmat(shmid, 0, 0);
    return Py_BuildValue("i",*ptext);
}
static PyObject* readbytes(PyObject* self, PyObject* args)
{
    int shmid;
    if (!PyArg_ParseTuple(args,"i",&shmid))
        return Py_BuildValue("i",-1);
    char * ptext = (char *)shmat(shmid, 0, 0);
    return Py_BuildValue("s",ptext);
}
static PyObject* delete(PyObject* self, PyObject* args)
{
    int shmid;
    if (!PyArg_ParseTuple(args,"i",&shmid))
        return Py_BuildValue("i",-1);
    
    return Py_BuildValue("i",shmctl(shmid,IPC_RMID,0));
}
static PyMethodDef mainMethods[] = {
    {"create",create,METH_VARARGS,"create the shared memory"},
    {"readbyte",readbyte,METH_VARARGS,"read byte data from sheared memory"},
    {"readword",readword,METH_VARARGS,"read word data from sheared memory"},
    {"readdword",readdword,METH_VARARGS,"read dword data from sheared memory"},
    {"readbytes",readbytes,METH_VARARGS,"read bytes data from sheared memory"},
    {"writebyte",writebyte,METH_VARARGS,"write byte data from sheared memory"},
    {"writeword",writeword,METH_VARARGS,"write word data from sheared memory"},
    {"writedword",writedword,METH_VARARGS,"write dword data from sheared memory"},
    {"writebytes",writebytes,METH_VARARGS,"write bytes data from sheared memory"},
    {"delete",delete,METH_VARARGS,"delete the shared memory"},
    {NULL,NULL,0,NULL}
};
static PyModuleDef sharedmem = {
 PyModuleDef_HEAD_INIT,
 "sharedmem","use shared memory by C",
 -1,
 mainMethods
};

PyMODINIT_FUNC 
PyInit_sharedmem(void) {
    return PyModule_Create(&sharedmem);
};