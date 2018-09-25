/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright 2011-2018 Dominik Charousset                                     *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#pragma once

#include "caf/actor.hpp"
#include "caf/io/newb.hpp"

namespace caf {
namespace policy {

struct new_raw_msg {
  char* payload;
  size_t payload_len;
};

template <class Inspector>
typename Inspector::result_type inspect(Inspector& fun, new_raw_msg& data) {
  return fun(meta::type_name("new_raw_msg"), data.payload_len);
}

struct raw {
  using message_type = new_raw_msg;
  using result_type = optional<message_type>;
  io::newb<message_type>* parent;
  message_type msg;

  void init(io::newb<message_type>* n) {
    this->parent = n;
  }

  error read(char* bytes, size_t count) {
    msg.payload = bytes;
    msg.payload_len = count;
    parent->handle(msg);
    return none;
  }

  error timeout(atom_value, uint32_t) {
    return none;
  }

  size_t write_header(io::byte_buffer&,
                      io::header_writer*) {
    return 0;
  }

  void prepare_for_sending(io::byte_buffer&, size_t, size_t, size_t) {
    // nop
  }
};

} // namespace policy
} // namespace caf