#ifndef LIBTORRENT_DEPS
#define LIBTORRENT_DEPS

#include <boost/smart_ptr/make_shared.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/entry.hpp>

#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/torrent_status.hpp>

// Show torrent status
std::string state(libtorrent::torrent_status::state_t s) {
  switch (s) {
    case libtorrent::torrent_status::checking_files:
      return std::string("Checking");
    case libtorrent::torrent_status::downloading_metadata:
      return std::string("Dl metadata");
    case libtorrent::torrent_status::downloading:
      return std::string("Downloading");
    case libtorrent::torrent_status::finished:
      return std::string("Finished");
    case libtorrent::torrent_status::seeding:
      return std::string("Seeding");
    case libtorrent::torrent_status::allocating:
      return std::string("Allocating");
    case libtorrent::torrent_status::checking_resume_data:
      return std::string("Checking resume");
    default:
      return std::string("<>");
  }
}

namespace gui {
std::string torrent_status(libtorrent::torrent_status stat) {
  std::stringstream ss;
  ss << "\r" << std::left << std::setw(17) << state(stat.state) << " "
     << std::left << std::setw(80) << stat.name << " " << stat.save_path << " "
     << (stat.download_payload_rate / 1000) << " kB/s "
     << (stat.upload_payload_rate / 1000) << " kB/s "
     << (stat.total_done / 1000) << "/" << (stat.total_wanted / 1000) << " kB ("
     << (stat.progress_ppm / 10000) << "%)"
     << " : " << stat.num_peers << " peers : " << stat.num_seeds << " seeders"
     << std::endl;
  return ss.str();
}

}  // namespace gui

#endif