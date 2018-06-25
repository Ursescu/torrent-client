#include <chrono>
#include <exception>
#include <iostream>
#include <map>
#include <sstream>
#include <thread>

#include "libtorrent_deps.hpp"
#include "console_gui.hpp"

#define TORRENT_PATH "./torrents/"

namespace lt = libtorrent;

int main(int argc, char const* argv[]) {
  initscr();             // init ncurses
  cbreak();              // close with Ctrl+c
  keypad(stdscr, true);  // allow to press keys
  noecho();              // echo key pressed

  std::map<std::string, lt::torrent_status> torrent_cache;

  lt::settings_pack pack;
  pack.set_int(lt::settings_pack::alert_mask,
               lt::alert::error_notification | lt::alert::storage_notification |
                   lt::alert::status_notification);

  lt::session ses(pack);

  std::vector<lt::add_torrent_params> torrents_to_add;
  std::vector<lt::torrent_status> cached_status;

  lt::error_code ec;

  for (int i = 1; i < argc; i++) {
    lt::add_torrent_params atp;
    atp.save_path = TORRENT_PATH;
    atp.ti = boost::make_shared<lt::torrent_info>(std::string(argv[i]),
                                                  boost::ref(ec), 0);
    atp.flags = lt::add_torrent_params::flag_duplicate_is_error |
                lt::add_torrent_params::flag_update_subscribe;
    torrents_to_add.push_back(atp);
  }

  std::vector<lt::torrent_handle> handlers;


  for (lt::add_torrent_params const &a : torrents_to_add) {
    lt::torrent_handle h =
        ses.add_torrent(a, ec);  // Add the torrent in sync mode, consider
                                 // async_add_torrent in future
    if (!ec) {
      lt::torrent_status stat = h.status();

      torrent_cache[stat.name] = stat;

      h.auto_managed(true);
      handlers.push_back(h);
    }
  }


  // Attempt to understand alert system, still need research

  for (;;) {
    // Drawing torrent status to screen
    erase();
    move(0, 0);
    for (auto map_i = torrent_cache.begin(); map_i != torrent_cache.end();
         ++map_i) {
      lt::torrent_status local_stat = map_i->second;
      std::string torrent_log = gui::torrent_status(local_stat);
      printw(torrent_log.c_str());
    }
    // updating after all
    refresh();

    // View alerts and for now just update the torrent status
    std::vector<lt::alert*> alerts;
    ses.pop_alerts(&alerts);

    for (lt::alert const* a : alerts) {
      if (lt::alert_cast<lt::torrent_finished_alert>(a)) {
        // Placeholder
      }
      if (lt::alert_cast<lt::torrent_error_alert>(a)) {
        // Placeholder
      }
      if (auto st = lt::alert_cast<lt::state_update_alert>(a)) {
        if (st->status.empty()) continue;  // nothing to update

        for (lt::torrent_status const& stat : st->status) {
          torrent_cache[stat.name] = stat;
        }
      }
    }

    //Slow down 
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    ses.post_torrent_updates();
  }

  endwin();
  return 0;
}
