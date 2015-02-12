#include <iostream>
#include <vector>

#include <ecl/fsm.hpp>

struct ev_1 {};
struct ev_2 {};
struct ev_3 {};
struct ev_4 {};
struct ev_5 { bool val { false }; };
struct ev_6 {};

enum class st
{
    s_1,
    s_2,
    s_3,
    s_4,
    s_5
};

class test_fsm : public ecl::state_machine<test_fsm, st, st::s_1>
{
    void on_ev_1  (const ev_1& e) { (void)(e); std::cout << "event 1 -> ";   }
    void on_ev_2  (const ev_2& e) { (void)(e); std::cout << "event 2 -> ";   }
    void on_ev_3  (const ev_3& e) { (void)(e); std::cout << "event 3 -> ";   }
    void on_ev_4  (const ev_4& e) { (void)(e); std::cout << "event 4 -> ";   }
    void on_ev_5_1(const ev_5& e) { (void)(e); std::cout << "event 5_1 -> "; }
    void on_ev_5_2(const ev_5& e) { (void)(e); std::cout << "event 5_2 -> "; }
    void on_ev_6  (const ev_6& e) { (void)(e); std::cout << "event 6 -> ";   }

    bool g_ev_1  (const ev_1& e) { (void)(e); std::cout << "guard 1 -> ";     return true;    }
    bool g_ev_2  (const ev_2& e) { (void)(e); std::cout << "guard 2 -> ";     return true;    }
    bool g_ev_3  (const ev_3& e) { (void)(e); std::cout << "guard 3 -> ";     return true;    }
    bool g_ev_4  (const ev_4& e) { (void)(e); std::cout << "guard 4 -> ";     return true;    }
    bool g_ev_5_1(const ev_5& e) { (void)(e); std::cout << "guard 5_1 -> ";   return e.val;   }
    bool g_ev_5_2(const ev_5& e) { (void)(e); std::cout << "guard 5_2 -> ";   return ! e.val; }
    bool g_ev_6  (const ev_6& e) { (void)(e); std::cout << "guard 6 -> ";     return true;    }
    template<typename ev>
    bool g_false (const ev& e)   { (void)(e); std::cout << "false guard -> "; return false;   }
    template<typename ev>
    bool g_true (const ev& e)    { (void)(e); std::cout << "true guard -> ";  return true;    }

    void on_s_1_enter() { std::cout << "s_1 enter -> "; }
    void on_s_1_exit()  { std::cout << "s_1 exit -> ";  }
    void on_s_2_enter() { std::cout << "s_2 enter -> "; }
    void on_s_2_exit()  { std::cout << "s_2 exit -> ";  }
    void on_s_3_enter() { std::cout << "s_3 enter -> "; }
    void on_s_3_exit()  { std::cout << "s_3 exit -> ";  }
    void on_s_4_enter() { std::cout << "s_4 enter -> "; }
    void on_s_4_exit()  { std::cout << "s_4 exit -> ";  }
    void on_s_5_enter() { std::cout << "s_5 enter -> "; }
    void on_s_5_exit()  { std::cout << "s_5 exit -> ";  }

    typedef test_fsm f;

    typedef transition_table<
//         | Start  | Event |  Next  |  Callback  |     Guard   |
// --------|--------|-------|--------|------------|-------------|
        row< st::s_1, ev_1, st::s_2, &f::on_ev_1,   &f::g_ev_1    >,
        row< st::s_2, ev_2, st::s_3, &f::on_ev_2,   &f::g_ev_2    >,
        row< st::s_3, ev_3, st::s_4, &f::on_ev_3,   &f::g_ev_3    >,
        row< st::s_4, ev_4, st::s_5, &f::on_ev_4,   &f::g_ev_4    >,
        row< st::s_5, ev_5, st::s_1, &f::on_ev_5_1, &f::g_ev_5_1  >,
        row< st::s_5, ev_5, st::s_3, &f::on_ev_5_2, &f::g_ev_5_2  >,
// --------+--------+-------+--------+------------+-------------|
        row< st::s_2, ev_6, st::s_4, &f::on_ev_6,   &f::g_ev_6    >,
        row< st::s_3, ev_6, st::s_5, &f::on_ev_6                  >,
        row< st::s_5, ev_6, st::s_1, &f::on_ev_6,   &f::g_false   >
    > transition_table_t;

    typedef callback_table<
        scb< st::s_1, &f::on_s_1_enter, &f::on_s_1_exit >,
        scb< st::s_2, &f::on_s_2_enter                  >,
        scb< st::s_3, nullptr         , &f::on_s_3_exit >,
        scb< st::s_5, &f::on_s_5_enter, &f::on_s_5_exit >
    > callback_table_t;

public:
    template<typename event_t>
    st process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }
};

struct play {};
struct open_close {};
struct cd_detected { 
    cd_detected(char const* const name, std::vector<float> const& tracks_duration) :
    m_name(name), m_tracks(tracks_duration)
    {}

    const char* const     m_name;
    std::vector<float> m_tracks;
};
struct pause {};
struct stop {};

enum class player_state {
    empty,
    open,
    stopped,
    playing,
    paused
};

class player : public ecl::state_machine<player, player_state, player_state::empty>
{
    void start_playback(play const& e)
    {
        (void)(e);
        std::cout << "start playback" << std::endl; 
    }

    void open_drawer(open_close const& e)
    { 
        (void)(e); 
        std::cout << "open drawer" << std::endl; 
    }

    void close_drawer(open_close const& e)
    { 
        (void)(e); 
        std::cout << "close drawer" << std::endl; 
    }

