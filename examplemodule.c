#include <Python.h>

static PyObject* multiply(PyObject* self, PyObject* args)
{
    double x,y;
    PyArg_ParseTuple(args, "dd", &x, &y);
    return Py_BuildValue("d", x*y);
}

static PyObject* sum(PyObject* self, PyObject* args)
{
    double x,y;
    PyArg_ParseTuple(args, "dd", &x, &y);
    return Py_BuildValue("d", (x+y)/2);
}

static PyMethodDef examplemodule_methods[] = {
    {"multiply", multiply, METH_VARARGS},
    {"sum", sum, METH_VARARGS},
    {NULL, NULL}
};

void initexamplemodule()
{
    (void) Py_InitModule("examplemodule", examplemodule_methods);
}
