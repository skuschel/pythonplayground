
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
    PyArrayObject *pyarray, *array;
    double ret, *carray;
    int i, n;

    if (!PyArg_ParseTuple(args, "O!",
        &PyArray_Type, &pyarray))  return NULL;
    if (NULL == pyarray)  return NULL;
    //if (not_doublevector(array)) return NULL;

    array = (PyArrayObject *) PyArray_GETCONTIGUOUS(pyarray);
    n = PyArray_SIZE(array);
    carray = PyArray_DATA(array);

    ret=0.0;
    for (i=0; i<n; i++) {
        ret += carray[i];
        //printf("%.4f\n", ret);
    }
    return Py_BuildValue("d", ret);
}

static PyObject* sumarrayiterator(PyObject* self, PyObject* args)
{
    PyArrayObject *pyarray;
    double ret, **dataptr;
    NpyIter *iter;
    NpyIter_IterNextFunc *iternext;

    if (!PyArg_ParseTuple(args, "O!",
        &PyArray_Type, &pyarray))  return NULL;
    if (NULL == pyarray)  return NULL;
    //if (not_doublevector(array)) return NULL;

    iter = NpyIter_New(pyarray,
            NPY_ITER_READONLY,
            NPY_KEEPORDER, NPY_NO_CASTING, PyArray_DescrFromType(NPY_DOUBLE));
    if (iter==NULL) {
        return NULL;
    }
    iternext = NpyIter_GetIterNext(iter, NULL);
    dataptr = (double **) NpyIter_GetDataPtrArray(iter);

    ret=0.0;
    do {
        //printf("%.3f\n", **dataptr);
        ret += **dataptr;
        //printf("%.4f\n", ret);
    } while (iternext(iter));

    Py_DECREF(iter);
    return Py_BuildValue("d", ret);
}

