/**
 * @file
 *
 * @brief Scope guard class. RAII idiom.
 *
 * @ingroup ecl
 */
#ifndef ECL_SCOPE_GUARD_HPP
#define ECL_SCOPE_GUARD_HPP

#include <functional>
#include <utility>

namespace ecl
{

/**
 * @brief Scope guard class.
 * @details Allows to execute something at exit of the scope.
 */
class scope_guard
{
    scope_guard()                                                      = delete;
    scope_guard(const scope_guard&)                                    = delete;
    scope_guard(const scope_guard&&)                                   = delete;
    scope_guard& operator=(const scope_guard&)                         = delete;
    scope_guard& operator=(const scope_guard&&)                        = delete;

public:
    /**
     * @brief Constructor
     *
     * @param action Object, that would be called on scope_guard destruction.
     */
    explicit scope_guard(std::function<void()> &&action) :
        m_action(std::forward<decltype(m_action)>(action))
    {

    }

    ~scope_guard()
    {
        m_action();
    }

private:
    std::function<void()> m_action;
};

} // namespace ecl

#endif // ECL_SCOPE_GUARD_HPP */
