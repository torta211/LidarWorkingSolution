#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>
#include <cmath>
#include "DataHandler.h"
#include <iostream>
#include <Windows.h>

std::vector<std::vector<float>> get_a_frame()
{
	VelodyneVLP16PCAP capture;
	capture.open_live(2);

	std::vector<DataPoint> dataPoints;
	int frame_counter = 0;

	while (capture.isRun())
	{
		capture.retrieve(dataPoints);

		if (dataPoints.empty() || dataPoints.size() == 0)
		{
			continue;
		}
		else
		{
			if (frame_counter == 0)
			{
				frame_counter += 1;
				dataPoints.clear();
				std::cout << "first frame thrown away" << std::endl;
				continue;
			}
			else
			{
				break;
			}
		}
	}
	std::cout << "(C++) number of points in this frame: " << dataPoints.size() << std::endl;

	std::vector<std::vector<float>> frame_as_2D_vector = std::vector<std::vector<float>>();

	for (const DataPoint& laser : dataPoints)
	{
		std::vector<float> xyz = std::vector<float>();
		xyz.push_back(laser.coordinates.x);
		xyz.push_back(laser.coordinates.y);
		xyz.push_back(laser.coordinates.z);

		frame_as_2D_vector.push_back(xyz);
	}

	return frame_as_2D_vector;
}

namespace py = pybind11;

PYBIND11_MODULE(Ccode, m) {

	py::class_<VelodyneVLP16PCAP>(m, "VeloParser")
		.def(py::init<>())
		.def("open_live", &VelodyneVLP16PCAP::open_live)
		.def("read_frame", &VelodyneVLP16PCAP::read_frame);

	m.def("get_frame", &get_a_frame, R"pbdoc(
        Compute a hyperbolic tangent of a single argument expressed in radians.
    )pbdoc");

#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif
}