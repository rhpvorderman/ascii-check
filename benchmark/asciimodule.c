#define PY_SSIZE_T_CLEAN
#include <Python.h>
#if ASCII_CHECK_SSE2
#include "ascii_check_sse2.h"
#elif ASCII_CHECK_AVX2
#include "ascii_check_avx2.h"
#else
#include "ascii_check.h"
#endif
PyDoc_STRVAR(ascii_check_doc,
"ascii_check($module, data, /)\n"
"--\n"
"\n"
"Check whether the given data is ASCII-encoded by checking for any values\n"
"\ngreater than 128."
"\n"
"  data\n"
"   bytes object\n"
);
static PyObject * 
ascii_check(PyObject *module, PyObject * obj) {
    return PyBool_FromLong(
        string_is_ascii(
            PyBytes_AS_STRING(obj),
            PyBytes_GET_SIZE(obj)
        )
    );
}

PyDoc_STRVAR(ascii_search_doc,
"search_non_ascii($module, data, /)\n"
"--\n"
"\n"
"Return the position of the first non-ASCII character. -1 if not found."
"\n"
"  data\n"
"   bytes object\n"
);
static PyObject *
ascii_search_non_ascii(PyObject *module, PyObject * obj) {
    char * string = PyBytes_AS_STRING(obj);
    Py_ssize_t size = PyBytes_GET_SIZE(obj);
    const char * non_ascii = search_non_ascii(string, size);
    if (non_ascii == NULL) {
        return PyLong_FromLong(-1);
    }
    return PyLong_FromSize_t(non_ascii - string);
}

static PyMethodDef ascii_methods[] = {
    {"ascii_check", (PyCFunction)(void(*)(void))ascii_check, METH_O, 
     ascii_check_doc},
    {"search_non_ascii", (PyCFunction)(void(*)(void))ascii_search_non_ascii,
     METH_O, ascii_search_doc},
    {NULL}
};

static struct PyModuleDef ascii_module = {
    PyModuleDef_HEAD_INIT,
    "ascii",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,
    ascii_methods  /* module methods */
};

PyMODINIT_FUNC
PyInit_ascii(void)
{
    PyObject *m;

    m = PyModule_Create(&ascii_module);
    if (m == NULL)
        return NULL;

    return m;
}