static PyObject* sumarrayitextloop(PyObject* self, PyObject* args)
{
    PyArrayObject *pyarray;
    double ret;
    char **dataptr;
    NpyIter *iter;
    NpyIter_IterNextFunc *iternext;
    npy_intp *strideptr, *innersizeptr;
    npy_intp size;

    if (!PyArg_ParseTuple(args, "O!",
        &PyArray_Type, &pyarray))  return NULL;
    if (NULL == pyarray)  return NULL;
    //if (not_doublevector(array)) return NULL;

    iter = NpyIter_New(pyarray,
            NPY_ITER_READONLY | NPY_ITER_EXTERNAL_LOOP | NPY_ITER_REFS_OK,
            NPY_KEEPORDER, NPY_NO_CASTING, PyArray_DescrFromType(NPY_DOUBLE));
    if (iter==NULL) {
        return NULL;
    }

    iternext = NpyIter_GetIterNext(iter, NULL);
    dataptr = (char **) NpyIter_GetDataPtrArray(iter);
    strideptr = NpyIter_GetInnerStrideArray(iter);
    innersizeptr = NpyIter_GetInnerLoopSizePtr(iter);
    npy_intp iop, nop = NpyIter_GetNOp(iter);

    ret=0.0;
    do {  //external loop
        //double *data = *dataptr;
        //npy_intp stride = *strideptr;
        size = *innersizeptr;

        //printf("size: %.3d\n", size);
        //printf("nop:  %.3d\n", nop);
        //printf("strideptr:  %.3d\n", *strideptr);

        while (size--) {  //internal loop
            double *ddata = (double*) dataptr[0];
            ret += *ddata;
            for (iop = 0; iop < nop; ++iop) {
                //printf("%.3f\n", *dataptr[iop]);
                dataptr[iop] += strideptr[iop];
            }
            //ret += *data;
            //data += stride;
        }

    } while (iternext(iter));

    NpyIter_Deallocate(iter);
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
    double min, max, x, *ret;
    int bins;

    // default for pyweights
    pyweights = NULL;

    // Parse Input
    if (!PyArg_ParseTuple(args, "O!ddi|O!",
        &PyArray_Type, &pyarray, &min, &max, &bins, &PyArray_Type, &pyweights))  return NULL;
    if (NULL == pyarray)  return NULL;

    NpyIter *iter;
    iter = NpyIter_New(pyarray,
            NPY_ITER_READONLY | NPY_ITER_EXTERNAL_LOOP | NPY_ITER_REFS_OK,
            NPY_KEEPORDER, NPY_NO_CASTING, PyArray_DescrFromType(NPY_DOUBLE));
    if (iter==NULL) {
        return NULL;
    }

    NpyIter_IterNextFunc* iternext = NpyIter_GetIterNext(iter, NULL);
    char** dataptr = (char **) NpyIter_GetDataPtrArray(iter);
    npy_intp* strideptr = NpyIter_GetInnerStrideArray(iter);
    npy_intp* innersizeptr = NpyIter_GetInnerLoopSizePtr(iter);
    npy_intp iop, nop = NpyIter_GetNOp(iter);
    if (nop != 1){
        return NULL;
    }

    // initialize return values
    int outdims[2];
    outdims[0] = bins;
    pyret = (PyArrayObject *) PyArray_FromDims(1, outdims, NPY_DOUBLE);
    ret = PyArray_DATA(pyret);
    double tmp = 1.0 / (max - min) * bins;

    if (NULL == pyweights) {  //no weights are given
        do {  //external loop
            npy_intp size = *innersizeptr;
            while (size--) {  //internal loop
                double *ddata = (double*) dataptr[0];
                x = (*ddata - min) * tmp;
                if ((x>0.0) & (x<bins)) {
                    ret[(int)x] += 1.0;
                }
                dataptr[0] += strideptr[0];
            }
        } while (iternext(iter));
    } else {  //weights are given
        NpyIter* witer = NpyIter_New(pyweights,
                NPY_ITER_READONLY | NPY_ITER_EXTERNAL_LOOP | NPY_ITER_REFS_OK,
                NPY_KEEPORDER, NPY_NO_CASTING, PyArray_DescrFromType(NPY_DOUBLE));
        NpyIter_IterNextFunc* witernext = NpyIter_GetIterNext(witer, NULL);
        char** wdataptr = (char **) NpyIter_GetDataPtrArray(witer);
        npy_intp* wstrideptr = NpyIter_GetInnerStrideArray(witer);
        //npy_intp* winnersizeptr = NpyIter_GetInnerLoopSizePtr(witer);
        do {  //external loop
            npy_intp size = *innersizeptr;
            while (size--) {  //internal loop
                double *ddata = (double*) dataptr[0];
                double *wddata = (double*) wdataptr[0];
                x = (*ddata - min) * tmp;
                if ((x>0.0) & (x<bins)) {
                    ret[(int)x] += *wddata;
                }
                dataptr[0] += strideptr[0];
                wdataptr[0] += wstrideptr[0];
            }
        } while (iternext(iter));
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
    int n, bins, xr, size;
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
            if ((xr >= 0.0) & (xr < bins)) {
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
            if ((xr >= 0.0) & (xr < bins)) {
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
    {"sumarrayiterator", sumarrayiterator, METH_VARARGS},
    {"sumarrayiteratorextloop", sumarrayitextloop, METH_VARARGS},
    {"hist1d", hist1d, METH_VARARGS},
    {"hist1dtophat", hist1dtophat, METH_VARARGS},
    {NULL, NULL}
};


#if PY_MAJOR_VERSION < 3
// python2 init
void initexamplemodule()
{
    (void) Py_InitModule("examplemodule", examplemodule_methods);
    import_array();  //necessary for numpy
}
#else
// python3 init
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "Examplemodule",
    "Examplemodule docstring",
    -1,
    examplemodule_methods,
    NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_examplemodule(void)
{
    Py_Initialize();
    import_array();
    return PyModule_Create(&moduledef);
}
#endif

