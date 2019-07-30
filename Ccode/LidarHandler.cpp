#include <Python.h>
#include <cmath>
#include "DataHandler.h"
#include <iostream>
#include <Windows.h>

PyObject* get_a_frame()
{
	VelodyneVLP16PCAP capture;
	capture.open_live();

	std::vector<DataPoint> dataPoints;
	while (capture.isRun())
	{
		capture.retrieve(dataPoints);
		if (dataPoints.empty())
		{
			continue;
		}
		else
		{
			break;
		}

	}

	std::cout << "frame read in c++ vector form" << std::endl;
	std::cout << "number of points in this frame: " << dataPoints.size() << std::endl;

	PyObject* frame_as_list = PyList_New(0);

	for (const DataPoint& laser : dataPoints)
	{
		PyObject* one_point = PyList_New(0);
		PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.x));
		PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.y));
		PyList_Append(one_point, PyFloat_FromDouble(laser.coordinates.z));

		PyList_Append(frame_as_list, one_point);
	}

	std::cout << "passing frame to python code" << std::endl;
	return frame_as_list;
}

static PyMethodDef lidarhandler_methods[] = {
	// The first property is the name exposed to Python, fast_tanh, the second is the C++
	// function name that contains the implementation.
    { "get_frame", (PyCFunction)get_a_frame, METH_O, nullptr },

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