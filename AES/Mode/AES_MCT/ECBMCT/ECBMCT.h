#ifndef _ECBMCT_
#pragma once

#include <iostream>
#include "AES32.h"
#include "HexByte.h"
#include "../../../Math/GF_Matrix_Math.h"
#include "../../../Math/GF_Math.h"

using namespace std;
typedef unsigned char byte;
typedef unsigned char u8;
typedef unsigned int u32;

// ECB MCT Test
void ECBMCT();

#endif