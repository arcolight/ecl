#ifndef ECL_TEST_FSM_HPP
#define ECL_TEST_FSM_HPP

#include <ecl/fsm.hpp>

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

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
        log.push_back("start_playback");
    }

    void open_drawer(open_close const&)
    {
        log.push_back("open_drawer");
    }

    void close_drawer(open_close const&)
    {
        log.push_back("close_drawer");
    }

    void store_cd_info(cd_detected const& e)
    {
        log.push_back("store_cd_info");
        cd_name      = e.m_name;
        tracks_count = e.m_tracks.size();
    }

    void stop_playback(stop const&)
    {
        log.push_back("stop_playback");
    }

    void pause_playback(pause const&)
    {
        log.push_back("pause_playback");
    }

    void resume_playback(play const&)
    {
        log.push_back("resume_playback");
    }

    void stop_and_open(open_close const&)
    {
        log.push_back("stop_and_open");
    }

    bool has_tracks(cd_detected const& e)
    {
        return !e.m_tracks.empty();
    }

    void on_enter_playing() { log.push_back("enter_playing"); }
    void on_exit_playing()  { log.push_back("exit_playing");  }
    void on_enter_open()    { log.push_back("enter_open");    }

    using s = player_state;
    using p = player;

    using transition_table_t = transition_table
    <
        //      Start         Event         Next            Action                Guard
        //  +------------+-------------+------------+---------------------+-----------------+
        row < s::stopped , play        , s::playing , &p::start_playback                    >,
        row < s::stopped , open_close  , s::open    , &p::open_drawer                       >,
        //  +------------+-------------+------------+---------------------+-----------------+
        row < s::open    , open_close  , s::empty   , &p::close_drawer                      >,
        //  +------------+-------------+------------+---------------------+-----------------+
        row < s::empty   , open_close  , s::open    , &p::open_drawer                       >,
        row < s::empty   , cd_detected , s::stopped , &p::store_cd_info   , &p::has_tracks  >,
        //  +------------+-------------+------------+---------------------+-----------------+
        row < s::playing , stop        , s::stopped , &p::stop_playback                     >,
        row < s::playing , pause       , s::paused  , &p::pause_playback                    >,
        row < s::playing , open_close  , s::open    , &p::stop_and_open                     >,
        //  +------------+-------------+------------+---------------------+-----------------+
        row < s::paused  , play        , s::playing , &p::resume_playback                   >,
        row < s::paused  , stop        , s::stopped , &p::stop_playback                     >,
        row < s::paused  , open_close  , s::open    , &p::stop_and_open                     >
        //  +------------+-------------+------------+---------------------+-----------------+
    >;

    using callback_table_t = callback_table
    <
        scb< s::playing , &p::on_enter_playing , &p::on_exit_playing >,
        scb< s::open    , &p::on_enter_open                          >
    >;

public:
    player()
        : log()
        , cd_name()
    {}

    template<typename event_t>
    s process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }

    std::vector<std::string> log;
    std::string              cd_name;
    std::size_t              tracks_count { 0 };
};

#define ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(current, expected) \
    BOOST_CHECK_MESSAGE(current == expected, out_state(current) << " = " << out_state(expected))

static cd_detected make_cd()
{
    return cd_detected("CD disk", { 3.08, 4.34, 2.58, 5.01, 4.12, 3.24, 1.23 });
}

BOOST_AUTO_TEST_CASE( player_case )
{
    player p;
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::empty);

    p.process_event(open_close()); // user opens CD player
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::open);

    p.process_event(open_close()); // inserts CD and closes
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::empty);

    p.process_event(make_cd());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::stopped);
    BOOST_CHECK_EQUAL(p.cd_name, "CD disk");
    BOOST_CHECK_EQUAL(p.tracks_count, 7u);

    p.process_event(play());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::playing);

    p.process_event(pause());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::paused);

    p.process_event(play());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::playing);

    p.process_event(stop());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::stopped);
}

BOOST_AUTO_TEST_CASE( process_event_return_case )
{
    player p;
    player_state s = p.process_event(open_close());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(s, player_state::open);

    s = p.process_event(open_close());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(s, player_state::empty);

    s = p.process_event(play());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(s, player_state::empty);
}

BOOST_AUTO_TEST_CASE( actions_and_callbacks_order_case )
{
    player p;

    p.process_event(make_cd());
    p.process_event(play());
    p.process_event(pause());
    p.process_event(play());
    p.process_event(open_close());

    const std::vector<std::string> expected
    {
        "store_cd_info",
        "start_playback",  "enter_playing",
        "pause_playback",  "exit_playing",
        "resume_playback", "enter_playing",
        "stop_and_open",   "exit_playing", "enter_open"
    };

    BOOST_CHECK_EQUAL_COLLECTIONS(p.log.begin(), p.log.end(),
                                  expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE( unhandled_event_case )
{
    player p;

    p.process_event(play());
    p.process_event(pause());
    p.process_event(stop());

    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::empty);
    BOOST_CHECK(p.log.empty());

    p.process_event(make_cd());
    p.process_event(make_cd());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::stopped);
    BOOST_CHECK_EQUAL(p.log.size(), 1u);
}

BOOST_AUTO_TEST_CASE( guard_case )
{
    player p;

    p.process_event(cd_detected("empty disk", {}));
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::empty);
    BOOST_CHECK(p.log.empty());
    BOOST_CHECK(p.cd_name.empty());

    p.process_event(make_cd());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::stopped);
}

BOOST_AUTO_TEST_CASE( reset_case )
{
    player p;

    p.process_event(make_cd());
    p.process_event(play());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::playing);

    p.reset();
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::empty);

    p.process_event(open_close());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p.state(), player_state::open);
}

BOOST_AUTO_TEST_CASE( independent_instances_case )
{
    player p1;
    player p2;

    p1.process_event(open_close());
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p1.state(), player_state::open);
    ECL_BOOST_CHECK_MESSAGE_PLAYER_FSM(p2.state(), player_state::empty);

    p2.process_event(make_cd());
    BOOST_CHECK_EQUAL(p2.log.size(), 1u);
    BOOST_CHECK_EQUAL(p1.log.size(), 2u);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_FSM_HPP
