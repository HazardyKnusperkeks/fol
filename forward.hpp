/**
 * @file
 * @brief Contains forward declarations for all classes.
 */

#ifndef FOL_FORWARD_HPP
#define FOL_FORWARD_HPP

namespace fol {

template<char... String>
class Name;

class RtName;

template<char c, char... Name>
struct Variable;

struct RtVariable;

template<typename Name, typename... Args>
struct Function;

template<typename Name, typename... Args>
struct Predicate;

template<typename T1, typename T2>
struct Equality;

template<typename T>
struct Not;

} //namespace fol

#endif
