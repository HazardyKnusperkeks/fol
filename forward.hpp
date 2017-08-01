/**
 * @file
 * @brief Contains forward declarations for all classes.
 */

#ifndef FOL_FORWARD_HPP
#define FOL_FORWARD_HPP

namespace fol {

template<char c, char... Name>
class Variable;

class RtVariable;

template<typename Name, typename... Args>
class Function;

} //namespace fol

#endif
