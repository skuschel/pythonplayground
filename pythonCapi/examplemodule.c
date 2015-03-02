
#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION

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

    n = PyArray_SIZE(array);
    carray = PyArray_DATA(array);

    ret=0.0;
    for (i=0; i<n; i++) {
        ret += carray[i];
        //printf("%.4f\n", ret);
    }
    return Py_BuildValue("d", ret);
}


static PyObject* hist1d(PyObject* self, PyObject* args)
{
    /*
    function numpyarray hist1d (numpyarray array, double min, double max, int bins, numpyarray weights)
    Simple Histogram of array with n bins
    weights is optional
    */
    PyArrayObject *pyarray, *pyret, *pyweights;
    double min, max, x, *array, *ret, tmp, *weights;
    int i, n, bins, size;
    int outdims[2];

    // default for pyweights
    pyweights = NULL;

    // Parse Input
    if (!PyArg_ParseTuple(args, "O!ddi|O!",
        &PyArray_Type, &pyarray, &min, &max, &bins, &PyArray_Type, &pyweights))  return NULL;
    if (NULL == pyarray)  return NULL;
    outdims[0] = bins;
    //printf("%.3f\n", min);
    //printf("%.3f\n", max);
    array = PyArray_DATA(pyarray);
    size = PyArray_SIZE(pyarray);

    // initialize return values
    pyret = (PyArrayObject *) PyArray_FromDims(1, outdims, NPY_DOUBLE);
    ret = PyArray_DATA(pyret);

    // do work
    tmp = 1.0 / (max - min) * bins;
    //printf("%.3f\n", tmp);
    if (NULL == pyweights) {  //weights not given
        for (n=0; n < size; n++) {
            x = (array[n] - min) * tmp;
            //printf("%.3f\n", x);
            if (x >= 0.0 & x < bins) {
                ret[(int)x] += 1.0;
            }
        }
    } else {  //weights is given
        weights = PyArray_DATA(pyweights);
        for (n=0; n < size; n++) {
            x = (array[n] - min) * tmp;
            //printf("%.3f\n", weights[n]);
            if (x >= 0.0 & x < bins) {
                ret[(int)x] += weights[n];
            }
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
    PyArrayObject *pyarray, *pyret, *pyweights;
    double min, max, x, *array, *ret, tmp, *weights;
    int i, n, bins, xr, size;
    int outdims[2];

    // default if optional arguemnt not supplied
    pyweights = NULL;

    // Parse Input
    if (!PyArg_ParseTuple(args, "O!ddi|O!",
        &PyArray_Type, &pyarray, &min, &max, &bins, &PyArray_Type, &pyweights))  return NULL;
    if (NULL == pyarray)  return NULL;
    outdims[0] = bins;
    //printf("%.3f\n", min);
    //printf("%.3f\n", max);
    array = PyArray_DATA(pyarray);
    size = PyArray_SIZE(pyarray);

    // initialize return values
    pyret = (PyArrayObject *) PyArray_FromDims(1, outdims, NPY_DOUBLE);
    ret = PyArray_DATA(pyret);

    // do work
    tmp = 1.0 / (max - min) * bins;
    //printf("%.3f\n", tmp);
    if (NULL == pyweights) {  //weights not given
        for (n=0; n < size; n++) {
            x = (array[n] - min) * tmp;
            xr = floor(x + 0.5);
            if (xr >= 0.0 & xr < bins) {
                ret[(int)xr] += 0.5 + x - xr;
                if (xr > 1.0){
                    ret[(int)(xr) - 1] += 0.5 - x + xr;
                }
            }
        }
    } else {  //weights is given
        //printf("weights will be used");
        weights = PyArray_DATA(pyweights);
        for (n=0; n < size; n++) {
            x = (array[n] - min) * tmp;
            xr = floor(x + 0.5);
            if (xr >= 0.0 & xr < bins) {
                ret[(int)xr] += (0.5 + x - xr) * weights[n];
                if (xr > 1.0){
                    ret[(int)(xr) - 1] += (0.5 - x + xr) * weights[n];
                }
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
