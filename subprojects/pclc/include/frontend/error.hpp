// -- Part of ParaCL Compiler ------------------------------------------------//
//    ___                _______     _____                _ __                //
//   / _ \___ ________ _/ ___/ /    / ___/__  __ _  ___  (_) /__ ____         //
//  / ___/ _ `/ __/ _ `/ /__/ /__  / /__/ _ \/  ' \/ _ \/ / / -_) __/         //
// /_/   \_,_/_/  \_,_/\___/____/  \___/\___/_/_/_/ .__/_/_/\__/_/            //
//                                               /_/                          //
// -- This code is licensed under GPLv3 license (see LICENSE for details) --- //

#pragma once

#include "frontend/source.hpp"
#include "location.hpp"

#include <fmt/core.h>

#include <string>
#include <vector>

namespace paracl::frontend {

struct error_kind final {
  std::string m_error_message;
  location m_loc;
};

struct error_attachment final {
  std::string m_info_message;
  location m_loc;
};

struct error_report final {
  error_kind m_primary_error;
  std::vector<error_attachment> m_attachments;

public:
  error_report(const error_kind &primary) : m_primary_error(primary) {}
  void add_attachment(error_attachment attach) {
    m_attachments.push_back(std::move(attach));
  }
};

using error_queue_type = std::vector<error_report>;

class error_reporter {
private:
  const source_input *m_source;

private:
  void print_message_location(std::string_view msg, location loc) {
    constexpr int offset_width = 8;

    const auto make_squigly_line = [](int column) {
      constexpr int max_squigly_width = 4;
      int squigly_width = std::min(max_squigly_width, column - 1);

      std::stringstream ss;
      for (int i = 0; i < column - squigly_width - 1; ++i) {
        ss << " ";
      }

      const auto make_squigly = [&](auto width) {
        for (int i = 0; i < width; ++i) {
          ss << "~";
        }
      };

      make_squigly(squigly_width);
      ss << "^";
      make_squigly(max_squigly_width);

      return ss.str();
    };

    const auto loc_to_str = [](auto &&loc) {
      std::stringstream ss;
      ss << loc;
      return ss.str();
    };

    fmt::println("{}: {}", loc_to_str(loc), msg);
    if (loc.begin.line == loc.end.line) {
      fmt::println("{:<{}}{}", fmt::format("{} |", loc.begin.line),
                   offset_width, m_source->getline(loc.begin.line));
      fmt::println("{:<{}}{}", "", offset_width,
                   make_squigly_line(loc.begin.column));
      return;
    }

    constexpr int c_max_lines = 4;
    auto last_line = std::min(loc.end.line, loc.begin.line + c_max_lines);

    // Here we handle multiline errors
    fmt::println("{:<{}}{}", fmt::format("{} |", loc.begin.line), offset_width,
                 m_source->getline(loc.begin.line));
    for (auto start = loc.begin.line + 1, finish = last_line; start <= finish;
         ++start) {
      fmt::println("{:<{}}{}", "", offset_width, m_source->getline(start));
    }
  }

public:
  error_reporter(const source_input &source) : m_source{&source} {}

  void report_pretty_error(error_kind err) {
    auto [msg, loc] = err;

    const std::string bison_syntax = "syntax error";
    if (err.m_error_message.starts_with(bison_syntax)) {
      auto &str =
          err.m_error_message; // Hacky workaround to capitalize bison syntax
                               // error. Should rework later. TODO[Sergei]
      str.replace(str.find(bison_syntax), bison_syntax.length(),
                  "Syntax error");
    }

    print_message_location(err.m_error_message, err.m_loc);
  }

  void report_pretty_error(error_report report) {
    report_pretty_error(report.m_primary_error);
    for (const auto &attach : report.m_attachments) {
      fmt::println("");
      print_message_location(attach.m_info_message, attach.m_loc);
    }
  }

  void switch_source(const source_input &source) { m_source = &source; }
};

} // namespace paracl::frontend
