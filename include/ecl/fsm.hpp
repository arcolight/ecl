#ifndef ECL_FSM_HPP
#define ECL_FSM_HPP

#include <cstddef>
#include <type_traits>

#include "singleton.hpp"

namespace ecl
{

template<typename derived, typename state_t, state_t init>
class state_machine
{
protected:
    constexpr state_machine() : m_state(init),
                                m_fsm_ptr(static_cast<derived*>(this)) {}

    template<state_t start, typename event_t, state_t next,
             void (derived::*action)(const event_t&) = nullptr,
             bool (derived::*guard)(const event_t&) = nullptr>
    class row
    {
    protected:
        state_t call(derived& fsm, const event_t& e)                       const
        {
            state_t result = fsm.state();

            bool transition = (nullptr == guard) ? true : (fsm.*(guard))(e);

            if(transition)
            {
                if(nullptr != action) 
                {
                    (fsm.*(action))(e);
                }
                result = next;
            }

            return result;
        }

        template<typename other_event_t>
        state_t call(derived& fsm, const other_event_t& e)                 const
        {
            (void)(e);
            return fsm.state();
        }

        typedef event_t row_event_t;
        constexpr static state_t row_start_state() { return start; }
    };

    template<typename... rows>
    class transition_table : public rows... 
    {
    public:
        template<typename event_t>
        state_t transition(derived& fsm, const event_t& e)                 const
        {
            return transition_internal<event_t, rows...>(fsm, e);
        }

    private:
        template<typename event_t, typename r, typename... tail>
        state_t transition_internal(derived& fsm, const event_t& e)        const
        {
            if(std::is_same<event_t, typename r::row_event_t>::value &&
               (fsm.state() == r::row_start_state()))
            {
                return this->r::call(fsm, e);
            }

            return transition_internal<event_t, tail...>(fsm, e);
        }

        template<typename event_t>
        state_t transition_internal(derived& fsm, const event_t& e)        const
        {
            (void)(e);
            return fsm.state();
        }
    };

    typedef void (derived::*on_enter_t)(void);
    typedef void (derived::*on_exit_t)(void);

    template<state_t state,
             on_enter_t on_enter = nullptr,
             on_exit_t on_exit = nullptr>
    class scb 
    {
    protected:
        constexpr static state_t    cb_state()    { return state;    }
        constexpr static on_enter_t cb_on_enter() { return on_enter; }
        constexpr static on_exit_t  cb_on_exit()  { return on_exit;  }
    };

    template<typename... callbacks>
    class callback_table : public callbacks...
    {
    public:
        static void call(derived& fsm, const state_t prev, const state_t next)
        {
            call_chain<0, callbacks...>(fsm, prev, next);
        }

    private:
        template<size_t CNT, typename callback, typename... tail>
        static void call_chain(derived& fsm, const state_t p, const state_t n)
        {
            if(callback::cb_state() == p)
            {
                if(nullptr != callback::cb_on_exit()) 
                {
                    (fsm.*(callback::cb_on_exit()))();
                }
            }
            if(callback::cb_state() == n)
            {
                if(nullptr != callback::cb_on_enter()) 
                {
                    (fsm.*(callback::cb_on_enter()))();
                }
            }

            call_chain<CNT + 1, tail...>(fsm, p, n);
        }

        template<size_t CNT>
        static void call_chain(derived& fsm, const state_t p, const state_t n)
        {
            (void)(fsm);
            (void)(p);
            (void)(n);
        }
    };

public:
    template<typename event_t,
             typename transition_table_t,
             typename callback_table_t>
    state_t transition(const event_t& e)
    {
        transition_table_t& tt = singleton<transition_table_t>::instance();

        const state_t current = m_state;
        const state_t next = tt.template transition<event_t>(*m_fsm_ptr, e);
        m_state = next;

        if(next != current) 
        {
            callback_table_t::call(*m_fsm_ptr, current, next);
        }

        return m_state;
    }

    state_t state()                                                        const 
    {
        return m_state; 
    }

    void reset()
    {
        m_state = m_s_init_state;
    }

private:
    state_machine(const state_machine& other)                          = delete;
    state_machine& operator= (const state_machine& other)              = delete;
    state_machine(const state_machine&& other)                         = delete;
    state_machine& operator= (const state_machine&& other)             = delete;    

    state_t m_state;
    derived* const m_fsm_ptr;
    constexpr static state_t m_s_init_state { init };
};

} // namespace ecl

#endif // ECL_FSM_HPP
