#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>

inline void CHECK_DX(HRESULT hr) {
	if (FAILED(hr)) {
		std::cout << "Error detected: " << hr << std::endl;
		throw std::exception();
	}
}