    void store_cd_info(cd_detected const& e)
    { 
        std::cout << "store cd info:" << std::endl << e.m_name << std::endl;
        for(auto& i: e.m_tracks) {
            std::cout << "Track: " << i << std::endl;
        }
    }

    void stop_playback(stop const& e)
    { 
        (void)(e); 
        std::cout << "stop playback" << std::endl; 
    }

    void pause_playback(pause const& e)
    { 
        (void)(e); 
        std::cout << "pause playback" << std::endl; 
    }

    void resume_playback(play const& e)
    { 
        (void)(e); 
        std::cout << "resume playback" << std::endl; 
    }

    void stop_and_open(open_close const& e)
    { 
        (void)(e); 
        std::cout << "stop and open" << std::endl; 
    }

    typedef player_state s;
    typedef player       p;

    typedef transition_table<
    //      Start         Event         Next            Action
    //  +------------+-------------+------------+---------------------+
    row < s::stopped , play        , s::playing , &p::start_playback  >,
    row < s::stopped , open_close  , s::open    , &p::open_drawer     >,
    //  +------------+-------------+------------+---------------------+
    row < s::open    , open_close  , s::empty   , &p::close_drawer    >,
    //  +------------+-------------+------------+---------------------+
    row < s::empty   , open_close  , s::open    , &p::open_drawer     >,
    row < s::empty   , cd_detected , s::stopped , &p::store_cd_info   >,
    //  +------------+-------------+------------+---------------------+
    row < s::playing , stop        , s::stopped , &p::stop_playback   >,
    row < s::playing , pause       , s::paused  , &p::pause_playback  >,
    row < s::playing , open_close  , s::open    , &p::stop_and_open   >,
    //  +------------+-------------+------------+---------------------+
    row < s::paused  , play        , s::playing , &p::resume_playback >,
    row < s::paused  , stop        , s::stopped , &p::stop_playback   >,
    row < s::paused  , open_close  , s::open    , &p::stop_and_open   >
    //  +------------+-------------+------------+---------------------+
    > transition_table_t;

    typedef callback_table<> callback_table_t;

public:
    template<typename event_t>
    s process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }
};

void out_state(st s)
{
	switch(s) {
		case st::s_1:
			std::cout << "s_1" << std::endl;
		break;
		case st::s_2:
			std::cout << "s_2" << std::endl;
		break;
		case st::s_3:
			std::cout << "s_3" << std::endl;
		break;
		case st::s_4:
			std::cout << "s_4" << std::endl;
		break;
		case st::s_5:
			std::cout << "s_5" << std::endl;
		break;
	}
}

int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);
    
    test_fsm fsm;

    std::cout << std::endl << "Test fsm" << std::endl;

    std::cout << "sizeof(test_fsm): " << sizeof(test_fsm) << std::endl;

    struct ev_1 ev1;
    struct ev_2 ev2;
    struct ev_3 ev3;
    struct ev_4 ev4;
    struct ev_5 ev5;
    struct ev_6 ev6;

    st s;

    fsm.reset();
    std::cout << "TEST 1 e5.val = true" << std::endl;
    std::cout << "initial state: ";
    out_state(fsm.state());

    // 1 -> 2 -> 3 -> 4 -> 5 -> 1
    s = fsm.process_event(ev1);
    out_state(s);
    s = fsm.process_event(ev2);
    out_state(s);
    s = fsm.process_event(ev3);
    out_state(s);
    s = fsm.process_event(ev4);
    out_state(s);
    ev5.val = true;
    s = fsm.process_event(ev5);
    out_state(s);

    fsm.reset();
    std::cout << "TEST 1 e5.val = false" << std::endl;
    std::cout << "initial state: ";
    out_state(fsm.state());

    // 1 -> 2 -> 3 -> 4 -> 5 -> 1
    s = fsm.process_event(ev1);
    out_state(s);
    s = fsm.process_event(ev2);
    out_state(s);
    s = fsm.process_event(ev3);
    out_state(s);
    s = fsm.process_event(ev4);
    out_state(s);
    ev5.val = false;
    s = fsm.process_event(ev5);
    out_state(s);

    fsm.reset();
    std::cout << std::endl << "TEST 2" << std::endl;
    std::cout << "initial state: ";
    out_state(fsm.state());

    // 1 -> 2 -> 4 -> 5 -> 5
    s = fsm.process_event(ev1);
    out_state(s);
    s = fsm.process_event(ev6);
    out_state(s);
    s = fsm.process_event(ev4);
    out_state(s);
    s = fsm.process_event(ev6);
    out_state(s);

    fsm.reset();
    std::cout << std::endl << "TEST 3" << std::endl;
    std::cout << "initial state: ";
    out_state(fsm.state());

    // 1 -> 2 -> 3
    s = fsm.process_event(ev1);
    out_state(s);
    s = fsm.process_event(ev2);
    out_state(s);
    s = fsm.process_event(ev6);
    out_state(s);

    std::cout << std::endl << "Player fsm" << std::endl;

    player p;
    std::cout << "sizeof(player): " << sizeof(player) << std::endl;

    p.process_event(open_close()); // user opens CD player
    p.process_event(open_close()); // inserts CD and closes
    std::vector<float> tracks(7);
    tracks[0] = 3.08;
    tracks[1] = 4.34;
    tracks[2] = 2.58;
    tracks[3] = 5.01;
    tracks[4] = 4.12;
    tracks[5] = 3.24;
    tracks[6] = 1.23;

    p.process_event(               // CD is detected
        cd_detected(
             "CD disk"
           , tracks
        )
    );
    p.process_event(play());       // etc.
    p.process_event(pause());
    p.process_event(play());
    p.process_event(stop());
}