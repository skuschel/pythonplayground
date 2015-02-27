#include <Python.h>
#include "numpy/arrayobject.h"
#include <stdio.h>
#include <math.h>

// Stephan Kuschel 2015

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

    ret=0.0;
    for (i=0; i<n; i++) {
        ret += array->data[i];
        //printf("%.4f\n", ret);
    }
    return Py_BuildValue("d", ret);
}


static PyObject* hist1d(PyObject* self, PyObject* args)
{
    /*
    function numpyarray hist1d (numpyarray array, double min, double max, int bins)
    Simple Histogram of array with n bins
    */
    PyArrayObject *pyarray, *pyret;
    double min, max, x, *array, *ret, tmp;
    int i, n, bins;
    int outdims[2];

    // Parse Input
    if (!PyArg_ParseTuple(args, "O!ddi",
        &PyArray_Type, &pyarray, &min, &max, &bins))  return NULL;
    if (NULL == pyarray)  return NULL;
    outdims[0] = bins;
    //printf("%.3f\n", min);
    //printf("%.3f\n", max);
    array = (double *) pyarray->data;

    // initialize return values
    pyret = (PyArrayObject *) PyArray_FromDims(1, outdims, NPY_DOUBLE);
    ret = (double *) pyret->data;

    // do work
    tmp = 1.0 / (max - min) * bins;
    //printf("%.3f\n", tmp);
    for (n=0; n < pyarray->dimensions[0]; n++) {
        x = (array[n] - min) * tmp;
        //printf("%.3f\n", x);
        if (x >= 0.0 & x < bins) {
            ret[(int)x] += 1.0;
        }
    }

    return PyArray_Return(pyret);
}

static PyObject* hist1dtophat(PyObject* self, PyObject* args)
{
    /*
    function numpyarray hist1d (numpyarray array, double min, double max, int bins)
    Simple Histogram of array with n bins
    */
    PyArrayObject *pyarray, *pyret;
    double min, max, x, *array, *ret, tmp;
    int i, n, bins, xr;
    int outdims[2];

    // Parse Input
    if (!PyArg_ParseTuple(args, "O!ddi",
        &PyArray_Type, &pyarray, &min, &max, &bins))  return NULL;
    if (NULL == pyarray)  return NULL;
    outdims[0] = bins;
    //printf("%.3f\n", min);
    //printf("%.3f\n", max);
    array = (double *) pyarray->data;

    // initialize return values
    pyret = (PyArrayObject *) PyArray_FromDims(1, outdims, NPY_DOUBLE);
    ret = (double *) pyret->data;

    // do work
    tmp = 1.0 / (max - min) * bins;
    //printf("%.3f\n", tmp);
    for (n=0; n < pyarray->dimensions[0]; n++) {
        x = (array[n] - min) * tmp;
        xr = floor(x + 0.5);
        if (xr >= 0.0 & xr < bins) {
            ret[(int)xr] += 0.5 + x - xr;
            if (xr > 1.0){
                ret[(int)(xr) - 1] += 0.5 - x + xr;
            }
        }
    }

    return PyArray_Return(pyret);
}


static PyMethodDef examplemodule_methods[] = {
    {"multiply", multiply, METH_VARARGS},
    {"sum", sum, METH_VARARGS},
    {"sumarray", sumarray, METH_VARARGS},
    {"hist1d", hist1d, METH_VARARGS},
    {"hist1dtophat", hist1dtophat, METH_VARARGS},
    {NULL, NULL}
};



void initexamplemodule()
{
    (void) Py_InitModule("examplemodule", examplemodule_methods);
    import_array();  //necessary for numpy
}
