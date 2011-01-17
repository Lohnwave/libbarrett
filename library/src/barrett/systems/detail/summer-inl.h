/*
 * summer.tcc
 *
 *  Created on: Sep 11, 2009
 *      Author: dc
 */

#include <boost/array.hpp>
#include <string>
#include <bitset>

#include "../../detail/stl_utils.h"
#include "../../math/traits.h"
#include "../abstract/system.h"


namespace barrett {
namespace systems {


template<typename T, size_t numInputs>
Summer<T, numInputs>::Summer(const Polarity& inputPolarity, bool undefinedIsZero) :
	SingleOutput<T>(this), polarity(inputPolarity), strict(!undefinedIsZero)
{
	initInputs();
}

template<typename T, size_t numInputs>
Summer<T, numInputs>::Summer(const std::string& inputPolarity, bool undefinedIsZero) :
	SingleOutput<T>(this), polarity(inputPolarity), strict(!undefinedIsZero)
{
	initInputs();
}

template<typename T, size_t numInputs>
Summer<T, numInputs>::Summer(const char* inputPolarity, bool undefinedIsZero) :
	SingleOutput<T>(this), polarity(inputPolarity), strict(!undefinedIsZero)
{
	initInputs();
}

template<typename T, size_t numInputs>
Summer<T, numInputs>::Summer(const std::bitset<numInputs>& inputPolarity, bool undefinedIsZero) :
	SingleOutput<T>(this), polarity(inputPolarity), strict(!undefinedIsZero)
{
	initInputs();
}

template<typename T, size_t numInputs>
Summer<T, numInputs>::Summer(bool undefinedIsZero) :
	SingleOutput<T>(this), polarity(), strict(!undefinedIsZero)
{
	initInputs();
}

template<typename T, size_t numInputs>
inline Summer<T, numInputs>::~Summer()
{
	purge(inputs);
}

template<typename T, size_t numInputs>
inline System::Input<T>& Summer<T, numInputs>::getInput(const size_t i) {
	return *( inputs.at(i) );
}

template<typename T, size_t numInputs>
void Summer<T, numInputs>::operate()
{
	typedef math::Traits<T> Traits;

	T sum = Traits::zero();
	for (size_t i = 0; i < numInputs; ++i) {
		if (inputs[i]->valueDefined()) {
			sum = Traits::add(sum, Traits::mult(polarity[i], inputs[i]->getValue()));
		} else if (strict) {
			this->outputValue->setValueUndefined();
			return;
		}
	}

	this->outputValue->setValue(sum);
}

template<typename T, size_t numInputs>
void Summer<T, numInputs>::initInputs()
{
	for (size_t i = 0; i < numInputs; ++i) {
		inputs[i] = new Input<T>(this);
	}
}


}
}