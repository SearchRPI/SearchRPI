#pragma once

/**
 * @file  types.h
 * @brief Global typedefs
*/

namespace SearchRPI {

/** A unique identifier for a document.
 *
 *  Docid 0 is invalid, providing an "out of range" value which can be
 *  used to mean "not a valid document".
 */
typedef unsigned int docid;

}
