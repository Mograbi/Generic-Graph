/*
 * exceptions.h
 *
 *  Created on: Dec 20, 2015
 *      Author: Moawiya Mograbi
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>

namespace mgg {

class VertexDoesNotExist: public exception {
};

class EdgeDoesNotContainVertex: public exception {
};

class VertexAlreadyExist: public exception {
};

}

#endif /* EXCEPTIONS_H_ */
