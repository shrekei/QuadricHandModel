#pragma once
#include "Vector3.h"
#include "Matrix33.h"

bmu::Vector3 CalcEulerAnglesXYZ(bmu::Matrix33 m);
bmu::Vector3 CalcEulerAnglesXYZWithRef(bmu::Matrix33 m, bmu::Vector3 vRef);
bmu::Matrix33 Euler2MatrixXYZ(bmu::Vector3);