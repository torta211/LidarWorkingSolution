#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>
#include <cmath>
#include "DataHandler.h"
#include <iostream>
#include <Windows.h>

pybind11::handle get_a_frame()
{
	VelodyneVLP16PCAP capture;
	capture.open_live(2);
	capture.retrieve();

	return capture.retrieve();
}

namespace py = pybind11;

PYBIND11_MODULE(Ccode, m) {

	py::class_<VelodyneVLP16PCAP>(m, "VeloParser")
		.def(py::init<>())
		.def("open_live", &VelodyneVLP16PCAP::open_live)
		.def("retrieve", &VelodyneVLP16PCAP::retrieve);

	m.def("get_frame", &get_a_frame, R"pbdoc(
        Compute a hyperbolic tangent of a single argument expressed in radians.
    )pbdoc");

#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif
}