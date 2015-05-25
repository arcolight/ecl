#ifndef ECL_FSM_HPP
#define ECL_FSM_HPP

#include <cstddef>

#include "singleton.hpp"

namespace ecl
{

template<typename derived, typename state_t, state_t init>
class state_machine
{
protected:
    constexpr state_machine() : m_state(init),
                                m_fsm_ptr(static_cast<derived*>(this)) {}

    template<typename event_t>
    class transition_layer
    {
        typedef void (derived::*action_t)(const event_t&);
        typedef bool (derived::*guard_t)(const event_t&);

    protected:
        typedef struct transition_info
        {
            action_t action;
            guard_t  guard;
            state_t  start;
            state_t  next;

            struct transition_info* link;
        } transition_info_t;

    private:
        transition_layer(const transition_layer& other)                = delete;
        transition_layer& operator= (const transition_layer& other)    = delete;
        transition_layer(const transition_layer&& other)               = delete;
        transition_layer& operator= (const transition_layer&& other)   = delete;

        transition_info_t* m_list;

    protected:
        transition_layer() : m_list(nullptr) {}

        void add(transition_info_t* const ti)
        {
            if(nullptr == m_list) 
            {
                m_list = ti;
                return;
            }

            transition_info_t* entry = m_list;

            while(nullptr != entry->link)
            {
                entry = entry->link;
            }

            entry->link = ti;
        }

        state_t call(derived& fsm, const event_t& e)                       const
        {
            state_t st = fsm.state();

            const transition_info_t* entry = m_list;

            while(nullptr != entry)
            {
                if(st == entry->start)
                {
                    bool transition = true;

                    if(nullptr != entry->guard) 
                    {
                        transition = (fsm.*(entry->guard))(e);
                    }

                    if(transition) 
                    {
                        if(nullptr != entry->action) 
                        {
                            (fsm.*(entry->action))(e);
                        }
                        return entry->next;
                    }
                }

                entry = entry->link;
            }

            return st;
        }
    };

    template<state_t start, typename event_t, state_t next,
             void (derived::*action)(const event_t&) = nullptr,
             bool (derived::*guard)(const event_t&) = nullptr>
    class row : public virtual transition_layer<event_t>
    {
    protected:
        row() : ti( {action, guard, start, next, nullptr} )
        {
            this->transition_layer<event_t>::add(&ti);
        }

    private:
        row(const row& other)                                          = delete;
        row& operator= (const row& other)                              = delete;
        row(const row&& other)                                         = delete;
        row& operator= (const row&& other)                             = delete;

        typename transition_layer<event_t>::transition_info_t ti;
    };

    template<typename... rows>
    class transition_table : public rows...
    {
    public:
    	transition_table();

        template<typename event_t>
        state_t transition(derived& fsm, const event_t& e)                 const
        {
            return this->transition_layer<event_t>::call(fsm, e);
        }

    private:
        transition_table(const transition_table& other)                = delete;
        transition_table& operator= (const transition_table& other)    = delete;
        transition_table(const transition_table&& other)               = delete;
        transition_table& operator= (const transition_table&& other)   = delete;
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

    private:
        scb(const scb& other)                                          = delete;
        scb& operator= (const scb& other)                              = delete;
        scb(const scb&& other)                                         = delete;
        scb& operator= (const scb&& other)                             = delete;
    };

    template<typename... callbacks>
    class callback_table : public callbacks...
    {
    public:
        static void call(derived& fsm, const state_t prev, const state_t next)
        {
            on_exit_chain<0, callbacks...>(fsm, prev);
            on_enter_chain<0, callbacks...>(fsm, next);
        }

    private:
        callback_table(const callback_table& other)                    = delete;
        callback_table& operator= (const callback_table& other)        = delete;
        callback_table(const callback_table&& other)                   = delete;
        callback_table& operator= (const callback_table&& other)       = delete;

        template<size_t CNT, typename callback, typename... tail>
        static void on_exit_chain(derived& fsm, const state_t s)
        {
            if(callback::cb_state() == s) 
            {
                if(nullptr != callback::cb_on_exit()) 
                {
                    (fsm.*(callback::cb_on_exit()))();
                }
                return;
            }

            on_exit_chain<CNT + 1, tail...>(fsm, s);
        }

        template<size_t CNT>
        static void on_exit_chain(derived& fsm, const state_t s)
        {
            (void)(fsm);
            (void)(s);
        }

        template<size_t CNT, typename callback, typename... tail>
        static void on_enter_chain(derived& fsm, const state_t s)
        {
            if(callback::cb_state() == s)
            {
                if(nullptr != callback::cb_on_enter()) 
                {
                    (fsm.*(callback::cb_on_enter()))();
                }
                return;
            }

            on_enter_chain<CNT + 1, tail...>(fsm, s);
        }

        template<size_t CNT>
        static void on_enter_chain(derived& fsm, const state_t s)
        {
            (void)(fsm);
            (void)(s);
        }
    };

public:
    template<typename event_t,
             typename transition_table_t,
             typename callback_table_t>
    state_t transition(const event_t& e);

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

template<typename derived, typename state_t, state_t init>
template<typename event_t,
         typename transition_table_t,
         typename callback_table_t>
state_t state_machine<derived, state_t, init>::transition(const event_t& e)
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

template<typename derived, typename state_t, state_t init>
template<typename... rows>
state_machine<derived, state_t, init>::transition_table<rows...>::transition_table()
{
}

} // namespace ecl

#endif // ECL_FSM_HPP
