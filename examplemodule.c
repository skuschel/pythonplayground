#include <Python.h>
#include "numpy/arrayobject.h"
#include <stdio.h>
#include <math.h>

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

static PyObject* sumarray(PyObject* self, PyObject* args)
{
    PyArrayObject *array;
    double ret, *carray;
    int i, n;

    if (!PyArg_ParseTuple(args, "O!",
        &PyArray_Type, &array))  return NULL;
    if (NULL == array)  return NULL;
    //if (not_doublevector(array)) return NULL;

    n = array->dimensions[0];
    carray = (double *) array->data;

    ret=0.0;
    for (i=0; i<n; i++) {
        ret += carray[i];
        //printf("%.4f\n", ret);
    }
    return Py_BuildValue("d", ret);
}

static PyMethodDef examplemodule_methods[] = {
    {"multiply", multiply, METH_VARARGS},
    {"sum", sum, METH_VARARGS},
    {"sumarray", sumarray, METH_VARARGS},
    {NULL, NULL}
};

void initexamplemodule()
{
    (void) Py_InitModule("examplemodule", examplemodule_methods);
    import_array();  //necessary for numpy
}
