#ifndef ECL_TEST_FSM_HPP
#define ECL_TEST_FSM_HPP

#include <ecl/fsm.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE( fsm_suite )

struct play {};
struct open_close {};
struct cd_detected {
    cd_detected(std::string name, std::vector<double> const& tracks_duration) :
    m_name(name), m_tracks(tracks_duration)
    {}

    std::string         m_name;
    std::vector<double> m_tracks;
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

std::string out_state(player_state s)
{
    switch(s)
    {
        case player_state::empty:   return "empty";   break;
        case player_state::open:    return "open";    break;
        case player_state::stopped: return "stopped"; break;
        case player_state::playing: return "playing"; break;
        case player_state::paused:  return "paused";  break;
    }
    return "we should not be there...";
}

class player : public ecl::state_machine<player, player_state, player_state::empty>
{
    void start_playback(play const&)
    {
        BOOST_TEST_MESSAGE("start playback");
    }

    void open_drawer(open_close const&)
    {
        BOOST_TEST_MESSAGE("open drawer");
    }

    void close_drawer(open_close const&)
    {
        BOOST_TEST_MESSAGE("close drawer");
    }

    void store_cd_info(cd_detected const& e)
    {
        BOOST_TEST_MESSAGE("store cd info:" << e.m_name);
        for(auto& i: e.m_tracks)
        {
            BOOST_TEST_MESSAGE("Track: " << i);
        }
    }

    void stop_playback(stop const&)
    {
        BOOST_TEST_MESSAGE("stop playback");
    }

    void pause_playback(pause const&)
    {
        BOOST_TEST_MESSAGE("pause playback");
    }

    void resume_playback(play const&)
    {
        BOOST_TEST_MESSAGE("resume playback");
    }

    void stop_and_open(open_close const&)
    {
        BOOST_TEST_MESSAGE("stop and open");
    }

    using s = player_state;
    using p = player;

    using transition_table_t = transition_table
    <
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
    >;

    using callback_table_t = callback_table<>;

public:
    template<typename event_t>
    s process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }
};

#define ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(current, expected) \
    BOOST_CHECK_MESSAGE(current == expected, out_state(current) << " = " << out_state(expected))

BOOST_AUTO_TEST_CASE( player_case )
{
    player p;
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::empty);

    p.process_event(open_close()); // user opens CD player
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::open);

    p.process_event(open_close()); // inserts CD and closes
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::empty);

    std::vector<double> tracks(7);
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
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::playing);

    p.process_event(pause());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::paused);

    p.process_event(play());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::playing);

    p.process_event(stop());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::stopped);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_FSM_HPP
