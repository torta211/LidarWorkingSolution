#include <Python.h>
#include <cmath>
#include "DataHandler.h"
#include <iostream>
#include <Windows.h>


PyObject* get_a_test_list() {
	std::cout << "Converting an std::vector to Python vector" << std::endl;

	PyObject* result_list = PyList_New(0);

	for (int i = 0; i < 100; i++)
	{
		PyObject* sub_list = PyList_New(0);
		PyList_Append(sub_list, PyFloat_FromDouble(i));
		PyList_Append(sub_list, PyFloat_FromDouble(i * 2.0 + i));
		PyList_Append(sub_list, PyFloat_FromDouble(i * 0.5 - i));

		PyList_Append(result_list, sub_list);
	}
	return result_list;
}

const double e = 2.7182818284590452353602874713527;

double sinh_impl(double x) {
	return (1 - pow(e, (-2 * x))) / (2 * pow(e, -x));
}

double cosh_impl(double x) {
	return (1 + pow(e, (-2 * x))) / (2 * pow(e, -x));
}

PyObject* tanh_impl(PyObject*, PyObject* o) {
	double x = PyFloat_AsDouble(o);
	double tanh_x = sinh_impl(x) / cosh_impl(x);
	return PyFloat_FromDouble(tanh_x);
}

static PyMethodDef lidarhandler_methods[] = {
	// The first property is the name exposed to Python, fast_tanh, the second is the C++
	// function name that contains the implementation.
	{ "fast_tanh", (PyCFunction)tanh_impl, METH_O, nullptr },
    { "get_test_list", (PyCFunction)get_a_test_list, METH_O, nullptr },

	// Terminate the array with an object containing nulls.
	{ nullptr, nullptr, 0, nullptr }
};

static PyModuleDef lidarhandler_module = {
	PyModuleDef_HEAD_INIT,
	"C_mod",                        // Module name to use with Python import statements
	"Provides some functions, but faster",  // Module description
	0,
	lidarhandler_methods                  // Structure that defines the methods of the module
};

PyMODINIT_FUNC PyInit_Ccode() {
	return PyModule_Create(&lidarhandler_module);
}