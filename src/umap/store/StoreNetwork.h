//////////////////////////////////////////////////////////////////////////////
// Copyright 2017-2020 Lawrence Livermore National Security, LLC and other
// UMAP Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: LGPL-2.1-only
//////////////////////////////////////////////////////////////////////////////

#ifndef _UMAP_NETWORK_STORE_H_
#define _UMAP_NETWORK_STORE_H_
#include <cstdint>
#include "umap/store/Store.hpp"
#include "umap/umap.h"


namespace Umap {
  class StoreNetwork : public Store {
    public:
      StoreNetwork(std::size_t _rsize_);
      ~StoreNetwork();
      ssize_t read_from_store(char* buf, size_t nb, off_t off);
      ssize_t write_to_store(char* buf, size_t nb, off_t off);
    private:
      void* region;
      void* alignment_buffer;
      size_t rsize;
      size_t alignsize;
      int fd;

      int rank;
      int size;
      bool is_server;
      int umap_rpc_id;
  };
}
#endif
