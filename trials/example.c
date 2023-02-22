#include <Python.h>

static PyObject *helloworld(PyObject *self, PyObject *args) {
    return Py_BuildValue("s", "Hello, world!");
}

static PyMethodDef methods[] = {
    {"helloworld", helloworld, METH_NOARGS, "Return the string 'Hello, world!'"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "example",
    "An example module",
    -1,
    methods
};

PyMODINIT_FUNC PyInit_example(void) {
    return PyModule_Create(&module);
}
