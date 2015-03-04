#ifndef SCOPE_GUARD_HPP_
#define SCOPE_GUARD_HPP_

#include <functional>
#include <utility>

namespace ecl
{
class scope_guard
{
    scope_guard() = delete;
    scope_guard(const scope_guard&) = delete;
    scope_guard(const scope_guard&&) = delete;
    scope_guard& operator=(const scope_guard&) = delete;
    scope_guard& operator=(const scope_guard&&) = delete;
public:
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
}

#endif /* SCOPE_GUARD_HPP_ */
