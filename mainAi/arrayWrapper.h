#ifndef ARRAYWRAPPER_H
#define ARRAYWRAPPER_H

#include "array.h"

namespace NeurelNetwork
{
	namespace ArrayUtils
{
		/*
		PLEASE REMEMBER, ARRAYWRAPPER REQUIRES ITS DESTRUCTOR TO HAPPEN BEFORE DESTRUCTOR OF ARRAY THIS MEANS IT MUST
		BE INSTANTIATIED AFTER THE ARRAY AS DESTRUCTORS HAPPEN IN REVERSE ORDER OF INSTANTIATION
		*/

		template <typename Type>
		class ArrayWrapper
		{
		public:
			ArrayWrapper(Array<Type>* wrappedArr);
			~ArrayWrapper();
		private:
			Array<Type>* wrappedArr;
		};

		template <typename Type>
		ArrayWrapper<Type>::ArrayWrapper(Array<Type>* wrappedArr):
			wrappedArr(wrappedArr)
		{
			wrappedArr->autoDeleteMem = false; //you could do this->wrappedArr->autoDeleteMem but you dont need to
		}

		template <typename Type>
		ArrayWrapper<Type>::~ArrayWrapper()
		{
			wrappedArr->autoDeleteMem = true;
			//wrapped arr should get its destructor called the line after this
		}
}
}



#endif //ARRAYWRAPPER_H
