#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <ascii_check.h>

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

static PyMethodDef ascii_methods[] = {
    {"ascii_check", (PyCFunction)(void(*)(void))ascii_check,
     METH_O, ascii_check_doc},
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
